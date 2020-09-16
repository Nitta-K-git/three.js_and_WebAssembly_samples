MeshLabのmeshデータをthree.jsのデータ構造に変換して、js側に送る。

js側でデータを再構築してMeshLabのデータを表示する



## main.cpp

```cpp
#include <iostream>
#include <common/ml_mesh_type.h>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/complex.h>

#include <vcg/complex/algorithms/update/curvature.h>
#include <vcg/complex/algorithms/update/color.h>
#include <vcg/complex/algorithms/clean.h>

#include <vcg/space/triangle3.h>

#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <string>
#include <vector>

using namespace emscripten;
using namespace std;
using namespace vcg;

CMeshO mesh;
vector<MESHLAB_SCALAR> vd;

void set_js_array(val v){
	vd = vecFromJSArray<MESHLAB_SCALAR>(v);
    
	// load as CMeshO
	vector<Point3m> coordVec;
	vector<Point3i> indexVec;
	
	for(unsigned int i=0; i*3<vd.size(); ++i){
		coordVec.push_back(Point3m(vd[i*3+0], vd[i*3+1], vd[i*3+2]));
	}
	tri::BuildMeshFromCoordVector(mesh,coordVec);
}

// meshlabのデータをthree.jsのデータ構造に変換
val vector_to_js(){
	tri::Dodecahedron(mesh);
	val vo = val::object();
	vo.set("position", val::array());
	vo.set("findex", val::array());
	for(CMeshO::VertexIterator vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
		Point3m p = vi->P();
		vo["position"].call<val>("push", p.X());
		vo["position"].call<val>("push", p.Y());
		vo["position"].call<val>("push", p.Z());
	}
	for(CMeshO::FaceIterator fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){
		for(int i=0;i<fi->VN();++i)
			vo["findex"].call<val>("push", tri::Index(mesh, fi->V(i)));
	}
	return vo;
}

EMSCRIPTEN_BINDINGS(module) {
	emscripten::function("set_js_array", &set_js_array);
	emscripten::function("process_data", &process_data);
	emscripten::function("vector_to_js", &vector_to_js);
}
```





## html

```html
<html>

<head>
    <meta http-equiv="content-type" charset="utf-8">
</head>

<body>
    <script src="cmake_meshlab_edit_data.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/r119/three.min.js"></script>
    <!-- <script src="js/loaders/OBJLoader.js"></script> -->
    <script src="../js/loaders/PLYLoader.js"></script>
    <script src="../js/controls/OrbitControls.js"></script>

    <h2>3Dファイルの読み込み・表示・編集</h2>
    <form>
        <input type="file" id="inputfile" accept=".ply" />
    </form>
    <canvas id="myCanvas"></canvas>
    <!-- 読み込んだデータをwasmに送るボタン -->
    <input type="button" value="wasmへデータ転送" onclick="send_to_wasm()">
    <!-- wasm側で処理を行うトリガー -->
    <input type="button" value="wasmでデータ処理" onclick="trigger_process_wasm()">
    <!-- wasmのデータをjavascript側に反映 -->
    <input type="button" value="wasmのデータ読込" onclick="load_wasm_data()">

    <script type="text/javascript">
        var inputfile = document.getElementById('inputfile')
        var scene;
        var geometry;
        var mesh;
        var material;

        // ページの読み込みを待つ(loadイベントが完了した時点で呼び出す関数を指定)
        window.addEventListener('load', init);

        function send_to_wasm() {
            Module.set_js_array(geometry.attributes.position.array);
        }
        function trigger_process_wasm() {
            Module.process_data();
        }
        function load_wasm_data() {
            let geometry = new THREE.BufferGeometry();

            v = Module.vector_to_js();
            vf = new Float32Array(v["position"]);
            indices = v["findex"]; // Int32Arrayだと動かない
            // 更新
            geometry.setAttribute('position', new THREE.BufferAttribute(vf, 3));
            geometry.setIndex(indices);
            // geometry.computeFaceNormals();
            let mesh = new THREE.Mesh(geometry, material);
            scene.add(mesh);
        }

        inputfile.addEventListener("change", function (e) {
            var file = e.target.files;

            var reader = new FileReader();
            var arraybuf;
            reader.readAsArrayBuffer(file[0]);

            reader.onload = function () { // ファイルの読み込みが完了したら実行
                arraybuf = reader.result;
                console.log(arraybuf);

                geometry = load_obj(arraybuf);
                geometry = geometry.toNonIndexed();
                add_geometry(geometry);
            }
        }, false);

        function load_obj(arraybuf) {
            var loader = new THREE.PLYLoader();
            return loader.parse(arraybuf);
        }

        function add_geometry(geometry) {
            mesh = new THREE.Mesh(geometry, material);
            mesh.name = "def";
            scene.remove(scene.getObjectByName("def"));
            scene.add(mesh);
        }

        function init() {
            const width = 960;
            const height = 540;

            const renderer = new THREE.WebGLRenderer({
                canvas: document.querySelector('#myCanvas') // 予め作っておいたcanvas要素を連携させる
            });
            renderer.setPixelRatio(window.devicePixelRatio);
            renderer.setSize(width, height); // canvas要素の大きさ自体が変わる
            document.body.appendChild(renderer.domElement);

            scene = new THREE.Scene();

            //light
            var light = new THREE.HemisphereLight();
            scene.add(light);

            // camera
            const camera = new THREE.PerspectiveCamera(45, width / height);
            camera.position.set(0, 0, +5);

            // カメラコントローラーを作成
            const controls = new THREE.OrbitControls(camera, renderer.domElement);

            // geometry
            geometry = new THREE.BufferGeometry();
            geometry.fromGeometry(new THREE.BoxGeometry(1, 1, 1.5));
            geometry = geometry.toNonIndexed();
            material = new THREE.MeshBasicMaterial({
                color: 0x6699FF
            });
            const mesh = new THREE.Mesh(geometry, material);
            mesh.name = "def";
            scene.add(mesh);

            tick();

            function tick() {
                requestAnimationFrame(tick);
                renderer.render(scene, camera);
            }
        }

    </script>
</body>

</html>
```

