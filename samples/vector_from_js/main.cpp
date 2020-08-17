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

EMSCRIPTEN_BINDINGS(module) {
	function("print_array", &print_array);
	function("print_vvarray", &print_vvarray);
	
	register_vector<int>("VectorInt");
	register_vector<std::vector<int>>("VVInt");
}
