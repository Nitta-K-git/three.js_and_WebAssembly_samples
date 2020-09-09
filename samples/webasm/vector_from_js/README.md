# jsのvector形をC++に渡す



- [embind: Cannot pass vector of strings from JS to C++ · Issue #8933 · emscripten-core/emscripten](https://github.com/emscripten-core/emscripten/issues/8933)

- C++でバインド定義した型をjs側で使用する方式
  - 二次元以上の配列も扱える
- js側で宣言した配列をC++側でベクターに変換
  - 任意のデータ型の配列を強制的にC++の方でキャストして使える

```cpp
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
```

```html
<!doctype html>
<html>
<script>
  var Module = {
    onRuntimeInitialized: function () {
      const v = new Module.VectorInt();
      const v2 = new Module.VectorInt();
      const vv = new Module.VVInt();
      v.push_back(10);
      v.push_back(1);
      v.push_back(3);
      v.push_back(5);
      v2.push_back(4);
      v2.push_back(8);
      v2.push_back(16);
      vv.push_back(v);
      vv.push_back(v2);

      Module.print_array(v);
      Module.print_array(v2);
      Module.print_vvarray(vv);

      // jsの配列をC++に直接渡す
      var vertices = new Float32Array([1., 10, -23]); // Float型で渡す
      Module.print_jsarray_double(vertices);
      Module.print_jsarray_double(new Int32Array([1, 10, -23])); // Int型で渡す
      Module.print_jsarray_double([1, 10, -23]); // リスト型で渡す
    }
  };
</script>
<script src="vector_from_js.js"></script>

</html>
```

