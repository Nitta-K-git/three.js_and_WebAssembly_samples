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

CMeshO mesh;

bool calc_min_curvature(CMeshO &mesh){
	mesh.vert.EnableVFAdjacency();
	mesh.face.EnableFFAdjacency();
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

int main()
{
    std::cout << "Hello MeshLab" << std::endl;
}

void hello(){
	std::cout << "Hello!!" << std::endl;
}

void js_to_asm(val v){
	vector<MESHLAB_SCALAR> vd;
	vd = vecFromJSArray<MESHLAB_SCALAR>(v);
	cout << "vecFromJSArray" << endl;
	vector<Point3m> coordVec;
	vector<Point3i> indexVec;
	
	for(unsigned int i=0; i*3<vd.size(); ++i){
		coordVec.push_back(Point3m(vd[i*3+0], vd[i*3+1], vd[i*3+2]));
	}
	tri::BuildMeshFromCoordVector(mesh,coordVec);
}

// meshlabの関数を呼び出す 
// 引数にgeometryと頂点の選択状態を辞書型で入れて渡す
void calc_curvature(){
	calc_min_curvature(mesh);
}

val asm_to_js(){
//	tri::Dodecahedron(mesh);
	val vo = val::object();
	vo.set("position", val::array());
	vo.set("findex", val::array());
	vo.set("color", val::array());
	for(CMeshO::VertexIterator vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
		Point3m p = vi->P();
		vo["position"].call<val>("push", p.X());
		vo["position"].call<val>("push", p.Y());
		vo["position"].call<val>("push", p.Z());
		
		vo["color"].call<val>("push", vi->cC()[0]);
		vo["color"].call<val>("push", vi->cC()[1]);
		vo["color"].call<val>("push", vi->cC()[2]);
	}
	for(CMeshO::FaceIterator fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){
		for(int i=0;i<fi->VN();++i)
			vo["findex"].call<val>("push", tri::Index(mesh, fi->V(i)));
	}
	return vo;
}

EMSCRIPTEN_BINDINGS(module) {
	emscripten::function("to_asm", &js_to_asm);
	emscripten::function("calc_curvature", &calc_curvature);
	emscripten::function("from_asm", &asm_to_js);
	emscripten::function("hello", &hello);
}
