# embind with class

https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#classes

pybindと同じように公開するクラスを作成して、メンバ・メソッドも登録できる(登録数が増えるとファイルサイズも大きくなるので、なるべく最小限にする)

main.cpp

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

class ExMesh{
public:
	CMeshO mesh;
	bool calc_min_curvature(){
		mesh.vert.EnableVFAdjacency();
		mesh.face.EnableFFAdjacency();
		mesh.face.EnableVFAdjacency();
		mesh.vert.EnableCurvature();
		mesh.vert.EnableCurvatureDir();
		
		// delete invalid vertices and faces
		// compactVert(mesh);
		
		// calc normal
		tri::UpdateTopology<CMeshO>::FaceFace(mesh);
		tri::UpdateTopology<CMeshO>::VertexFace(mesh);
		// calc curvature
		tri::UpdateCurvature<CMeshO>::MeanAndGaussian(mesh);
		for(CMeshO::VertexIterator vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
			CMeshO::VertexType *pv = &*vi;
			float H = pv->Kh();
			float G = pv->Kg();
			float a = std::max(0.f, H*H-G); // h^2-gは手法によっては負になる場合があるので0でmaxとる
			MESHLAB_SCALAR sq = sqrt(a);
			pv->Q() = H-sq; // minimum curvature
			//pv->Q() = H+sq; // maximum curvature
		}
		std::pair<float,float> curv_minmax = tri::Stat<CMeshO>::ComputePerVertexQualityMinMax(mesh);
		float curv_min = curv_minmax.first;
		float curv_max = curv_minmax.second;
		printf("curvature min,max = (%f, %f)\n", curv_min, curv_max);
		tri::UpdateColor<CMeshO>::PerVertexQualityRamp(mesh, curv_min, curv_max);
		return true;
	}
	void create_pcd(val vert){
		vector<MESHLAB_SCALAR> vd;
		vd = vecFromJSArray<MESHLAB_SCALAR>(vert);
		
		vector<Point3m> coordVec;
		for(unsigned int i=0; i*3<vd.size(); ++i){
			coordVec.push_back(Point3m(vd[i*3+0], vd[i*3+1], vd[i*3+2]));
		}
		tri::BuildMeshFromCoordVector(mesh,coordVec);
		printf("vn : %d, fn : %d\n", mesh.VN(), mesh.FN());
	}
	void create_mesh(val vert, val face){
		vector<MESHLAB_SCALAR> vd;
		vector<int> vf;
	
		vector<Point3m> coordVec;
		vector<Point3i> indexVec;
		
		vd = vecFromJSArray<MESHLAB_SCALAR>(vert);
		for(unsigned int i=0; i*3<vd.size(); ++i){
			coordVec.push_back(Point3m(vd[i*3+0], vd[i*3+1], vd[i*3+2]));
		}
	
		if(face.isNull() || face["length"].isNull()){
			// 他の面と頂点を共有しない
			for(int i=0; i*3<coordVec.size(); ++i){
				indexVec.push_back(Point3i(i*3+0, i*3+1, i*3+2));
			}
		}else{
			vf = vecFromJSArray<int>(face);
			for(unsigned int i=0; i*3<vf.size(); ++i){
				indexVec.push_back(Point3i(vf[i*3+0], vf[i*3+1], vf[i*3+2]));
			}
		}
		
		tri::BuildMeshFromCoordVectorIndexVector(mesh, coordVec, indexVec);
		printf("vn : %d, fn : %d\n", mesh.VN(), mesh.FN());
	}
	val get_mesh_array(){
		val vo = val::object();
		vo.set("position", val::array());
		vo.set("findex", val::array());
		vo.set("color", val::array());
		for(CMeshO::VertexIterator vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
			Point3m p = vi->P();
			vo["position"].call<val>("push", p.X());
			vo["position"].call<val>("push", p.Y());
			vo["position"].call<val>("push", p.Z());
			
			vo["color"].call<val>("push", vi->cC()[0]/255.0);
			vo["color"].call<val>("push", vi->cC()[1]/255.0);
			vo["color"].call<val>("push", vi->cC()[2]/255.0);
		}
		for(CMeshO::FaceIterator fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){
			for(int i=0;i<fi->VN();++i)
				vo["findex"].call<val>("push", tri::Index(mesh, fi->V(i)));
		}
		return vo;
	}
};

int main(){
    std::cout << "Hello MeshLab" << std::endl;
}

EMSCRIPTEN_BINDINGS(my_class_example) { // you can use arbitrary name
	class_<ExMesh>("ExMesh")
			.constructor()
			.function("calc_min_curvature", &ExMesh::calc_min_curvature)
			.function("create_pcd", &ExMesh::create_pcd)
			.function("create_mesh", &ExMesh::create_mesh)
			.function("get_mesh_array", &ExMesh::get_mesh_array)
			;
}
```



index.html

```html
<html>

<head>
    <meta http-equiv="content-type" charset="utf-8">
</head>

<body>
    <script src="cmake_meshlab_class.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/r119/three.min.js"></script>
    <script src="../../../../three.js/examples/js/loaders/OBJLoader.js"></script>
    <script src="../../../../three.js/examples/js/loaders/PLYLoader.js"></script>
    <script src="../../../../three.js/examples/js/controls/OrbitControls.js"></script>

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

    <input type="button" value="sceneオブジェクト削除" onclick="delete_scene_data()">

    <script type="text/javascript">
        var inputfile = document.getElementById('inputfile')
        var scene;
        var geometry;
        var mesh;
        var material;
        let exMesh;

        // ページの読み込みを待つ(loadイベントが完了した時点で呼び出す関数を指定)
        window.addEventListener('load', init);

        function delete_scene_data() {
            let l = [];
            for (let i = 0; i < scene.children.length; ++i) {
                d = scene.children[i];
                if (d.type == "Mesh") {
                    l.push(d);
                }
            }
            // まとめてremoveしないと残る
            scene.remove(...l); // 可変長引数でとるので、リストを展開して入れる
        }

        function send_to_wasm() {
            dic = {};
            v = geometry.attributes.position.array;
            f = geometry.getIndex();
            if (f != null) {
                f = f.array;
            }
            exMesh.create_mesh(v, f);
        }
        function trigger_process_wasm() {
            exMesh.calc_min_curvature();
        }
        function load_wasm_data() {
            let geometry = new THREE.BufferGeometry();

            v = exMesh.get_mesh_array();
            vf = new Float32Array(v["position"]);
            indices = v["findex"]; // Int32Arrayだと動かない
            if (v.color) {
                colors = new Float32Array(v["color"]);
                geometry.setAttribute('color', new THREE.BufferAttribute(colors, 3));
                geometry.attributes.color.needsUpdate = true;
                console.log("colors", colors);
            }
            // 更新
            geometry.setAttribute('position', new THREE.BufferAttribute(vf, 3));
            geometry.setIndex(indices);
            let mesh = new THREE.Mesh(geometry, material);
            scene.add(mesh);
        }

        inputfile.addEventListener("change", function (e) {
            var file = e.target.files;

            var reader = new FileReader();
            var arraybuf;
            if (file.length > 0) {
                reader.readAsArrayBuffer(file[0]);

                reader.onload = function () { // ファイルの読み込みが完了したら実行
                    arraybuf = reader.result;
                    console.log(arraybuf);

                    geometry = load_obj(arraybuf);
                    add_geometry(geometry);
                }
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
            exMesh = new Module.ExMesh();

            const width = 960;
            const height = 540;

            const renderer = new THREE.WebGLRenderer({
                canvas: document.querySelector('#myCanvas') // 予め作っておいたcanvas要素を連携させる
            });
            renderer.setPixelRatio(window.devicePixelRatio);
            renderer.setSize(width, height); // canvas要素の大きさ自体が変わる
            document.body.appendChild(renderer.domElement);

            // シーンを作成 : 3D空間のこと，3Dオブジェクトや光源を置く場所
            scene = new THREE.Scene();

            // 平行光源
            const directionalLight = new THREE.DirectionalLight(0xffffff);
            directionalLight.position.set(1, 1, 1);
            scene.add(directionalLight);

            // 環境光源
            const light = new THREE.AmbientLight(0xFFFFFF, 0.3);
            scene.add(light);

            // camera
            const camera = new THREE.PerspectiveCamera(45, width / height);
            camera.position.set(0, 0, +5);

            // カメラコントローラーを作成
            const controls = new THREE.OrbitControls(camera, renderer.domElement);

            // geometry
            geometry = new THREE.BufferGeometry();
            geometry.fromGeometry(new THREE.BoxGeometry(1, 1, 1.5));
            geometry.computeFaceNormals();
            geometry.computeVertexNormals();

            material = new THREE.MeshPhongMaterial({
                flatShading: true,
                vertexColors: true,
            });
            const mesh = new THREE.Mesh(geometry, material);
            mesh.name = "def";
            scene.add(mesh);

            tick();

            function tick() {
                requestAnimationFrame(tick);
                directionalLight.position.copy( camera.position );
                renderer.render(scene, camera);
            }
        }

    </script>
</body>

</html>
```

