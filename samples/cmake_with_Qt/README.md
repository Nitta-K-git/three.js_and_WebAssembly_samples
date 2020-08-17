# CMAKEのプロジェクトをIDEで扱う

emsdkのパスをcmakeファイルに追加するとQtなどのIDEで自動補完機能が使える

```cmake
project( embind_test )
 
set(CMAKE_BUILD_TYPE Debug)
 
if (EMSCRIPTEN)
    # set(CMAKE_EXECUTABLE_SUFFIX ".html") # デフォルトでは.js
else()
    set( EM_DIR C:/Users/kyohei_nitta/Documents/GitHub/emsdk/upstream/emscripten/system/include)
    include_directories(${EM_DIR})
endif()
 
add_executable(emc_sample mesh.cpp)
```





