# three.jsの使い方

- [Three.js入門サイト - ICS MEDIA](https://ics.media/tutorial-three/)



[TOC]

---

# template

## ひな形

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

## 複数のオブジェクトをランダムで作って配置する

```javascript
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

```

## Raycast 複数オブジェクト

```javascript
function tick() {
    raycaster.setFromCamera(mouse, camera);
    const intersects = raycaster.intersectObjects(meshList);

    meshList.map(mesh => {
        // 交差しているオブジェクトが1つ以上存在し、
        // 交差しているオブジェクトの1番目(最前面)のものだったら
        if (intersects.length > 0 && mesh === intersects[0].object) {
            let m = intersects[0];
            mesh.material.color.setHex(0xff0000); // 色を赤くする
            let elem = document.getElementById("hit_obj_id");
            elem.innerHTML = "オブジェクトID : " + String(m.object.id);
            document.getElementById("hit_face_index").innerHTML = "face index : " + String(m.faceIndex);
            document.getElementById("hit_pos").innerHTML = 
                formatByArr("pos ({0},{1},{2})", m.point.x.toFixed(2), m.point.y.toFixed(2), m.point.z.toFixed(2));
        } else {
            mesh.material.color.setHex(0xffffff); // それ以外は元の色にする
        }
    });
    renderer.render(scene, camera);
    requestAnimationFrame(tick);
}

```





## Raycast : クリックして選択

```javascript
function init() {
    // ~~~

    // マウス座標管理用のベクトルを作成
    const mouse = new THREE.Vector2();
    // レイキャストを作成
    const raycaster = new THREE.Raycaster();

    canvas.addEventListener('mousedown', handleMouseDown, false);
    function handleMouseDown(event) {
        if (event.button == 0) { // click left button
            const element = event.currentTarget;
            mouse.x = ((event.clientX - element.offsetLeft) / element.offsetWidth) * 2 - 1;
            mouse.y = -((event.clientY - element.offsetTop) / element.offsetHeight) * 2 + 1;

            raycaster.setFromCamera(mouse, camera);
            const intersects = raycaster.intersectObject(mesh);
            if (intersects.length > 0) {
                var intersect = intersects[0]; // 最前面の公差したオブジェクト取り出し
                var face = intersect.face;
                var fi = intersect.faceIndex;
            }
        }
    }
}
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

# raycast 

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

[source](raycast_objects.html)

- [HTML要素の中身を変えるinnerHTML | JavaScript入門編 - ウェブプログラミングポータル](https://wp-p.info/tpl_rep.php?cat=js-biginner&fl=r13)
- [文字を表示させてみよう | HTML5入門編 - ウェブプログラミングポータル](https://wp-p.info/tpl_rep.php?cat=html-biginner&fl=r10)

```html
<html>

<head>
  <meta charset="utf-8" />
  <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/r119/three.min.js"></script>
  <script>
    // ページの読み込みを待つ
    window.addEventListener('load', init);

    function formatByArr(msg) { var args = []; for (var i = 1; i < arguments.length; i++) { args[i - 1] = arguments[i]; } return msg.replace(/\{(\d+)\}/g, function (m, k) { return args[k]; }); };

    function init() {
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

## 面の選択

[source (geometry version)](select_face_geometry.html)

- [raycasterでクリックイベントの実現 - Qiita](https://qiita.com/mo49/items/c8ca223fb90a8053f902)

マウスのボタンダウンでraycastによる判定を行う

BufferGeometryだとPLY方式のデータで面単位で色を付ける機能がないため、実装してない

```javascript
canvas.addEventListener('mousedown', handleMouseDown, false);
function handleMouseDown(event) {
    if (event.button == 0) { // click left button
        const element = event.currentTarget;
        mouse.x = ((event.clientX - element.offsetLeft) / element.offsetWidth) * 2 - 1;
        mouse.y = -((event.clientY - element.offsetTop) / element.offsetHeight) * 2 + 1;

        raycaster.setFromCamera(mouse, camera);
        const intersects = raycaster.intersectObject(mesh);
        if (intersects.length > 0) {
            var intersect = intersects[0];
            var face = intersect.face;
            var fi = intersect.faceIndex;
            console.log(fi);
            if (select_face_id != fi) {
                if (select_face_id >= 0) {
                    geometry.faces[select_face_id].color.setRGB(1, 1, 1);
                }
                geometry.faces[fi].color.setRGB(1, 0, 0);
                geometry.colorsNeedUpdate = true;
                select_face_id = fi;
            }
        }
    }
}
```



## 頂点の選択

[source (Geometry version)](select_vert_geometry.html)

[source (BufferGeometry version)](select_vert_bufgeometry.html)

頂点のみ選択をしたい場合は、raycastに点群だけのデータ(`THEREE.Points`)を作って渡す

点群も選択された頂点のインデックスとオブジェクトが取得できるので、面と同じように扱うことができる

GeometryでもBufferGeometryでも同じように処理できる

```javascript
// BufferGeometry
geometry = new THREE.BufferGeometry();
var vertices = new Float32Array([
    -1, -1, +1,
    +1, -1, +1,
    +1, +1, +1,
    -1, +1, +1,
]);
var colors = new Float32Array([
    0, 1, 1,
    0, 1, 1,
    0, 1, 1,
    0, 1, 1,
]);
geometry.setAttribute('position', new THREE.BufferAttribute(vertices, 3));
geometry.setAttribute('color', new THREE.BufferAttribute(colors, 3));
geometry.computeBoundingBox();

var material = new THREE.PointsMaterial({
    size: 0.10, // 点群のサイズ設定
    vertexColors: true,
});

pcd = new THREE.Points(geometry, material);
scene.add(pcd);

// レイキャストを作成
const raycaster = new THREE.Raycaster();
raycaster.params.Points.threshold = 0.1; // 点の当たり判定閾値(def:1)

canvas.addEventListener('mousedown', handleMouseDown, false);

function handleMouseDown(event) {
    if (event.button == 0) { // click left button
        const element = event.currentTarget;
        mouse.x = ((event.clientX - element.offsetLeft) / element.offsetWidth) * 2 - 1;
        mouse.y = -((event.clientY - element.offsetTop) / element.offsetHeight) * 2 + 1;

        raycaster.setFromCamera(mouse, camera);
        const intersects = raycaster.intersectObject(pcd);
        if (intersects.length > 0) {
            var intersect = intersects[0];
            let vi = intersect.index;
            // console.log(intersect.point, intersect.index, intersect.object);
            console.log(vi);
            if (select_vert_id != vi) {
                if (select_vert_id >= 0) {
                    geometry.attributes.color.setXYZ(select_vert_id, 0, 1, 1);
                }
                geometry.attributes.color.setXYZ(vi, 1, 0, 0);
                geometry.attributes.color.needsUpdate = true;
                select_vert_id = vi;
            }
        }
    }
}
```

```javascript
// geometry
geometry = new THREE.Geometry();
geometry.vertices.push(
    new THREE.Vector3(-1, -1, +1),
    new THREE.Vector3(+1, -1, +1),
    new THREE.Vector3(+1, +1, +1),
    new THREE.Vector3(-1, +1, +1),
);
geometry.colors.push(
    new THREE.Color(0x00ffff),
    new THREE.Color(0x00ffff),
    new THREE.Color(0x00ffff),
    new THREE.Color(0x00ffff),
);

var material = new THREE.PointsMaterial({
    size: 0.10,
    vertexColors: true,
});

pcd = new THREE.Points(geometry, material);

scene.add(pcd);

// レイキャストを作成
const raycaster = new THREE.Raycaster();
raycaster.params.Points.threshold = 0.1; // 点の当たり判定閾値(def:1)

canvas.addEventListener('mousedown', handleMouseDown, false);

function handleMouseDown(event) {
    if (event.button == 0) { // click left button
        const element = event.currentTarget;
        mouse.x = ((event.clientX - element.offsetLeft) / element.offsetWidth) * 2 - 1;
        mouse.y = -((event.clientY - element.offsetTop) / element.offsetHeight) * 2 + 1;

        raycaster.setFromCamera(mouse, camera);
        const intersects = raycaster.intersectObject(pcd);
        if (intersects.length > 0) {
            var intersect = intersects[0];
            let vi = intersect.index;
            // console.log(intersect.point, intersect.index, intersect.object);
            console.log(vi);
            if (select_vert_id != vi) {
                if (select_vert_id >= 0) {
                    geometry.colors[select_vert_id].setRGB(0,1,1);
                }
                geometry.colors[vi].setRGB(1,0,0);
                geometry.colorsNeedUpdate = true;
                select_vert_id = vi;
            }
        }
    }
}
```







## raycast (object単体)

[source](raycast_object.html)

ひとつのオブジェクトの中でカーソルがヒットした面を取得する

使い方は渡すのが配列から単体のオブジェクトに変わったくらいで、`intersectObjects`の方とほとんど変わらない。戻り値も同じ形式

```javascript
const intersects = raycaster.intersectObject(mesh);
```



# 特定のオブジェクトに画角を合わせる

[source](select_obj_and_zoom.html)

- [Camera Zoom To Fit Object - Questions - three.js forum](https://discourse.threejs.org/t/camera-zoom-to-fit-object/936/24)

```javascript
// camera = new THREE.PerspectiveCamera(45, width / height);
// constrols = new THREE.OrbitControls(camera, renderer.domElement);
// selection = object list
function fitCameraToSelection(camera, controls, selection, fitOffset = 1.2) {
    const box = new THREE.Box3();

    for (const object of selection) box.expandByObject(object); // selectionにオブジェクトのリストを入れる場合

    const size = box.getSize(new THREE.Vector3());
    const center = box.getCenter(new THREE.Vector3());

    const maxSize = Math.max(size.x, size.y, size.z);
    const fitHeightDistance = maxSize / (2 * Math.atan(Math.PI * camera.fov / 360));
    const fitWidthDistance = fitHeightDistance / camera.aspect;
    const distance = fitOffset * Math.max(fitHeightDistance, fitWidthDistance);

    const direction = controls.target.clone()
    .sub(camera.position)
    .normalize()
    .multiplyScalar(distance);

    controls.maxDistance = distance * 10;
    controls.target.copy(center);

    camera.near = distance / 100;
    camera.far = distance * 100;
    camera.updateProjectionMatrix();

    camera.position.copy(controls.target).sub(direction);
    controls.update();
}
```



# 範囲選択

[source]

- [Three.js selectionBoxを使ってみた - Qiita](https://qiita.com/tftf_w/items/ebac78ddd0ed998d2e9d)
- [Three.jsのSelectionBoxをOrthographicCameraで使いたい – しすこん](http://systemconcept.co.jp/blog/?p=1358)
- https://threejs.org/examples/#misc_boxselection
  - 公式のサンプルは矩形内にオブジェクトの中心が入っているかだけで判定している
  - モジュールとして実装されている`SelectionBox.js`がそもそもオブジェクトの中心しか判定しない仕様になっている
  - `SelectionBox.js`は[Frustum](https://threejs.org/docs/index.html#api/en/math/Frustum)を使っている
  - このサンプルをほぼそのまま使用する





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

[source](./load_file_ply.html)

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

## 点群を円形の形状で表示する

- https://threejs.org/examples/#webgl_points_billboards



# Camera

## 平行光源を視点に追従させる

デフォルトではOrbitControlで視点を移動しても光源とオブジェクトの位置関係は固定されているため、陰影に変化がない。

- [javascript - three.js directional light following camera - Stack Overflow](https://stackoverflow.com/questions/24790829/three-js-directional-light-following-camera)

```javascript
// 平行光源
const directionalLight = new THREE.DirectionalLight(0xffffff);
directionalLight.position.set(1, 1, 1);
scene.add(directionalLight);

function tick() {
    requestAnimationFrame(tick);
    directionalLight.position.copy( camera.position ); // カメラと光源の位置を一致させる
    renderer.render(scene, camera);
}
```



# Geometry

## Basic usage

- [[Three.js] BufferGeometry vs Geometry - Qiita](https://qiita.com/masato_makino/items/2ad5b1eb01a118d0a155)
- [three.jsのGeometryからBufferGeometryへの書き換えで躓いたところ - Qiita](https://qiita.com/izumi_ikezaki/items/9c43901c9b8f67423f49)

  - インスタンス生成
  - 頂点の座標を動かす
  - 頂点の座標変化を更新する
  - 法線情報の更新
  - 頂点数
  - 形式の相互変換
  - パフォーマンスの違い

## Convert between BufferGeometry and Geometry

[source](bufgeo_to_geo.html)

- [[Three.js] BufferGeometry vs Geometry - Qiita](https://qiita.com/masato_makino/items/2ad5b1eb01a118d0a155)
  - 二つの形式でパフォーマンスに差が出るのは頂点データの書き換えと生成時のみ。描画時は数万ポリゴンのオーダーなら差はない

- Geometry → BufferGeometry への変換では**面の情報はなくなる**(強制的にSTL形式になる)
- BufferGeometry → Geometry の変換では面のindexは必ず付加される(元がSTL形式の場合はそのまま連番でつけられる、PLY形式の場合はそのまま)

```javascript
var bufgeo = new THREE.BufferGeometry();
var geo = new THREE.Geometry();

bufgeo.fromGeometry(geo); // bufgeoにgeoの中身が読み込まれる
geo.fromBufferGeometry(bufgeo);
```



## Geometry

### data structure

```javascript
geometry.vertices[i].setX(posX);
```

### Create Geometry data on the fly

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

//the face normals and vertex normals can be calculated automatically if not supplied above 法線計算
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





## BufferGeometry

### data structure

```javascript
geometry.attributes.position.setX(i, posX);
```



### Create BufferGeometry data on the fly

- https://threejs.org/docs/#api/en/core/BufferGeometry

- 面を定義するのに頂点インデックスを使うこともできるし、頂点だけで直接定義することもできる
- `geometry.setAttribute('color', new THREE.Float32BufferAttribute(colors, 3));`
  - 渡す配列はコピー渡し
  - リスト型でも`Float32Array`型とかでも渡せる
- `geometry.setAttribute('color', new THREE.BufferAttribute(colors, 3));`
  - 渡す配列は参照渡し
  - `Float32Array`とかの型付き配列でないと渡せない

#### 頂点だけで面を指定(STL方式)

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
geometry.computeFaceNormals(); // 法線計算
geometry.computeVertexNormals();

var material = new THREE.MeshBasicMaterial({
    // color: 0xff0000, 
    vertexColors: true, // 頂点に対して色を設定する場合
});
var mesh = new THREE.Mesh(geometry, material);
scene.add(mesh);
```

#### 面を頂点インデックスで指定(PLY方式)

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



# Mesh, PointCloud

メッシュデータから点群を作成して一緒に表示させるデモ([html](module_file_load.html), [javascript](module_file_load.js))


```javascript
// mesh
let mesh = new THREE.Mesh(geometry, material);

// pcd
let pcd = new THREE.Points(geometry, material);
pcd.geometry.vertices
pcd.geometry.vertices.length
pcd.isPoints == true
```



# Group

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

## 基本構文 for,

```javascript
for(let i=0; i<10; i++){
    console.log(i);
}

arr = ["A","B","C"];
arr.foreach(function(name,index){
    console.log(name+":"+index);
});
arr.foreach(function(name){
    console.log(name);
});

dict = {"A":10,"B":20,"C":30};
for(const key in dict){
    console.log(key+":"+dict[key]);
}

for(const item of arr){
    console.log(item);
}
```



## 可変長引数にリストを展開して渡す

- [JavaScriptで可変長引数を扱うモダンな書き方 - 銀の弾丸](https://takamints.hatenablog.jp/entry/varg-on-es6-modern-javascript)
- [ES6で可変長の引数に配列を渡す - Qiita](https://qiita.com/keng0o/items/8be36613307d220ed19c)

```javascript
var aiu = ["あ","い","う"];
test(...aiu); // リストが展開されて可変長引数として入る
```



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

## マウスボタンの識別

- [マウスでクリックされたボタンを取得する - JavaScript プログラミング](https://www.ipentec.com/document/javascript-get-mouse-down-button)

```javascript
function handleMouseDown(event) {
    event = event || window.event; // IE対応
    target = document.getElementById("output");
    if (event.button == 0) {
        target.innerHTML = "左ボタンが押されました。";
    }
    else if (event.button == 1) {
        target.innerHTML = "中ボタンが押されました。";
    }
    else if (event.button == 2) {
        target.innerHTML = "右ボタンが押されました。";
    }
}

function handleMouseDown(event) {
    event = event || window.event; // IE対応
    target = document.getElementById("output");
    target.innerHTML = "";
    if ((event.buttons & 1) == 1) {
        target.innerHTML += "左ボタンが押されました。";
    }
    if ((event.buttons & 4) == 4) {
        target.innerHTML += "中ボタンが押されました。";
    }
    if ((event.buttons & 2) == 2) {
        target.innerHTML += "右ボタンが押されました。";
    }
}
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

## 配列の初期化

- [JavaScriptの配列初期化 - Qiita](https://qiita.com/yuta-ike/items/f60b940540a89332df2a)
- [JavaScriptで[ 0, 1, 2, 3, 4 ]のような連番の配列を生成する方法 - Qiita](https://qiita.com/suin/items/1b39ce57dd660f12f34b)

```javascript
// 任意のオブジェクトで初期化できる
a = Array(5).fill(0) //[0, 0, 0, 0, 0]
b = new Array(5).fill(0) //これも可
c = Array(5).fill(new Hoge()); // Hogeオブジェクトは一つだけ作られて、すべての配列が同じオブジェクト参照するので注意
d = [...Array(5)].map((_, i) => new Hoge()); // これなら各配列に異なるオブジェクトが入る
```



## 配列のconcat

- [Javascriptで配列をマージする - Qiita](https://qiita.com/bitarx/items/259f3a397fd2af23477a)

```javascript
let array_base = ['blue', 'green', 'yellow'];
let array_add = ['red', 'white'];
let result_concat = array_base.concat(array_add);
console.log(result_concat);
```



## HTML部品

### slider

[HTML5 スライダーで数値を入力するサンプル(range) | ITSakura](https://itsakura.com/html5-range)

```html
<input type="range" id="slider" value="0.5" min="-10.5" max="10" step="0.1" oninput="change_slider()">
<output id="output1">1</output>
<input type="button" value="test" onclick="test()">

<script type="text/javascript">
    function change_slider(){ // スライダーが動いたとき
        const slider = document.getElementById('slider');
        document.getElementById('output1').value=slider.value;
    }
    function test(){
        // スライダーの最小値を変更するテスト
        const slider = document.getElementById('slider');
        slider.min = -0.5;
    }
</script>
```

### dialog box

[HTMLでダイアログを表現する - dialog要素の基本 | CodeGrid](https://app.codegrid.net/entry/dialog-1)



# モジュール化

- [JavaScript モジュール - JavaScript | MDN](https://developer.mozilla.org/ja/docs/Web/JavaScript/Guide/Modules) : 使い方はこれだけ読んどけばOK
- [JavaScriptにおけるモジュール機構や、モジュールバンドラーについて | 楽しいだけで十分です](https://yinm.info/20200411/)
- [モジュールバンドラーはなぜモダンなフロントエンドの開発に必要なのか？｜Kosukeee｜note](https://note.com/billion_dollars/n/n596fecfdeb2e)
- [【JavaScript】 export/importでモジュール化とファイル分割する方法](https://affi-sapo-sv.com/note/js-module.php)

## 概要

- JavaScriptで作った関数などをファイル内でexportすれば他のJavaScriptファイルからimportして使える
- importは関数名そのままでもasで任意の名前にも変えられるし、`モジュール名.関数名`みたいに使うことも可能
- 複数モジュールをまとめるだけのファイルも作れる→このファイルをimportすると一括でimportできる
- モジュール内のexportは一括でimportできる。exportは逐次指定
- htmlで読み込むときは`<script type="module" src="main.mjs"></script>`のようにmodule指定が必要

## 注意点

- nginxでは読み込むURLの起点より上のディレクトリのモジュールは読めない

  ```json
  # "C:\Users\Public\Documents\GitHub\web_assembly_sample\samples\webasm\"内にあるhtmlファイルを参照したい場合
  # samples内にあるwebasmと同じ階層のディレクトリやそのさらに上は読めない
  location /webasm/ {
  	root    C:\\Users\\Public\\Documents\\GitHub\\web_assembly_sample\\samples;
  }
  # samples/js以下ののモジュールを読み込む場合はlocationをここまでで止めておく
  location /samples/ {
      root    C:\\Users\\Public\\Documents\\GitHub\\web_assembly_sample;
  }
  ```






## three.jsをモジュールとしてimport

