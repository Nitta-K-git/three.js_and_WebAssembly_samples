# Sample codes of WebAssembly and three.js

# Install

## three.js

You can download the library from [github](https://github.com/mrdoob/three.js/tree/dev/examples)

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

http://nginx.org/download/nginx-1.18.0.zip をダウンロードして解凍

nginx.exeを起動して、http://localhost/ にアクセスして起動画面が出ればOK


# Get started

`C:\soft\nginx-1.18.0\html\` のパスに適当なフォルダを作って、html, js, wasmファイルを置く

ファイルパスが `C:\soft\nginx-1.18.0\html\hoge\foo.html` の場合は `http://localhost/hoge/foo.html` でアクセスできる


# WebAssembly sample codes

- template with cmake : [CODE](./samples/webasm/cmake_template)
- IDEで自動補完を使えるようにする : [CODE](./samples/webasm/cmake_with_Qt)
- C++のVectorデータをjs側に渡す : [CODE](./samples/webasm/vector_to_js)
- jsのデータをC++側に渡す : [CODE](./samples/webasm/vector_from_js)



# three.js sample codes

- template : [CODE](./samples/threejs/template.html)
- define mesh on the fly : [CODE](./samples/threejs/geometry.html)
- picking object(raycast) : [CODE](./samples/threejs/raycast.html)
- picking face : [CODE]()
- picking vertex : [CODE]()
- 















