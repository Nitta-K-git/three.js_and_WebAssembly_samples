# cmakeでコンパイルを行う

## command

```
cmake_template>"C:\Users\Public\Documents\GitHub\emsdk\emsdk_env.bat"
cmake_template>cd emake_build
cmake_template\emake_build>emcmake cmake .. -G "MinGW Makefiles"
cmake_template\emake_build>emmake make
```

以下のファイルをnginxの階層化にコピーしてnginxでhtmlにアクセスする

- cmake_template/emake_build/sample.js
- cmake_template/emake_build/sample.wasm
- cmake_template/sample.html

## example

```
C:\Users\Public\Documents\GitHub\web_assembly_sample\samples\cmake_template>"C:\Users\Public\Documents\GitHub\emsdk\emsdk_env.bat"
Adding directories to PATH:
PATH += C:\Users\Public\Documents\GitHub\emsdk
PATH += C:\Users\Public\Documents\GitHub\emsdk\upstream\emscripten
PATH += C:\Users\Public\Documents\GitHub\emsdk\node\12.18.1_64bit\bin
PATH += C:\Users\Public\Documents\GitHub\emsdk\python\3.7.4-pywin32_64bit
PATH += C:\Users\Public\Documents\GitHub\emsdk\java\8.152_64bit\bin

Setting environment variables:
EMSDK = C:/Users/Public/Documents/GitHub/emsdk
EM_CONFIG = C:\Users\Public\Documents\GitHub\emsdk\.emscripten
EM_CACHE = C:/Users/Public/Documents/GitHub/emsdk/upstream/emscripten\cache
EMSDK_NODE = C:\Users\Public\Documents\GitHub\emsdk\node\12.18.1_64bit\bin\node.exe
EMSDK_PYTHON = C:\Users\Public\Documents\GitHub\emsdk\python\3.7.4-pywin32_64bit\python.exe
JAVA_HOME = C:\Users\Public\Documents\GitHub\emsdk\java\8.152_64bit

C:\Users\Public\Documents\GitHub\web_assembly_sample\samples\cmake_template>cd emake_build

C:\Users\Public\Documents\GitHub\web_assembly_sample\samples\cmake_template\emake_build>emcmake cmake .. -G "MinGW Makefiles"
-- Configuring done
-- Generating done
-- Build files have been written to: C:/Users/Public/Documents/GitHub/web_assembly_sample/samples/cmake_template/emake_build

C:\Users\Public\Documents\GitHub\web_assembly_sample\samples\cmake_template\emake_build>emmake make
make: ['C:\\MinGW\\bin\\mingw32-make.exe']
Scanning dependencies of target sample
[ 50%] Building CXX object CMakeFiles/sample.dir/main.cpp.o
[100%] Linking CXX executable sample.js
cache:INFO: generating system library: libc.a... (this will be cached in "C:\Users\Public\Documents\GitHub\emsdk\upstream\emscripten\cache\wasm-obj\libc.a" for subsequent builds)
cache:INFO:  - ok
cache:INFO: generating system library: libcompiler_rt.a... (this will be cached in "C:\Users\Public\Documents\GitHub\emsdk\upstream\emscripten\cache\wasm-obj\libcompiler_rt.a" for subsequent builds)
cache:INFO:  - ok
cache:INFO: generating system library: libc-wasm.a... (this will be cached in "C:\Users\Public\Documents\GitHub\emsdk\upstream\emscripten\cache\wasm-obj\libc-wasm.a" for subsequent builds)
cache:INFO:  - ok
cache:INFO: generating system library: libc++-noexcept.a... (this will be cached in "C:\Users\Public\Documents\GitHub\emsdk\upstream\emscripten\cache\wasm-obj\libc++-noexcept.a" for subsequent builds)
cache:INFO:  - ok
cache:INFO: generating system library: libc++abi-noexcept.a... (this will be cached in "C:\Users\Public\Documents\GitHub\emsdk\upstream\emscripten\cache\wasm-obj\libc++abi-noexcept.a" for subsequent builds)
cache:INFO:  - ok
cache:INFO: generating system library: libdlmalloc.a... (this will be cached in "C:\Users\Public\Documents\GitHub\emsdk\upstream\emscripten\cache\wasm-obj\libdlmalloc.a" for subsequent builds)
cache:INFO:  - ok
cache:INFO: generating system library: libpthread_stub.a... (this will be cached in "C:\Users\Public\Documents\GitHub\emsdk\upstream\emscripten\cache\wasm-obj\libpthread_stub.a" for subsequent builds)
cache:INFO:  - ok
cache:INFO: generating system library: libc_rt_wasm.a... (this will be cached in "C:\Users\Public\Documents\GitHub\emsdk\upstream\emscripten\cache\wasm-obj\libc_rt_wasm.a" for subsequent builds)
cache:INFO:  - ok
cache:INFO: generating system library: libsockets.a... (this will be cached in "C:\Users\Public\Documents\GitHub\emsdk\upstream\emscripten\cache\wasm-obj\libsockets.a" for subsequent builds)
cache:INFO:  - ok
cache:INFO: generating system asset: generated_struct_info.json... (this will be cached in "C:\Users\Public\Documents\GitHub\emsdk\upstream\emscripten\cache\wasm-obj\generated_struct_info.json" for subsequent builds)
cache:INFO:  - ok
[100%] Built target sample
```



