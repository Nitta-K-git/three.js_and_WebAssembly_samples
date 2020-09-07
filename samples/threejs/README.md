# three.jsの使い方

- [Three.js入門サイト - ICS MEDIA](https://ics.media/tutorial-three/)



[TOC]

---

# template

[source](template.html)

```html
<!DOCTYPE html>
<html>

<head>
    <meta charset="utf-8" />
    <!-- JSライブラリ読み込み，CDNを使用 -->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/105/three.min.js"></script>
    <script>
        // ページの読み込みを待つ(loadイベントが完了した時点で呼び出す関数を指定)
        window.addEventListener('load', init);

        function init() {
            // サイズを指定
            const width = 960;
            const height = 540;

            // レンダラーを作成
            const renderer = new THREE.WebGLRenderer({
                canvas: document.querySelector('#myCanvas') // 予め作っておいたcanvas要素を連携させる
            });
            renderer.setPixelRatio(window.devicePixelRatio);
            renderer.setSize(width, height); // canvas要素の大きさ自体が変わる

            // シーンを作成 : 3D空間のこと，3Dオブジェクトや光源を置く場所
            const scene = new THREE.Scene();

            // カメラを作成
            const camera = new THREE.PerspectiveCamera(45, width / height);
            // カメラの初期座標を設定
            camera.position.set(0, 0, +1000);

            // geometry作成
            const geometry = new THREE.BoxGeometry(400, 400, 800); // 形状を表す
            const material = new THREE.MeshBasicMaterial({ // 材質を表す, 適当なカラーを割り振るマテリアル
                color: 0x6699FF
            });
            const mesh = new THREE.Mesh(geometry, material); // Mesh型が表示オブジェクト，形状＋材質
            scene.add(mesh); // 空間に追加

            // 初回実行 → 一度実行されると後はループ設定が有効になる
            tick();

            // 毎フレーム時に実行されるループイベントです
            function tick() {
                requestAnimationFrame(tick); // セットした関数を毎フレーム呼び出す，関数内に書かないとループしない
                // アニメーション処理を書く
                renderer.render(scene, camera); // レンダリング，画面更新は明示的に書く
            }
        }
    </script>
</head>

<body>
    <!-- 描画エリアを用意 -->
    <canvas id="myCanvas"></canvas>
</body>

</html>
```

# Orbit camera

[source](orbit_camera.html)

html単体をchrome上で実行する場合は[参考元](https://github.com/ics-creative/tutorial-three/blob/master/samples/camera_orbitcontrols_basic.html)のサンプルでもエラーは出ないが、nginx上で読み込むと以下のエラーが出るため注意。

`OrbitControls.js:1117 Uncaught TypeError: Cannot read property 'addEventListener' of undefined`

対策方法

- [javascript - Three.js + OrbitControls - Uncaught TypeError: Cannot read property 'addEventListener' of undefined - Stack Overflow](https://stackoverflow.com/questions/63272315/three-js-orbitcontrols-uncaught-typeerror-cannot-read-property-addeventlis)
- [OrbitControls – three.js docs](https://threejs.org/docs/index.html#examples/en/controls/OrbitControls)

```html
<html>

<head>
    <meta charset="utf-8" />
    <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/110/three.min.js"></script>
    <script src="../js/controls/OrbitControls.js"></script>
    <script>
        // ページの読み込みを待つ
        window.addEventListener('load', init);

        function init() {
            // サイズを指定
            const width = 960;
            const height = 540;

            // レンダラーを作成
            const renderer = new THREE.WebGLRenderer({
                canvas: document.querySelector('#myCanvas')
            });
            renderer.setSize(width, height);
            document.body.appendChild(renderer.domElement);

            // シーンを作成
            const scene = new THREE.Scene();

            // カメラを作成
            const camera = new THREE.PerspectiveCamera(45, width / height);
            // カメラの初期座標を設定
            camera.position.set(0, 0, 1000);

            // カメラコントローラーを作成
            const controls = new THREE.OrbitControls(camera, renderer.domElement);

            // 形状とマテリアルからメッシュを作成します
            const mesh = new THREE.Mesh(
                new THREE.BoxGeometry(300, 300, 300),
                new THREE.MeshNormalMaterial()
            );
            scene.add(mesh);

            tick();

            // 毎フレーム時に実行されるループイベントです
            function tick() {
                // レンダリング
                renderer.render(scene, camera);
                requestAnimationFrame(tick);
            }
        }
    </script>
</head>

<body>
    <canvas id="myCanvas"></canvas>
</body>

</html>
```

# raycast (objects)

## basic

複数のオブジェクトからカーソルで触れたものを抽出する。オブジェクト単位での取得

- [Raycaster – three.js docs](https://threejs.org/docs/index.html#api/en/core/Raycaster)
- [Three.jsでオブジェクトとの交差を調べる - ICS MEDIA](https://ics.media/tutorial-three/raycast/)

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

## ヒットしたオブジェクトの詳細を表示

[source](raycast.html)

- [HTML要素の中身を変えるinnerHTML | JavaScript入門編 - ウェブプログラミングポータル](https://wp-p.info/tpl_rep.php?cat=js-biginner&fl=r13)
- [文字を表示させてみよう | HTML5入門編 - ウェブプログラミングポータル](https://wp-p.info/tpl_rep.php?cat=html-biginner&fl=r10)

```html
<html>

<head>
  <meta charset="utf-8" />
  <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/110/three.min.js"></script>
  <script>
    // ページの読み込みを待つ
    window.addEventListener('load', init);

    function formatByArr(msg) { var args = []; for (var i = 1; i < arguments.length; i++) { args[i - 1] = arguments[i]; } return msg.replace(/\{(\d+)\}/g, function (m, k) { return args[k]; }); };

    function init() {
      // サイズを指定
      const width = 960;
      const height = 540;

      // マウス座標管理用のベクトルを作成
      const mouse = new THREE.Vector2();

      // canvas 要素の参照を取得する
      const canvas = document.querySelector('#myCanvas');
      // レンダラーを作成
      const renderer = new THREE.WebGLRenderer({
        canvas: canvas
      });
      renderer.setPixelRatio(window.devicePixelRatio);
      renderer.setSize(width, height);

      // シーンを作成
      const scene = new THREE.Scene();

      // カメラを作成
      const camera = new THREE.PerspectiveCamera(45, width / height);
      camera.position.set(0, 0, +1000);

      const geometry = new THREE.BoxBufferGeometry(50, 50, 50);

      // マウスとの交差を調べたいものは配列に格納する
      const meshList = [];
      for (let i = 0; i < 200; i++) {
        const material = new THREE.MeshStandardMaterial({ color: 0xffffff });

        const mesh = new THREE.Mesh(geometry, material);
        mesh.position.x = (Math.random() - 0.5) * 800;
        mesh.position.y = (Math.random() - 0.5) * 800;
        mesh.position.z = (Math.random() - 0.5) * 800;
        mesh.rotation.x = Math.random() * 2 * Math.PI;
        mesh.rotation.y = Math.random() * 2 * Math.PI;
        mesh.rotation.z = Math.random() * 2 * Math.PI;
        scene.add(mesh);

        // 配列に保存
        meshList.push(mesh);
      }

      // 平行光源
      const directionalLight = new THREE.DirectionalLight(0xffffff);
      directionalLight.position.set(1, 1, 1);
      scene.add(directionalLight);

      // 環境光源
      const ambientLight = new THREE.AmbientLight(0x333333);
      scene.add(ambientLight);

      // レイキャストを作成
      const raycaster = new THREE.Raycaster();

      canvas.addEventListener('mousemove', handleMouseMove);
      tick();

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

      // 毎フレーム時に実行されるループイベントです
      function tick() {
        // レイキャスト = マウス位置からまっすぐに伸びる光線ベクトルを生成
        raycaster.setFromCamera(mouse, camera);

        // その光線とぶつかったオブジェクトを得る
        const intersects = raycaster.intersectObjects(meshList);

        meshList.map(mesh => {
          // 交差しているオブジェクトが1つ以上存在し、
          // 交差しているオブジェクトの1番目(最前面)のものだったら
          if (intersects.length > 0 && mesh === intersects[0].object) {
            let m = intersects[0];
            // 色を赤くする
            mesh.material.color.setHex(0xff0000);
            let elem = document.getElementById("hit_obj_id");
            elem.innerHTML = "オブジェクトID : " + String(m.object.id);
            document.getElementById("hit_face_index").innerHTML = "face index : " + String(m.faceIndex);
            document.getElementById("hit_pos").innerHTML = formatByArr("pos ({0},{1},{2})", m.point.x.toFixed(2), m.point.y.toFixed(2), m.point.z.toFixed(2));
          } else {
            // それ以外は元の色にする
            mesh.material.color.setHex(0xffffff);
          }
        });

        // レンダリング
        renderer.render(scene, camera);
        requestAnimationFrame(tick);
      }
    }
  </script>
</head>

<body>
  <canvas id="myCanvas"></canvas>
  <p id="hit_obj_id">オブジェクトID : </p>
  <p id="hit_face_index">face index : </p>
  <p id="hit_pos">pos ()</p>
</body>

</html>
```

## ヒットした面の色を変える

BufferGeometry型ならnon indexタイプで3つの頂点をすべて色変更する

一括で色をリセットする機能はないようなので、色を戻したい場合は、色を変える度に前の選択の色だけ元に戻すアプローチが現実的

[source](buffergeometry_raycast_sel_face.html)

```javascript
const intersects = raycaster.intersectObject(mesh);
if (intersects.length > 0) {
    var intersect = intersects[0];
    var face = intersect.face;
    var fi = intersect.faceIndex;
    console.log(fi);

    for(let i=0; i<3; ++i){
        geometry.attributes.color.setXYZ(fi*3+i,1,1,1);
    }
    // geometry.faces[fi].color.set(0xffffff); // Geometry型の場合
    geometry.attributes.color.needsUpdate = true;
}
```

Geometry型なら面単位で選択が可能

- [javascript - How to change face color in Three.js - Stack Overflow](https://stackoverflow.com/questions/11252592/how-to-change-face-color-in-three-js/44767066)
- [three.js マウスクリックで一部の面の色を変える - Qiita](https://qiita.com/Arihi/items/62c612450f0219bf8225)



# raycast (object)

[source](raycast_object.html)

ひとつのオブジェクトの中でカーソルがヒットした面を取得する

使い方は渡すのが配列から単体のオブジェクトに変わったくらいで、`intersectObjects`の方とほとんど変わらない。戻り値も同じ形式

```javascript
const intersects = raycaster.intersectObject(mesh);
```



# ファイルの読み込み

## テキストファイル

- [JavaScriptプログラミング講座【ダイアログを表示する】](https://hakuhin.jp/js/dialog.html)
- [JavaScriptでFile APIを用いたファイル操作方法ついて解説 | ELOOP（イーループ）](https://www.e-loop.jp/knowledges/35/)

```html
<meta charset="utf-8" />
<html>

<body>
    <form>
        <input type="file" id="file" multiple> <br>
        <p id="text_result"></p>
    </form>
    <script type="text/javascript">
        var input_file = document.getElementById("file");
        input_file.onchange = function (e) {
            if (input_file.value) {
                console.log("ファイルが選択された:" + input_file.value);
            } else {
                console.log("ファイルが未選択");
                return;
            }

            // 読み込んだファイルの情報をすべて表示
            console.log(e.target.files);

            // テキストとして読み込んでログに表示
            var file = e.target.files;
            var reader = new FileReader();

            reader.readAsText(file[0]);
            reader.onload = function () {
                console.log(reader.result);
                // document.getElementById("text_result").innerHTML = reader.result; // そのまま代入すると改行コードがスペースになる
            }

            // FileList オブジェクトを取得する
            var file_list = input_file.files;
            if (!file_list) return;

            var i;
            var num = file_list.length;
            for (i = 0; i < num; i++) {
                // File オブジェクトを取得する
                var file = file_list[i];

                // 出力テスト
                console.log("id:" + i);
                console.log("name:" + file.name);
                console.log("size:" + file.size);
                console.log("type:" + file.type);
            }
        };
    </script>
</body>

</html>
```

## オブジェクトの削除

削除するときはgeometryやmaterialも明示的に消す

- [three.js の Mesh を削除する - Qiita](https://qiita.com/yomotsu/items/7be847bf2fd08335e8b1)
- [javascript - three.js remove specific object from scene - Stack Overflow](https://stackoverflow.com/questions/39322054/three-js-remove-specific-object-from-scene/39322121)

```javascript
scene.remove( mesh );
geometry.dispose();
material.dispose();
texture.dispose();
```

## 3DS

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

## OBJ

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

- [source](load_file_obj.html)
- https://threejs.org/docs/index.html#examples/en/loaders/OBJLoader

- OBJファイルのテキスト文字列をそのまま渡してObject3D作ることも可能
- 生成される形式は`BufferGeometry`
- 頂点は各faceごとに別々に作成される
  - 6面あったら18頂点定義される。重複あっても関係ない
  - index属性は使用しない
- objectを複数読み込んでgroupにまとめる前提で作っている
  - 中身にアクセスしたい場合は`group.children[0].geometry.attributes.position`みたいに使う
  - [Group型](https://threejs.org/docs/index.html#api/en/objects/Group)
  - materialの設定方法も要注意。[three.js TypeError: Cannot read property 'center' of undefined - Stack Overflow](https://stackoverflow.com/questions/32039553/three-js-typeerror-cannot-read-property-center-of-undefined)
- [ライブラリソースコード](https://github.com/mrdoob/three.js/blob/master/examples/jsm/loaders/OBJLoader.js)
  - this.verticesに頂点pushしてる
  - this.normalsとかにもpushしてる
  - objectは独自の辞書型になっている https://github.com/mrdoob/three.js/blob/master/examples/jsm/loaders/OBJLoader.js#L69
    - object.geometry.verticesに頂点座標が入る
  - 頂点とかの情報をBufferGeometryに設定 https://github.com/mrdoob/three.js/blob/master/examples/jsm/loaders/OBJLoader.js#L743
  - 最終的にmeshが作られて https://github.com/mrdoob/three.js/blob/master/examples/jsm/loaders/OBJLoader.js#L865
  - containerに入れられたものが返される https://github.com/mrdoob/three.js/blob/master/examples/jsm/loaders/OBJLoader.js#L873



## PLY

- [source](./load_file_ply.html)
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

# Material

## texture



# Geometry

## Create Geometry data on the fly

[source](geometry.html)

頂点座標と面のインデックスを定義

```javascript
// geometry
var material = new THREE.MeshStandardMaterial({
    color: 0x00cc00
});

//create a triangular geometry
var geometry = new THREE.Geometry();
geometry.vertices.push(new THREE.Vector3(-50, -50, 0));
geometry.vertices.push(new THREE.Vector3(50, -50, 0));
geometry.vertices.push(new THREE.Vector3(50, 50, 0));
geometry.vertices.push(new THREE.Vector3(-50, 50, 0));

//create a new face using vertices 0, 1, 2
var color = new THREE.Color(0xffaa00); //optional
var face = new THREE.Face3(0, 1, 2, color=color);

//add the face to the geometry's faces array
geometry.faces.push(face);
geometry.faces.push(new THREE.Face3(1, 2, 3, color=new THREE.Color(0x0000ff)));

//the face normals and vertex normals can be calculated automatically if not supplied above
geometry.computeFaceNormals();
geometry.computeVertexNormals();

var mesh = new THREE.Mesh(geometry, material);
scene.add(mesh);
```

### 色情報の更新

[source](geometry_update_col.html)

```javascript
geometry = new THREE.Geometry();
geometry.vertices.push(
    new THREE.Vector3(-1, -1, 1),
    new THREE.Vector3(1, -1, 1),
    new THREE.Vector3(1, 1, 1),
    new THREE.Vector3(-1, 1, 1),
);
geometry.faces.push(
    new THREE.Face3(0, 1, 2, color=new THREE.Color(0xff0000)),
    new THREE.Face3(2, 3, 0, color=new THREE.Color(0x00ff00)),
);
geometry.faces[1].color.setRGB(1,0,0);

var material = new THREE.MeshBasicMaterial({
    vertexColors: true,
    // vertexColors: THREE.FaceColors,
});
var mesh = new THREE.Mesh(geometry, material);
scene.add(mesh);
```

```javascript
function set_col() {
    geometry.faces[0].color.setRGB(1,1,0);
    geometry.colorsNeedUpdate = true;
}
```





## Create BufferGeometry data on the fly

- https://threejs.org/docs/#api/en/core/BufferGeometry

- 面を定義するのに頂点インデックスを使うこともできるし、頂点だけで直接定義することもできる
- `geometry.setAttribute('color', new THREE.Float32BufferAttribute(colors, 3));`
  - 渡す配列はコピー渡し
  - リスト型でも`Float32Array`型とかでも渡せる
- `geometry.setAttribute('color', new THREE.BufferAttribute(colors, 3));`
  - 渡す配列は参照渡し
  - `Float32Array`とかの型付き配列でないと渡せない

### 頂点だけで面を指定(STL方式)

[source](buffergeometry_vert_only.html)

頂点だけで定義する場合は、重複点も毎回記述する。STLと同じ方式

色は面ごとに区別して指定できる

```javascript
var geometry = new THREE.BufferGeometry();
// create a simple square shape. We duplicate the top left and bottom right
// vertices because each vertex needs to appear once per triangle.
var vertices = new Float32Array([
    -1.0, -1.0, 1.0,
    1.0, -1.0, 1.0,
    1.0, 1.0, 1.0,

    1.0, 1.0, 1.0,
    -1.0, 1.0, 1.0,
    -1.0, -1.0, 1.0
]);

var colors = [];
colors.push(
    1, 0, 0,
    1, 1, 0,
    0, 1, 0,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
);
geometry.setAttribute('color', new THREE.Float32BufferAttribute(colors, 3));

// itemSize = 3 because there are 3 values (components) per vertex
// 値を更新する必要があるときはBufferAttributeオブジェクトのneedsUpdateフラグを立てる(https://threejs.org/docs/#api/en/core/BufferAttribute)
geometry.setAttribute('position', new THREE.BufferAttribute(vertices, 3));

var material = new THREE.MeshBasicMaterial({
    // color: 0xff0000, 
    vertexColors: true, // 頂点に対して色を設定する場合
});
var mesh = new THREE.Mesh(geometry, material);
scene.add(mesh);
```

### 面を頂点インデックスで指定(PLY方式)

[source](buffergeometry_face_index.html)

面を頂点インデックスで指定する。頂点は複数の面で共有できる

色は頂点にしか設定できないため、面ごとに違う色を付けるのは不可

- [three.js - THREE.BufferGeometry - How do I manually set face colors? - Stack Overflow](https://stackoverflow.com/questions/41670308/three-buffergeometry-how-do-i-manually-set-face-colors)
  - `geometry2 = geometry.toNonIndexed()` でインデックスを使わない形式に変換できる

```javascript
var geometry = new THREE.BufferGeometry();
var vertices = new Float32Array([
    -1.0, -1.0, 1.0,
    1.0, -1.0, 1.0,
    1.0, 1.0, 1.0,
    -1.0, 1.0, 1.0,
]);

var indices = [];
indices.push(
    0, 1, 2,
    2, 3, 0,
);
// Arrayを使う方法だとうまくいかない
//  var indices = new Uint32Array([
//      0,1,2,
//      1,2,3,
//  ]);

var colors = [];
colors.push(
    1, 0, 0,
    1, 1, 0,
    0, 1, 0,
    0, 0, 1,
);
geometry.setAttribute('color', new THREE.Float32BufferAttribute(colors, 3));

// itemSize = 3 because there are 3 values (components) per vertex
geometry.setAttribute('position', new THREE.BufferAttribute(vertices, 3));
geometry.setIndex(indices);

geometry2 = geometry.toNonIndexed() // 非index型に変換できる

var material = new THREE.MeshBasicMaterial({
    //color: 0xffff00, 
    vertexColors: true, // 頂点に対して色を設定する場合
});
var mesh = new THREE.Mesh(geometry, material);
scene.add(mesh);
```

### 頂点座標の更新

[source](buffergeometry_face_index_update_pos.html)

- https://threejs.org/docs/#api/en/core/BufferAttribute.needsUpdate
- [three.js - How to update the Geometry vertex position Objloader - Stack Overflow](https://stackoverflow.com/questions/31859819/how-to-update-the-geometry-vertex-position-objloader)
- [Three.js: How to Update BufferGeometry Vertices - Stack Overflow](https://stackoverflow.com/questions/20303239/three-js-how-to-update-buffergeometry-vertices)

値を更新する必要があるときはBufferAttributeオブジェクトのneedsUpdateフラグを立てる

```javascript
// geometryに渡した配列 or geometryの属性を直接編集する → update
function move_pos() {
    vertices[0] += 0.1;
    geometry.attributes.position.array[1] += 0.05;
    geometry.attributes.position.needsUpdate = true;
}
```

### 色情報の更新

[source](buffergeometry_vert_only_update_col.html)

```javascript
colors = new Float32Array([
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
]);
geometry.setAttribute('color', new THREE.BufferAttribute(colors, 3));
```

```javascript
function set_col() {
    colors[0] = 1;
    geometry.attributes.color.array[3] = 1;
    geometry.attributes.color.setX(2, 1);
    geometry.attributes.color.needsUpdate = true;
}
```



## group

```javascript
var cubeA = new THREE.Mesh(geometry, material);
cubeA.position.set(1, 1, 0);
var cubeB = new THREE.Mesh(geometry, material);
cubeB.position.set(-1, -1, 0);

var group = new THREE.Group();
group.add(cubeA);
group.add(cubeB);
```



# GUI

## dat



# HTML, JavaScript Tips

## 文字列のフォーマット指定して出力

- [文字列のフォーマット（String.Format的な処理） - JavaScript｜OAR CODE](https://code.st40.xyz/article/341)

```javascript
// フォーマット（引数可変（配列）版）
function formatByArr(msg) {
    var args = [];
    for (var i = 1; i < arguments.length; i++) {
        args[i - 1] = arguments[i];
    }
    return msg.replace(/\{(\d+)\}/g, function (m, k) {
        return args[k];
    });
};
    
// フォーマット（オブジェクト版）
function formatByObj(msg, obj) {
    return msg.replace(/\{(\w+)\}/g, function (m, k) {
        return obj[k];
    });
};

// 動作確認（引数可変版）
function testArr(){
    var msg = formatByArr("ID：{0}、TITLE：{1}", 12345, "タイトル");
    console.log(msg);   // ID：12345、TITLE：タイトル
}

// 動作確認（オブジェクト版）
function testObj(){
    var msg = formatByObj("ID：{id}、TITLE：{title}", { id: 12345, title: "タイトル" });
    console.log(msg);   // ID：12345、TITLE：タイトル
}
```

## 数値を文字列にフォーマット指定して変換

- [数値（Number） - とほほのWWW入門](http://www.tohoho-web.com/js/number.htm)

```javascript
var xx = 1.234e5;
console.log(xx.toFixed());     // => 123400
console.log(xx.toFixed(2));    // => 123400.00

xx = 123456789;
console.log(xx.toPrecision());     // => 123456789
console.log(xx.toPrecision(3));    // => 1.23e+8
xx = 1.23456789;
console.log(xx.toPrecision(3));    // => 1.23
xx = 0.0000123456789;
console.log(xx.toPrecision(3));    // => 0.0000123
xx = 1.23456789e30;
console.log(xx.toPrecision(3));    // => 1.23e+30

xx = 65535;
console.log(xx.toString(16));	// ffff
```

## 選択できるファイル拡張子の制限

- [input type=”file”で選択できるファイルの拡張子を制限する方法 │ Web備忘録](https://webbibouroku.com/Blog/Article/html5-file-accept)

```html
<!-- 拡張子 .pdf  -->
<input type="file" accept=".pdf">

<!-- 複数指定(.xlsxもしくは.docx) -->
<input type="file" accept=".xlsx,.docx">
```

## ボタン

[source](button.html)

```html
<body>
    <input type="button" value="ボタン" onclick="button1()">
</body>
<script>
    function button1(){
        console.log("pushed!!!");
    }
</script>
```



