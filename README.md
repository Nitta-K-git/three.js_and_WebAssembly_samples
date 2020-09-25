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

















