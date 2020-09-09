#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <string>
#include <vector>

using namespace emscripten;

std::vector<int> returnVectorData () {
	std::vector<int> v(10, 1);
	return v;
}

std::map<int, std::string> returnMapData () {
	std::map<int, std::string> m;
	m.insert(std::pair<int, std::string>(10, "This is a string."));
	return m;
}

std::vector<int> vi;

void inc_vec(){
	int a = vi.size();
	vi.push_back(a);
}

val getBytes() {
	return val(typed_memory_view(vi.size(), vi.data()));
}

val vector_to_js(){
	val vj = val::array();
	for(int i=0; i<5; ++i)
		vj.call<val>("push", i);
	return vj;
}

EMSCRIPTEN_BINDINGS(module) {
	function("returnVectorData", &returnVectorData);
	function("returnMapData", &returnMapData);
	function("inc_vec", &inc_vec);
	function("getBytes", &getBytes);
	function("retVal", &vector_to_js);
	
	// register bindings for std::vector<int> and std::map<int, std::string>.
	register_vector<int>("vector<int>");
	register_map<int, std::string>("map<int, string>");
}
