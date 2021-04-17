#pragma once
#include <DirectXMath.h>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>


struct Vertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
};

class Mesh
{	
public:
	Mesh(const char* filename, unsigned int flags);
	Vertex* getVerticesAddr();
	unsigned int* getIndicesAddr();
	unsigned int getVerticesByteSize();
	unsigned int getIndicesByteSize();
	unsigned int getVerticesSize();
	unsigned int getIndicesSize();
private:
	Assimp::Importer importer;
	std::vector<Vertex> VertexData;
	std::vector<unsigned int> Indices;
};

