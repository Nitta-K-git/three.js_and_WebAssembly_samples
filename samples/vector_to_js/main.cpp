//#include <emscripten/bind.h> // embind用ヘッダ
//#include <vector>
//using namespace emscripten;

//class MyMesh{
//public:
//	MyMesh(){
//	}
//	void create(int n){
//		data.clear();
//		for(int i=0;i<n;++i){
//			data.push_back(i);
//		}
//	}
//	std::vector<double> data;
//};

//MyMesh mesh;

//int create(int n) {
//	mesh.create(n);
//	return 1;
//}

//int get(){
//	// メモリを直接渡したい
//}
//// 引数と戻り値の型は自動推論
//EMSCRIPTEN_BINDINGS(my_module) {
//	function("create", &create);
//}


//// Binding code
////EMSCRIPTEN_BINDINGS(my_class_example) {
////	class_<MyClass>("MyClass")
////			.constructor<int, std::string>()
////			.function("incrementX", &MyClass::incrementX)
////			.property("x", &MyClass::getX, &MyClass::setX)
////			.class_function("getStringFromInstance", &MyClass::getStringFromInstance) //static用
////			;
////}

//// compile
//// emcc --bind -o class_example.js class_example.cpp


#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <string>
#include <vector>

using namespace emscripten;

//template <typename T> std::vector<T> vecFromJSArray(const emscripten::val &v)
//{
//	std::vector<T> rv;
	
//	const auto l = v["length"].as<unsigned>();
//	rv.resize(l);
	
//	emscripten::val memoryView{emscripten::typed_memory_view(l, rv.data())};
//	memoryView.call<void>("set", v);
	
//	return rv;
//}

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

EMSCRIPTEN_BINDINGS(module) {
	function("returnVectorData", &returnVectorData);
	function("returnMapData", &returnMapData);
	function("inc_vec", &inc_vec);
	function("getBytes", &getBytes);
	
	// register bindings for std::vector<int> and std::map<int, std::string>.
	register_vector<int>("vector<int>");
	register_map<int, std::string>("map<int, string>");
}
