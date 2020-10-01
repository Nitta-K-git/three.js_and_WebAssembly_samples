#include <iostream>
#include <common/ml_mesh_type.h>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/complex.h>

#include <vcg/complex/algorithms/update/curvature.h>
#include <vcg/complex/algorithms/update/color.h>
#include <vcg/complex/algorithms/clean.h>
#include <vcg/space/triangle3.h>

#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <string>
#include <vector>

using namespace emscripten;
using namespace std;
using namespace vcg;

class ExMesh{
public:
	CMeshO mesh;
	bool calc_min_curvature(){
		mesh.vert.EnableVFAdjacency();
		mesh.face.EnableFFAdjacency();
		mesh.face.EnableVFAdjacency();
		mesh.vert.EnableCurvature();
		mesh.vert.EnableCurvatureDir();
		
		// delete invalid vertices and faces
		// compactVert(mesh);
		
		// calc normal
		tri::UpdateTopology<CMeshO>::FaceFace(mesh);
		tri::UpdateTopology<CMeshO>::VertexFace(mesh);
		// calc curvature
		tri::UpdateCurvature<CMeshO>::MeanAndGaussian(mesh);
		for(CMeshO::VertexIterator vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
			CMeshO::VertexType *pv = &*vi;
			float H = pv->Kh();
			float G = pv->Kg();
			float a = std::max(0.f, H*H-G); // h^2-gは手法によっては負になる場合があるので0でmaxとる
			MESHLAB_SCALAR sq = sqrt(a);
			pv->Q() = H-sq; // minimum curvature
			//pv->Q() = H+sq; // maximum curvature
		}
		std::pair<float,float> curv_minmax = tri::Stat<CMeshO>::ComputePerVertexQualityMinMax(mesh);
		float curv_min = curv_minmax.first;
		float curv_max = curv_minmax.second;
		printf("curvature min,max = (%f, %f)\n", curv_min, curv_max);
		tri::UpdateColor<CMeshO>::PerVertexQualityRamp(mesh, curv_min, curv_max);
		return true;
	}
	void create_pcd(val vert){
		vector<MESHLAB_SCALAR> vd;
		vd = vecFromJSArray<MESHLAB_SCALAR>(vert);
		
		vector<Point3m> coordVec;
		for(unsigned int i=0; i*3<vd.size(); ++i){
			coordVec.push_back(Point3m(vd[i*3+0], vd[i*3+1], vd[i*3+2]));
		}
		tri::BuildMeshFromCoordVector(mesh,coordVec);
		printf("vn : %d, fn : %d\n", mesh.VN(), mesh.FN());
	}
	void create_mesh(val vert, val face){
		vector<MESHLAB_SCALAR> vd;
		vector<int> vf;
	
		vector<Point3m> coordVec;
		vector<Point3i> indexVec;
		
		vd = vecFromJSArray<MESHLAB_SCALAR>(vert);
		for(unsigned int i=0; i*3<vd.size(); ++i){
			coordVec.push_back(Point3m(vd[i*3+0], vd[i*3+1], vd[i*3+2]));
		}
	
		if(face.isNull() || face["length"].isNull()){
			// 他の面と頂点を共有しない
			for(int i=0; i*3<coordVec.size(); ++i){
				indexVec.push_back(Point3i(i*3+0, i*3+1, i*3+2));
			}
		}else{
			vf = vecFromJSArray<int>(face);
			for(unsigned int i=0; i*3<vf.size(); ++i){
				indexVec.push_back(Point3i(vf[i*3+0], vf[i*3+1], vf[i*3+2]));
			}
		}
		
		tri::BuildMeshFromCoordVectorIndexVector(mesh, coordVec, indexVec);
		printf("vn : %d, fn : %d\n", mesh.VN(), mesh.FN());
	}
	val get_mesh_array(){
		val vo = val::object();
		vo.set("position", val::array());
		vo.set("findex", val::array());
		vo.set("color", val::array());
		for(CMeshO::VertexIterator vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
			Point3m p = vi->P();
			vo["position"].call<val>("push", p.X());
			vo["position"].call<val>("push", p.Y());
			vo["position"].call<val>("push", p.Z());
			
			vo["color"].call<val>("push", vi->cC()[0]/255.0);
			vo["color"].call<val>("push", vi->cC()[1]/255.0);
			vo["color"].call<val>("push", vi->cC()[2]/255.0);
		}
		for(CMeshO::FaceIterator fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){
			for(int i=0;i<fi->VN();++i)
				vo["findex"].call<val>("push", tri::Index(mesh, fi->V(i)));
		}
		return vo;
	}
};

int main(){
    std::cout << "Hello MeshLab" << std::endl;
}

void hello(){
	std::cout << "Hello!!" << std::endl;
}

EMSCRIPTEN_BINDINGS(module) {
	emscripten::function("hello", &hello);
}

EMSCRIPTEN_BINDINGS(my_class_example) {
	class_<ExMesh>("ExMesh")
			.constructor()
			.function("calc_min_curvature", &ExMesh::calc_min_curvature)
			.function("create_pcd", &ExMesh::create_pcd)
			.function("create_mesh", &ExMesh::create_mesh)
			.function("get_mesh_array", &ExMesh::get_mesh_array)
			;
}
