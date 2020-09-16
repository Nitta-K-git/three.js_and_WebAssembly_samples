# `cmake_meshlab_load_file`

three.jsのBufferGeometry・Geometry型のデータをMeshLabのデータ型に変換する

基本的な方針は、three.js側のデータから頂点・面の情報を配列形式で取り出して、MeshLabに渡す

ただし、BufferGeometryとGeometryでは配列の形状が違うため、別々に変換用の関数を作成する

## BufferGeometry

配列は1次元型なので、C++側で頂点単位・面単位にまとめる

```javascript
v = {};
create_mesh_from_BufferGeometry
v["vertices"] = geometry.attributes.position.array;
v["faces"] = geometry.getIndex().array;
Module.create_mesh_from_BufferGeometry(v);
```

```cpp
void create_mesh_from_BufferGeometry(val v){
    val vertices = v["vertices"];
    val faces = v["faces"];

    if(vertices["length"].isNull()){
        cout << "vertices is not array" << endl;
        return;
    }
    if(faces["length"].isNull()){
        cout << "faces is not array" << endl;
        return;
    }
    vector<MESHLAB_SCALAR> vec_vert = vecFromJSArray<MESHLAB_SCALAR>(vertices);
    vector<int> vec_face = vecFromJSArray<int>(faces);

    vector<Point3m> coordVec;
    vector<Point3i> indexVec;

    for(unsigned int i=0; i*3<vec_vert.size(); ++i){
        coordVec.push_back(Point3m(vec_vert[i*3+0], vec_vert[i*3+1], vec_vert[i*3+2]));
    }
    for(unsigned int i=0; i*3<vec_face.size(); ++i){
        indexVec.push_back(Point3i(vec_face[i*3+0], vec_face[i*3+1], vec_face[i*3+2]));
    }
    tri::BuildMeshFromCoordVectorIndexVector(mesh,coordVec,indexVec);

    cout << "vert:" << endl;
    for(auto &&v:mesh.vert){
        cout << v.Index() << endl;
    }
    cout << "face:" << endl;
    for(auto &&f:mesh.face){
        cout << f.Index() << endl;
    }
}
```





## Geometry

配列が最初から頂点単位・面単位になっているので、そのままC++側で変換する

```javascript
v = {};
v["vertices"] = geometry.vertices;
v["faces"] = geometry.faces;
Module.create_mesh_from_Geometry(v);
```

```cpp
void create_mesh_from_Geometry(val v){
	val vertices = v["vertices"];
	val faces = v["faces"];
	
	if(vertices["length"].isNull()){
		cout << "vertices is not array" << endl;
		return;
	}
	if(faces["length"].isNull()){
		cout << "faces is not array" << endl;
		return;
	}
	
	vector<Point3m> coordVec;
	vector<Point3i> indexVec;
	
	for(unsigned i = 0; i < vertices["length"].as<unsigned>(); ++i) {
		MESHLAB_SCALAR x,y,z;
		x = vertices[i]["x"].as<MESHLAB_SCALAR>();
		y = vertices[i]["y"].as<MESHLAB_SCALAR>();
		z = vertices[i]["z"].as<MESHLAB_SCALAR>();
		coordVec.push_back(Point3m(x,y,z));
	}
	for(unsigned i = 0; i < faces["length"].as<unsigned>(); ++i) {
		int a,b,c;
		a = faces[i]["a"].as<int>();
		b = faces[i]["b"].as<int>();
		c = faces[i]["c"].as<int>();
		indexVec.push_back(Point3i(a,b,c));
	}
	tri::BuildMeshFromCoordVectorIndexVector(mesh,coordVec,indexVec);
	
	cout << "vert:" << endl;
	for(auto &&v:mesh.vert){
		cout << v.Index() << endl;
	}
	cout << "face:" << endl;
	for(auto &&f:mesh.face){
		cout << f.Index() << endl;
	}
}
```



