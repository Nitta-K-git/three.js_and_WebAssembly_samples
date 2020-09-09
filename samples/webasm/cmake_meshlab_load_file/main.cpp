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

CMeshO mesh;
vector<double> vd;

int main()
{
    std::cout << "Hello MeshLab" << std::endl;
}

void init_mesh(val v){
	printf("called\n");
    std::vector<float> vd = vecFromJSArray<float>(v);
	for(auto a:vd){
		std::cout << a << std::endl;
	}
	int size = static_cast<int>(vd.size())/3;
	std::cout << "size: " << size << std::endl;
	auto vi = vcg::tri::Allocator<CMeshO>::AddVertices(mesh, 3);
	for(int i=0; i<size; ++i){
		vi[i].P() = Point3m(vd[i*3], vd[i*3+1], vd[i*3+2]);
	}
	std::cout<<"mesh:"<<mesh.VN() << std::endl;
	CMeshO::VertexIterator vv;
	for(vv=mesh.vert.begin(); vv!=mesh.vert.end(); ++vv){
		std::cout << vv->P().X() << std::endl;
	}
}

void set_js_array(val v){
	vd = vecFromJSArray<double>(v);
	cout << "vecFromJSArray" << endl;
	for(auto && v:vd){
		cout << v << endl;
	}
}

void print_data(){
	cout << "print_data" << endl;
	for(auto && v:vd){
		cout << v << endl;
		//
	}
}

EMSCRIPTEN_BINDINGS(module) {
	emscripten::function("set_js_array", &set_js_array);
	emscripten::function("print_data", &print_data);
	emscripten::function("init_mesh", &init_mesh);
}
