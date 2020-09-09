# C++のvector形をjsに渡す

- [emscripten の bind で JavaScript オブジェクトを返す方法](https://semiexp.net/blog/2017-05-13-emscripten_bind.html)

Vector型で保持しているデータをjs側に渡したい

[公式リファレンス](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#built-in-type-conversions)でデフォルトで型の互換性がとれるようになっている

Vectorのサンプルを動かすときは、cmakeの設定に注意する

このサンプルの方法は値渡し



https://github.com/emscripten-core/emscripten/issues/5651

以下のように--bindのオプションを明示的に加えないといけない

```cmake
cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)

set(CMAKE_CXX_STANDARD 11)

# define path of Eigen
file(TO_CMAKE_PATH $ENV{EIGEN3_INCLUDE_DIR} EIGEN3_INCLUDE_DIR)
# define path of vcglib
file(TO_CMAKE_PATH $ENV{VCGLIB_DIR} VCGLIB_DIR)
# define path of meshlab source
file(TO_CMAKE_PATH $ENV{MESHLAB_DIR} MESHLAB_DIR)
add_definitions(-DMESHLAB_SCALAR=float) # you should add this definition if you use meshlab source

file(TO_CMAKE_PATH $ENV{EMSDK_DIR} EMSDK_DIR)

# set output file directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Add Executable
add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${EIGEN3_INCLUDE_DIR}
  ${VCGLIB_DIR}
  ${MESHLAB_DIR}
  ${EMSDK_DIR}
)

target_sources(${PROJECT_NAME}
  PRIVATE
    main.cpp
)

if (EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "--bind")
endif()

```



main.cpp

- `std::vector`をそのままreturnする
- 使用する型は`register_vector`で宣言しておく
- もしくは`emscripten::val`型の変数に配列データを入れて返す

```cpp
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
```



html

- wasmで定義したものをModuleで呼び出す
- 値の追加なども可能

```html
<!doctype html>
<html>
<script>
  var Module = {
    onRuntimeInitialized: function () {
      var retVector = Module['returnVectorData']();

      // vector size
      var vectorSize = retVector.size();

      // reset vector value
      retVector.set(vectorSize - 1, 11);

      // push value into vector
      retVector.push_back(12);

      // retrieve value from the vector
      for (var i = 0; i < retVector.size(); i++) {
        console.log("Vector Value: ", retVector.get(i));
      }

      // expand vector size
      retVector.resize(20, 1);

      var retMap = Module['returnMapData']();

      // map size
      var mapSize = retMap.size();

      // retrieve value from map
      console.log("Map Value: ", retMap.get(10));

      // figure out which map keys are available
      // NB! You must call `register_vector<key_type>`
      // to make vectors available
      var mapKeys = retMap.keys();
      for (var i = 0; i < mapKeys.size(); i++) {
        var key = mapKeys.get(i);
        console.log("Map key/value: ", key, retMap.get(key));
      }

      // reset the value at the given index position
      retMap.set(10, "OtherValue");
    }
  };
</script>
<script src="vector_to_js.js"></script>

</html>
```

