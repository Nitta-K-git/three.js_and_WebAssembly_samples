#include <iostream>
#include <meshlab/common/ml_mesh_type.h>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/complex.h>

#include <vcg/complex/algorithms/update/curvature.h>
#include <vcg/complex/algorithms/update/color.h>
#include <vcg/complex/algorithms/clean.h>
#include <vcg/complex/algorithms/create/resampler.h>
#include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h>
#include <vcg/complex/algorithms/local_optimization.h>
// #include "quadric_simp.h" // for simplification collapse edge
#include <vcg/complex/algorithms/clustering.h>
#include <vcg/complex/algorithms/create/plymc/plymc.h>

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

CMeshO mesh;

// int main()
// {
//     CMeshO mesh;
//     std::cout << "Hello World" << std::endl;
// }

void print_array(std::vector<int> &v){
	for(auto a:v){
		printf("%d\n", a);
	}
}
void print_vvarray(std::vector<std::vector<int>> &vv){
	for(auto v:vv){
		for(auto a:v){
			printf("%d\n", a);
		}
	}
}
void print_jsarray_double(val v){
	auto l = v["length"].as<unsigned>();
	std::vector<double> rv;
	for(unsigned i = 0; i < l; ++i) {
		double d = v[i].as<double>();
		rv.push_back(d);
		printf("%lf\n", d);
	}
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

EMSCRIPTEN_BINDINGS(module) {
	emscripten::function("print_array", &print_array);
	emscripten::function("print_vvarray", &print_vvarray);
	emscripten::function("print_jsarray_double", &print_jsarray_double);
	emscripten::function("init_mesh", &init_mesh);
	
	register_vector<int>("VectorInt");
	register_vector<std::vector<int>>("VVInt");
}
