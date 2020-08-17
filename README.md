# WebAssemblyのサンプルコード

動作環境はwindows10, 64bit

# Install

## Install emsdk

- [Download and install — Emscripten 1.39.20 documentation](https://emscripten.org/docs/getting_started/downloads.html)

1. https://github.com/emscripten-core/emsdk をcloneする
2. install, activate, PATHの設定をコマンドプロンプトで行う

```
$ emsdk install 1.39.7
$ emsdk activate 1.39.7
$ emsdk_env.bat
```

## Install MinGW for CMAKE

[EmscriptenとCMakeでのビルド方法](https://gist.github.com/faithandbrave/9b3d439d135e63abdbe7)の通りにMinGWをインストール、"C:\MinGW\bin" のパスを通す

## Install nginx

- [Windows10環境にNginxをインストールする - Qiita](https://qiita.com/ita3qiita/items/35f0b0d1b61c801b738d)
- [Windowsにnginxをセットアップする - Qiita](https://qiita.com/tatuno/items/44ec8130d7544d1534f7)

http://nginx.org/download/nginx-1.18.0.zip をダウンロードして解凍

nginx.exeを起動して、http://localhost/ にアクセスして起動画面が出ればOK


# 動作確認方法

`C:\soft\nginx-1.18.0\html\` のパスに適当なフォルダを作って、html, js, wasmファイルを置く

ファイルパスが `C:\soft\nginx-1.18.0\html\hoge\foo.html` の場合は `http://localhost/hoge/foo.html` でアクセスできる


# Samples

- cmakeのテンプレート : [CODE](./samples/cmake_template)
- IDEで自動補完を使えるようにする : [CODE](./samples/cmake_with_Qt)
- C++のVectorデータをjs側に渡す : [CODE](./samples/vector_to_js)
- jsのデータをC++側に渡す : [CODE](./samples/vector_from_js)
- 















