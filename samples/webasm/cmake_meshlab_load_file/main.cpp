#include <iostream>
#include <common/ml_mesh_type.h>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/complex.h>

#include <vcg/complex/algorithms/update/curvature.h>
#include <vcg/complex/algorithms/update/color.h>
#include <vcg/complex/algorithms/clean.h>
// #include <vcg/complex/algorithms/create/resampler.h>
// #include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h>
// #include <vcg/complex/algorithms/local_optimization.h>
// #include "quadric_simp.h" // for simplification collapse edge
// #include <vcg/complex/algorithms/clustering.h>
// #include <vcg/complex/algorithms/create/plymc/plymc.h>

// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <algorithm>

#include <vcg/space/triangle3.h>

#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <string>
#include <vector>

using namespace emscripten;
using namespace std;
using namespace vcg;

CMeshO mesh;
vector<double> vd;

int main()
{
    std::cout << "Hello MeshLab" << std::endl;
}

void init_mesh(val v){
	printf("called\n");
    std::vector<float> vd = vecFromJSArray<float>(v);
	for(auto a:vd){
		std::cout << a << std::endl;
	}
	int size = static_cast<int>(vd.size())/3;
	std::cout << "size: " << size << std::endl;
	auto vi = vcg::tri::Allocator<CMeshO>::AddVertices(mesh, 3);
	for(int i=0; i<size; ++i){
		vi[i].P() = Point3m(vd[i*3], vd[i*3+1], vd[i*3+2]);
	}
	std::cout<<"mesh:"<<mesh.VN() << std::endl;
	CMeshO::VertexIterator vv;
	for(vv=mesh.vert.begin(); vv!=mesh.vert.end(); ++vv){
		std::cout << vv->P().X() << std::endl;
	}
}

void set_js_array(val v){
	vd = vecFromJSArray<double>(v);
	cout << "vecFromJSArray" << endl;
	for(auto && v:vd){
		cout << v << endl;
	}
}

void print_data(){
	cout << "print_data" << endl;
	for(auto && v:vd){
		cout << v << endl;
		//
	}
}
/*
	v = {};
	v["vertices"] = geometry.attributes.position.array;
	v["faces"] = geometry.getIndex().array;
	Module.create_mesh_from_BufferGeometry(v);
*/
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
	
//	for(auto &&a : vec_vert)
//		cout << a << endl;
//	for(auto &&a : vec_face)
//		cout << a << endl;
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


EMSCRIPTEN_BINDINGS(module) {
	emscripten::function("set_js_array", &set_js_array);
	emscripten::function("print_data", &print_data);
	emscripten::function("init_mesh", &init_mesh);
	emscripten::function("create_mesh_from_BufferGeometry", &create_mesh_from_BufferGeometry);
	emscripten::function("create_mesh_from_Geometry", &create_mesh_from_Geometry);
}
