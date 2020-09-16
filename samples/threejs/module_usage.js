// import * as THREE from '/GitHub/three.js/build/three.module.js';
// import { OrbitControls } from '/GitHub/three.js/examples/jsm/controls/OrbitControls.js';
import * as THREE from '../../../three.js/build/three.module.js';
import { OrbitControls } from '../../../three.js/examples/jsm/controls/OrbitControls.js';
var camera, scene, renderer;
var geometry, material, mesh;

// ページの読み込みを待つ(loadイベントが完了した時点で呼び出す関数を指定)
window.addEventListener('load', init);

function init() {
    // const width = window.innerWidth;
    // const height = window.innerHeight;
    const width = 960;
    const height = 540;

    const camera = new THREE.PerspectiveCamera(70, width / height, 0.01, 10);
    camera.position.z = 1;
    // camera.position.set(0, 0, +1);

    scene = new THREE.Scene();

    geometry = new THREE.BoxGeometry(0.2, 0.2, 0.2);
    material = new THREE.MeshNormalMaterial();

    mesh = new THREE.Mesh(geometry, material);
    scene.add(mesh);

    //light
    // var light = new THREE.HemisphereLight();
    // scene.add(light);

    const renderer = new THREE.WebGLRenderer({
        antialias: true,
        // canvas: document.querySelector('#myCanvas'), // 予め作っておいたcanvas要素を連携させる
    });
    renderer.setPixelRatio(window.devicePixelRatio);
    renderer.setSize(width, height);
    document.body.appendChild(renderer.domElement);

    // カメラコントローラーを作成
    const controls = new OrbitControls(camera, renderer.domElement);

    animate();

    function animate() {
        requestAnimationFrame(animate);

        mesh.rotation.x += 0.01;
        mesh.rotation.y += 0.02;

        renderer.render(scene, camera);
    }

}


