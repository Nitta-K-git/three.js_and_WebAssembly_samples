import * as THREE from '../../../three.js/build/three.module.js';
import { OrbitControls } from '../../../three.js/examples/jsm/controls/OrbitControls.js';
import { PLYLoader } from '../../../three.js/examples/jsm/loaders/PLYLoader.js';

const canvas = document.querySelector('#myCanvas');
const sprite = new THREE.TextureLoader().load('../../../three.js/examples/textures/sprites/disc.png');

const points_threshold = 0.01; // 点の当たり判定閾値 ピクセル値ではなく3D空間内の距離なので近づくとピクセルの閾値は大きくなる
const mesh_name = "mesh";
const pcd_name = "pcd";
const selected_vert_col = 0xff0000;
const non_selected_vert_col = 0x00ffff;

let selected_vertex = []; // 選択されている頂点のid

let scene, camera;

const inputfile = document.getElementById('inputfile')
const raycaster = new THREE.Raycaster();

// Geometry変換
// 法線計算
// 頂点データの取得・点群生成
// 変数の整理
// カメラの距離を自動計算(bounding_boxの大きさから計算)

// sceneで扱うオブジェクトは単一で扱う
// sceneを毎回新規オブジェクトで作成 or 作ったオブジェクトに名前付けて、毎回削除する

window.addEventListener('load', init);

inputfile.addEventListener("change", load_file_event, false);

function init() {
    // const width = window.innerWidth;
    // const height = window.innerHeight;
    const width = 960;
    const height = 540;

    camera = new THREE.PerspectiveCamera(70, width / height, 0.01, 10);
    camera.position.z = 1;
    // camera.position.set(0, 0, +1);

    scene = new THREE.Scene();

    const geometry = new THREE.BoxGeometry(0.2, 0.2, 0.2);
    const material = new THREE.MeshNormalMaterial();

    const mesh = new THREE.Mesh(geometry, material);
    mesh.name = mesh_name;
    scene.add(mesh);

    const pcd = create_pcd_from_mesh(geometry);
    pcd.name = pcd_name;
    scene.add(pcd);

    //light
    // let light = new THREE.HemisphereLight();
    // scene.add(light);

    const renderer = new THREE.WebGLRenderer({
        antialias: true,
        canvas: document.querySelector('#myCanvas'), // 予め作っておいたcanvas要素を連携させる
    });
    renderer.setPixelRatio(window.devicePixelRatio);
    renderer.setSize(width, height);
    document.body.appendChild(renderer.domElement);

    const controls = new OrbitControls(camera, renderer.domElement);

    raycaster.params.Points.threshold = points_threshold; // 点の当たり判定閾値(def:1)
    canvas.addEventListener('mousedown', handleMouseDown);

    animate();

    function animate() {
        requestAnimationFrame(animate);

        // mesh.rotation.x += 0.01;
        // mesh.rotation.y += 0.02;

        renderer.render(scene, camera);
    }

}

function load_file_event(e) {
    const file = e.target.files;

    let reader = new FileReader();
    let arraybuf;
    reader.readAsArrayBuffer(file[0]);

    reader.onload = function () { // ファイルの読み込みが完了したら実行
        arraybuf = reader.result;
        console.log(arraybuf);

        const geometry = load_obj(arraybuf);
        const geo = new THREE.Geometry();
        geo.fromBufferGeometry(geometry);
        geo.computeBoundingBox();
        geo.computeFaceNormals(); // 法線を計算しないとMeshNormalMaterialで表示されない
        add_geometry(geo);
    }
}

function load_obj(arraybuf) {
    let loader = new PLYLoader();
    return loader.parse(arraybuf);
}

function add_geometry(geometry) {
    // const material = new THREE.MeshBasicMaterial({
    //     color: 0x66FF00
    // });
    const material = new THREE.MeshNormalMaterial();

    scene.remove(scene.getObjectByName(mesh_name));
    scene.remove(scene.getObjectByName(pcd_name));

    const mesh = new THREE.Mesh(geometry, material);
    mesh.name = mesh_name;
    scene.add(mesh);
    setCamera(geometry);

    const pcd = create_pcd_from_mesh(geometry);
    pcd.name = pcd_name;
    scene.add(pcd);
}

function setCamera(geometry) {
    const z = geometry.boundingBox.max.z * 2;
    camera.position.z = z;
}

// 点群で選択した色を変えるために各頂点の色を個別に定義して、vertexColorsを有効にする
// 点群の形状はデフォルトでは矩形。円形はこのサンプルを使用 https://threejs.org/examples/#webgl_points_billboards
function create_pcd_from_mesh(geo) {
    const geometry = new THREE.Geometry();
    geometry.vertices = geo.vertices;

    geometry.colors = [];
    // geometry.colors = Array(geo.vertices.length).fill(new THREE.Color(0x00ffff)); // この方法だと全部同じColorオブジェクト参照になってしまう
    geometry.colors = [...Array(geo.vertices.length)].map((_, i) => new THREE.Color(0x00ffff));

    var material = new THREE.PointsMaterial({
        size: 15,
        // color: 0xff0000,
        vertexColors: true,
        sizeAttenuation: false, // trueにするとsizeはpixel単位になるので1以上にすること
        map: sprite,
        alphaTest: 0.5,
        transparent: true,
    });

    const pcd = new THREE.Points(geometry, material);

    selected_vertex = Array(geo.vertices.length).fill(false);

    return pcd;
}

// 
function handleMouseDown(e) {
    const mouse = new THREE.Vector2();
    if (e.button == 0) { // click left button
        const element = e.currentTarget;
        const pcd = scene.getObjectByName(pcd_name);
        mouse.x = ((e.clientX - element.offsetLeft) / element.offsetWidth) * 2 - 1;
        mouse.y = -((e.clientY - element.offsetTop) / element.offsetHeight) * 2 + 1;

        raycaster.setFromCamera(mouse, camera);
        const intersects = raycaster.intersectObject(pcd);
        if (intersects.length > 0) {
            const intersect = intersects[0];
            const vi = intersect.index;
            // console.log(intersect.point, intersect.index, intersect.object);
            // console.log(vi);
            selected_vertex[vi] = !selected_vertex[vi]; // toggle
            // console.log(selected_vertex);
            if (selected_vertex[vi]) {
                pcd.geometry.colors[vi].setHex(selected_vert_col);
            }else{
                pcd.geometry.colors[vi].setHex(non_selected_vert_col);
            }
            pcd.geometry.colorsNeedUpdate = true;
        }
    }
}