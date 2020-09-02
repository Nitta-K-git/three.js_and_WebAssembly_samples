#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <string>
#include <vector>

using namespace emscripten;

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
	
	// vector型への変換用関数
	std::vector<double> vd = vecFromJSArray<double>(v);
}

EMSCRIPTEN_BINDINGS(module) {
	function("print_array", &print_array);
	function("print_vvarray", &print_vvarray);
	function("print_jsarray_double", &print_jsarray_double);
	
	register_vector<int>("VectorInt");
	register_vector<std::vector<int>>("VVInt");
}
