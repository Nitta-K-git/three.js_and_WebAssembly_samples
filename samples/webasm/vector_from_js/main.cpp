#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <string>
#include <vector>
#include <iostream>

using namespace emscripten;
using namespace std;

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
	cout << "vecFromJSArray" << endl;
	for(auto && v:vd){
		cout << v << endl;
	}
}

EMSCRIPTEN_BINDINGS(module) {
	emscripten::function("print_array", &print_array);
	emscripten::function("print_vvarray", &print_vvarray);
	emscripten::function("print_jsarray_double", &print_jsarray_double);
	
	register_vector<int>("VectorInt");
	register_vector<std::vector<int>>("VVInt");
}
