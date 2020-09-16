#include <iostream>
#include <common/ml_mesh_type.h>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/complex.h>

#include <vcg/complex/algorithms/update/curvature.h>
#include <vcg/complex/algorithms/update/color.h>
#include <vcg/complex/algorithms/clean.h>
// #include <vcg/complex/algorithms/create/resampler.h>
// #include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h>
// #include <vcg/complex/algorithms/local_optimization.h>
// #include "quadric_simp.h" // for simplification collapse edge
// #include <vcg/complex/algorithms/clustering.h>
// #include <vcg/complex/algorithms/create/plymc/plymc.h>

// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <algorithm>

#include <vcg/space/triangle3.h>

#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <string>
#include <vector>

using namespace emscripten;
using namespace std;
using namespace vcg;

CMeshO mesh;
vector<MESHLAB_SCALAR> vd;

int main()
{
    std::cout << "Hello MeshLab" << std::endl;
}

void set_js_array(val v){
	vd = vecFromJSArray<MESHLAB_SCALAR>(v);
	cout << "vecFromJSArray" << endl;
//	for(auto && v:vd){
//		cout << v << endl;
//	}
	// load as CMeshO
	vector<Point3m> coordVec;
	vector<Point3i> indexVec;
	
	for(unsigned int i=0; i*3<vd.size(); ++i){
		coordVec.push_back(Point3m(vd[i*3+0], vd[i*3+1], vd[i*3+2]));
	}
	tri::BuildMeshFromCoordVector(mesh,coordVec);
}

void process_data(){
	
}

val vector_to_js(){
	tri::Dodecahedron(mesh);
//	// val型の値を返す
//	val vj = val::array();
//	for(CMeshO::VertexIterator vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
//		Point3m p = vi->P();
//		vj.call<val>("push", p.X());
//		vj.call<val>("push", p.Y());
//		vj.call<val>("push", p.Z());
//	}
//	return vj;
	val vo = val::object();
	vo.set("position", val::array());
	vo.set("findex", val::array());
	for(CMeshO::VertexIterator vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
		Point3m p = vi->P();
		vo["position"].call<val>("push", p.X());
		vo["position"].call<val>("push", p.Y());
		vo["position"].call<val>("push", p.Z());
	}
	for(CMeshO::FaceIterator fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){
		for(int i=0;i<fi->VN();++i)
			vo["findex"].call<val>("push", tri::Index(mesh, fi->V(i)));
	}
	return vo;

}

EMSCRIPTEN_BINDINGS(module) {
	emscripten::function("set_js_array", &set_js_array);
	emscripten::function("process_data", &process_data);
	emscripten::function("vector_to_js", &vector_to_js);
}
