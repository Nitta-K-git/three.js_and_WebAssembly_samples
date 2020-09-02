# C++のvector形をjsに渡す

Vector型で保持しているデータをjs側に渡したい

[公式リファレンス](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#built-in-type-conversions)でデフォルトで型の互換性がとれるようになっている

Vectorのサンプルを動かすときは、cmakeの設定に注意する

このサンプルの方法は値渡し



https://github.com/emscripten-core/emscripten/issues/5651

以下のように--bindのオプションを明示的に加えないといけない

```cmake
cmake_minimum_required(VERSION 2.8)
project(embind_test CXX)
 
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
 
if (EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "--bind")
    # set(CMAKE_EXECUTABLE_SUFFIX ".html") # デフォルトでは.js
else()
    set( EM_DIR C:/Users/kyohei_nitta/Documents/GitHub/emsdk/upstream/emscripten/system/include)
    include_directories(${EM_DIR})
endif()
 
add_executable(emc_sample mesh.cpp)
```

