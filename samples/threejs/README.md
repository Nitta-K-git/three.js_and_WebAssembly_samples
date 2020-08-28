# three.jsの使い方

## raycast template

```javascript
// マウス座標管理用のベクトルを作成
const mouse = new THREE.Vector2(-2,-2);
// canvas 要素の参照を取得する
const canvas = document.querySelector('#myCanvas');

// レイキャストを作成
const raycaster = new THREE.Raycaster();

canvas.addEventListener('mousemove', handleMouseMove);
// マウスを動かしたときのイベント
function handleMouseMove(event) {
    const element = event.currentTarget;
    // canvas要素上のXY座標
    const x = event.clientX - element.offsetLeft;
    const y = event.clientY - element.offsetTop;
    // canvas要素の幅・高さ
    const w = element.offsetWidth;
    const h = element.offsetHeight;

    // -1〜+1の範囲で現在のマウス座標を登録する
    mouse.x = (x / w) * 2 - 1;
    mouse.y = -(y / h) * 2 + 1;
}

function tick() {
    // レイキャスト = マウス位置からまっすぐに伸びる光線ベクトルを生成
    raycaster.setFromCamera(mouse, camera);
    // その光線とぶつかったオブジェクトを得る
    const intersects = raycaster.intersectObject(mesh);

    if (intersects.length > 0) {
        var intersect = intersects[0];
        var face = intersect.face;
        var fi = intersect.faceIndex;
        console.log(fi);

        face.color.setHex(0xff00ff);
    }

    requestAnimationFrame(tick); // セットした関数を毎フレーム呼び出す，関数内に書かないとループしない
    // アニメーション処理を書く
    renderer.render(scene, camera); // レンダリング，画面更新は明示的に書く
}

```

## ファイルの読み込み

### 3DS

- [Three.jsでモデルデータを読み込む - ICS MEDIA](https://ics.media/tutorial-three/model_basic/)
- https://github.com/mrdoob/three.js/blob/master/examples/js/loaders/TDSLoader.js#L38

```javascript
// 3DS形式のモデルデータを読み込む
const loader = new THREE.TDSLoader();
// テクスチャーのパスを指定
loader.setPath('models/3ds/portalgun/textures/');
// 3dsファイルのパスを指定
loader.load('models/3ds/portalgun/portalgun.3ds',  (object) => {
  // 読み込み後に3D空間に追加 (アロー関数でonLoad関数に登録)
  // @param {Function} onLoad onLoad callback, receives group Object3D as argument.
  scene.add(object);
});
```

### OBJ

```html
<script src="js/loaders/OBJLoader.js"></script>
```

```javascript
var reader = new FileReader();
var arraybuf;
reader.readAsText(file[0]);
reader.onload = function () {
    arraybuf = reader.result;
    console.log(arraybuf);

    geometry = load_obj(arraybuf);
    add_geometry(geometry);
}
function load_obj(arraybuf) { // 引数はテキストデータ(バイナリも可？)
    var loader = new THREE.OBJLoader();
    return loader.parse(arraybuf); // groupが返ってくる
}
```



- [source](./load_file3_threejs_obj.html)
- https://threejs.org/docs/index.html#examples/en/loaders/OBJLoader

- OBJファイルのテキスト文字列をそのまま渡してObject3D作ることも可能
- 生成される形式は`BufferGeometry`
- 頂点は各faceごとに別々に作成される
  - 6面あったら18頂点定義される。重複あっても関係ない
  - index属性は使用しない
- objectを複数読み込んでgroupにまとめる前提で作っている
  - 中身にアクセスしたい場合は`group.children[0].geometry.attributes.position`みたいに使う
  - [Group型](https://threejs.org/docs/index.html#api/en/objects/Group)
- [ライブラリソースコード](https://github.com/mrdoob/three.js/blob/master/examples/jsm/loaders/OBJLoader.js)
  - this.verticesに頂点pushしてる
  - this.normalsとかにもpushしてる
  - objectは独自の辞書型になっている https://github.com/mrdoob/three.js/blob/master/examples/jsm/loaders/OBJLoader.js#L69
    - object.geometry.verticesに頂点座標が入る
  - 頂点とかの情報をBufferGeometryに設定 https://github.com/mrdoob/three.js/blob/master/examples/jsm/loaders/OBJLoader.js#L743
  - 最終的にmeshが作られて https://github.com/mrdoob/three.js/blob/master/examples/jsm/loaders/OBJLoader.js#L865
  - containerに入れられたものが返される https://github.com/mrdoob/three.js/blob/master/examples/jsm/loaders/OBJLoader.js#L873



### PLY

```html
<script src="js/loaders/PLYLoader.js"></script>
```

```javascript
function load_obj(arraybuf) { // 引数はテキスト or バイナリ
    var loader = new THREE.PLYLoader();
    return loader.parse(arraybuf); // geometryが返ってくる
}
```

- [source](./load_file3_threejs_ply.html)
- https://github.com/mrdoob/three.js/blob/master/examples/js/loaders/PLYLoader.js

- テキストとバイナリの両方に対応
- 生成される形式は`BufferGeometry`
- OBJとは違い、Group化はされない。BufferGeometry型がそのまま返される

```javascript
var loader = new THREE.PLYLoader();
loader.load('./models/ply/ascii/dolphins.ply', function (geometry) {
	scene.add( new THREE.Mesh( geometry ) );
} );
```



## ファイル出力

```html
<script src="js/exporters/PLYExporter.js"></script>
```

```javascript
function exportASCII() {
    exporter.parse(mesh, function (result) {
        saveString(result, 'box.ply');
    });
}
function saveString(text, filename) {
    save(new Blob([text], { type: 'text/plain' }), filename);
}
function save(blob, filename) {
    link.href = URL.createObjectURL(blob);
    link.download = filename;
    link.click();
}
```



# Geometry



## Geometry



## BufferGeometry











