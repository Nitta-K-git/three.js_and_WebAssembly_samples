# Sample codes of WebAssembly and three.js



[TOC]

# Install

## three.js

You can download the library from [github](https://github.com/mrdoob/three.js/tree/dev/examples) to local (e.g. C:/Users/Public/Documents/GitHub).

## WebAssembly

### Install emsdk

- [Download and install — Emscripten 1.39.20 documentation](https://emscripten.org/docs/getting_started/downloads.html)

1. clone from github https://github.com/emscripten-core/emsdk.
2. execute install, activate and env.bat command (below command are on windows10 version, detail and another OS command is [here](https://emscripten.org/docs/getting_started/downloads.html)).

```
$ emsdk install 1.39.7
$ emsdk activate 1.39.7
$ emsdk_env.bat
```

### Install MinGW for CMAKE

- Install MinGW [EmscriptenとCMakeでのビルド方法](https://gist.github.com/faithandbrave/9b3d439d135e63abdbe7)
- set path of "C:\MinGW\bin"

### Install nginx

- [Windows10環境にNginxをインストールする - Qiita](https://qiita.com/ita3qiita/items/35f0b0d1b61c801b738d)
- [Windowsにnginxをセットアップする - Qiita](https://qiita.com/tatuno/items/44ec8130d7544d1534f7)
- [Nginxの起動、再起動、停止 | Nginx入門](https://www.adminweb.jp/nginx/install/index2.html)

Download nginx from http://nginx.org/download/nginx-1.18.0.zip , then unzip it.

Run command `start nginx.exe`

Access to http://localhost/ on web browser. You can see `Welcome to nginx!` message if nginx work.

if you want to stop nginx, run command `nginx -s stop` or `nginx -s quit`.


# Get started

## nginx setting

nginxからgithub上のhtmlファイルを直接参照できるように設定を変更する。

- [nginx コマンド超シンプル早見表 - Qiita](https://qiita.com/katzueno/items/44e02db2f1bfa0de5860)
- [nginx 公開ディレクトリの変更 - Qiita](https://qiita.com/ShinyaOkazawa/items/789db336f27f0d080152)
- [How to use nginx "root" and "location" on windows? - Stack Overflow](https://stackoverflow.com/questions/31531231/how-to-use-nginx-root-and-location-on-windows)


手順

1. `nginx-1.18.0\conf\nginx.conf`のファイルを開く

2. http → server内のlocationの項目を追加する

   ```json
   # "C:\Users\Public\Documents\GitHub\web_assembly_sample\samples\threejs\"内にあるhtmlファイルを参照したい場合
   location /threejs/ {
       root    C:\\Users\\Public\\Documents\\GitHub\\web_assembly_sample\\samples;
   }
   # "C:\Users\Public\Documents\GitHub\web_assembly_sample\samples\webasm\"内にあるhtmlファイルを参照したい場合
   location /webasm/ {
   	root    C:\\Users\\Public\\Documents\\GitHub\\web_assembly_sample\\samples;
   }
   # You have to use this location, if you use GitHub/three.js repository libraries
   location /GitHub/ {
       root    C:\\Users\\Public\\Documents;
   }
   ```

3. `localhost/threejs/index.html`のようにファイルにアクセスできる

   `/threejs`が来ると自動的に設定したrootパスが付加される仕組み

   ※ モジュールをimportする場合、locationより上のディレクトリのファイルは読めないので注意

   ※ HTML内にscriptを直接記述する場合はモジュールで親ディレクトリ読むことはできない(同じディレクトリのモジュールを呼び出して、その中で親ディレクトリからimportする)

4. `nginx.exe -s reload` で設定をリロード

# WebAssembly sample codes

## Basic samples

- template with cmake : [CODE](./samples/webasm/cmake_template)
- IDEで自動補完を使えるようにする : [CODE](./samples/webasm/cmake_with_Qt)
- C++のVectorデータをjs側に渡す : [CODE](./samples/webasm/vector_to_js)
- jsのデータをC++側に渡す : [CODE](./samples/webasm/vector_from_js)
- jsの変数を直接C++で使う(`emscripten::val`型) : [CODE](samples/webasm/cmake_meshlab_load_file)
- `<script type="module">`でimportする : [CODE]

## Use with MeshLab library

- template with MeshLab : [CODE](./samples/webasm/cmake_meshlab)
- load file in three.js and pass data to MeshLab : [CODE](samples/webasm/cmake_meshlab_load_file)
- read mesh data from MeshLab : [CODE](samples/webasm/cmake_meshlab_send_data)
- call functions of MeshLab : [CODE]

# three.js sample codes

Basic usages are [here](./samples/threejs).



---

#   Introduction

## Overview

WebAssemblyの概要

- [WebAssemblyとは - Qiita](https://qiita.com/ShuntaShirai/items/3ac92412720789576f22)
- [WebAssembly 日本での利用事例 2019年まとめ - Qiita](https://qiita.com/chikoski/items/03ebe5547d3576a95873)
- 



## Related products

### Electron

JavaScriptでデスクトップアプリケーションが作成できるツール

- [30分で出来る、JavaScript (Electron) でデスクトップアプリを作って配布するまで - Qiita](https://qiita.com/nyanchu/items/15d514d9b9f87e5c0a29)
- [JavaScript (Electron) を使ってアプリの見栄えを整える - Qiita](https://qiita.com/nyanchu/items/9a1c910bbca55e9d2f3c)
- [ようこそ！Electron入門 - Qiita](https://qiita.com/umamichi/items/6ce4f46c1458e89c4cfc)
- [Electronでアプリケーションを作ってみよう - Qiita](https://qiita.com/Quramy/items/a4be32769366cfe55778)

### wasmer, wasmtime

wasmファイルを直接実行できる仕組み。javaと同じような位置付け

様々な言語をwasmに変換できる。まだ開発中のシステム

- [Wasmer - The Universal WebAssembly Runtime](https://wasmer.io/)
- [bytecodealliance/wasmtime: Standalone JIT-style runtime for WebAssembly, using Cranelift](https://github.com/bytecodealliance/wasmtime)
- [Emscriptenのstandaloneモードについて - Qiita](https://qiita.com/chikoski/items/29b6582eee01c8cba5b8)

## Other links

- [WebAssembly でも feature detection したい | Zenn](https://zenn.dev/chikoski/articles/c5f2d66d40360df2dec1)
- [夏休みだョ！WebAssembly Proposal全員集合！！ | Kabuku Developers Blog](https://www.kabuku.co.jp/developers/wasm-proposal)
- [今年 WebAssembly でつくった3つのアプリ | hiromasa.another :o)](https://another.maple4ever.net/archives/2770/)
- [WebAssemblyでもブラウザでステップ実行ができるようになってたので寄り道しながら頑張った話 - Qiita](https://qiita.com/lempiji/items/462cce79dab8166fa5a5)
- [Go言語でWeb Assemblyに入門した - tomato3713’s blog](https://tomato3713.hatenablog.com/entry/2019/12/15/113000)
- [Asyncify を使ってみた - Qiita](https://qiita.com/chikoski/items/ed78aace5452a94a49d5)
- [CLion で Emscripten をやっていきたい - Qiita](https://qiita.com/tan-y/items/d5539ddd8ce10972919a)
- [Webassemblyでもstd::optionalが使いたいのでemscripten binding書きます - Qiita](https://qiita.com/yumetodo/items/32eb162faa78f36009a6)

# Get Started

## tutorial (C language only) : not recommended

- [C/C++からWebAssemblyにコンパイルする - WebAssembly | MDN](https://developer.mozilla.org/ja/docs/WebAssembly/C_to_wasm)
- [WebAssemblyでC++とJavaScript間のやり取り - Qiita](https://qiita.com/soramimi_jp/items/1b7ed0ddcefb0f4a7172)

このやり方では、

- c言語を使っている
- html、wasm、jsファイルを出力してセットで使っている
- emccコマンドで直接cファイルをコンパイル

C++が使えない、htmlもオリジナルのものに組み込めない、emccコマンド直接たたくので、cmakeなどで他のプロジェクトと連携して使えないという欠点がある。

## tutorial2 (C++)

Emscriptenの中にEmbindというライブラリがある。これを用いるとC++との連携が簡単にできる。

通常のjsファイルと同じように`<script>`タグで読み込めるので、任意のhtmlファイルで使える。

- [Emscriptenを使ったC/C++の関数のエキスポート方法まとめ - Qiita](https://qiita.com/chikoski/items/462b34db61daf13a7897)
- [Embind — Emscripten 2.0.4 documentation](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#embind)

## tutorial3 (cmake)

- [EmscriptenとCMakeでのビルド方法](https://gist.github.com/faithandbrave/9b3d439d135e63abdbe7)

上記サイトで使ってる環境はやや古いが、基本的な流れは同じ。

オプションの設定でembind使う設定に変えたりするとよい。

## tutorial4 (recommended)

embindとC++を使ってcmake上でビルドする。

`CMakeList.txt`

```cmake
cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)

set(CMAKE_CXX_STANDARD 11)

file(TO_CMAKE_PATH $ENV{EMSDK_DIR} EMSDK_DIR) # githubからcloneしたemsdkのヘッダファイルパスを指定

# set output file directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Add Executable
add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${EMSDK_DIR}
)

target_sources(${PROJECT_NAME}
  PRIVATE
    main.cpp
)

if (EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "--bind") # bindオプションを追加
endif()
```



`main.cpp`

```cpp
#include <iostream>
#include <emscripten/bind.h>
#include <emscripten/val.h>

using namespace emscripten;
using namespace std;

int main()
{
    cout << "Hello World" << endl;
}

void hello(){
    cout << "hello" << endl;
}

int add(int a, int b){
    return a+b;
}

EMSCRIPTEN_BINDINGS(module) {
	emscripten::function("hello", &hello);
	emscripten::function("add", &add);
}
```



command

```shell
$ C:\Users\Public\Documents\GitHub\emsdk\emsdk_env.bat
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

$ emcmake cmake .. -G "MinGW Makefiles"
-- Configuring done
-- Generating done
-- Build files have been written to: C:/Users/Public/Documents/GitHub/web_assembly_sample/samples/webasm/tutorial/build

$ emmake make
make: ['C:\\MinGW\\bin\\mingw32-make.exe']
Scanning dependencies of target tutorial
[ 50%] Building CXX object CMakeFiles/tutorial.dir/main.cpp.o
[100%] Linking CXX executable ..\tutorial.js
[100%] Built target tutorial
```



html

```html
<!doctype html>
<html>
<script>
  var Module = {
    onRuntimeInitialized: function () {
      Module.hello();
      console.log("add : " + Module.add(1, 2));
    }
  };
</script>
<script src="tutorial.js"></script>

</html>
```

html (別パターン)

```html
<!doctype html>
<html>
<script>
  Module = {
    onRuntimeInitialized: function () {
      init();
    }
  };
  
  function init() {
    Module.hello();
    console.log("add : " + Module.add(1, 2));
  }
</script>
<script src="tutorial.js"></script>

</html>
```



result : access to `http://localhost/GitHub/web_assembly_sample/samples/webasm/tutorial/index.html`

console outputs

```shell
hello
add : 3
Hello World
```



## tutorial5 (others)

- [Qtを使って、デスクトップアプリ開発とWebAssemblyアプリ開発を同時に学ぼう - Qiita](https://qiita.com/soramimi_jp/items/0cb28436f67bb42690ee)
- [Emscripten で C/C++ から JS の関数を呼ぶには - Qiita](https://qiita.com/chikoski/items/9ac019a86095cfcf2c73)



# Extra

- [Embind — Emscripten 2.0.4 documentation](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#embind)
  - C++のオブジェクトはJS側で明示的に削除すること(GCは働かない)

モジュールとして読み込む場合





