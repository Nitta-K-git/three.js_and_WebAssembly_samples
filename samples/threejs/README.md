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

- https://threejs.org/docs/index.html#examples/en/loaders/OBJLoader

- OBJファイルのテキスト文字列をそのまま渡してObject3D作ることも可能
- 生成される形式は`BufferGeometry`



### PLY

- https://github.com/mrdoob/three.js/blob/master/examples/js/loaders/PLYLoader.js

- テキストとバイナリの両方に対応
- 生成される形式は`BufferGeometry`

```javascript
var loader = new THREE.PLYLoader();
loader.load('./models/ply/ascii/dolphins.ply', function (geometry) {
	scene.add( new THREE.Mesh( geometry ) );
} );
```



# Geometry



## Geometry



## BufferGeometry











