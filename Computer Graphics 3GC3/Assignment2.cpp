//#include <tiny_obj_loader/tiny_obj_loader.h>
//
//#include <iostream>
//#include <fstream>
//#include <string>
//
//const char* filepath = "file";
//tinyobj::attrib_t attrib;
//std::vector<tinyobj::shape_t> shapes;
//std::vector<tinyobj::material_t> materials;
//
//std::vector<tinyobj::attrib_t> vbuffer;
//
//bool load_obj(char* obj_path) {
//	// tinyobj load obj
//	std::string warn, err;
//	bool bTriangulate = true;
//	bool bSuc = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, obj_path, nullptr, bTriangulate);
//	if (!bSuc)
//	{
//		std::cout << "tinyobj error: " << err.c_str() << std::endl;
//		return false;
//	}
//	return true;
//}
//
//int main() {
//	for (auto face : shapes[0].mesh.indices) {
//		int vid = face.vertex_index;
//		int nid = face.normal_index;
//		//fill in vertex buffer with vertex positions
//		vbuffer.push_back(attrib.vertices[vid * 3]);//vertex vid’s x
//		vbuffer.push_back(attrib.vertices[vid * 3 + 1]);//vertex vid’s y
//		vbuffer.push_back(attrib.vertices[vid * 3 + 2]);//vertex vid’s z
//		//fill in normal buffer with normal directions
//		nbuffer.push_back(attrib.normals[nid * 3]);
//		nbuffer.push_back(attrib.normals[nid * 3 + 1]);
//		nbuffer.push_back(attrib.normals[nid * 3 + 2]);
//	}
//
//
//
//}