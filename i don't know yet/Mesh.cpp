#include "Mesh.h"

Mesh::Mesh(const char * filename, unsigned int flags)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, flags);
	if (scene == NULL)
	{
		throw std::runtime_error({ "Assimp aiScene is Null"});
	}
	unsigned int numVertices = scene->mMeshes[0]->mNumVertices;
	VertexData.reserve(sizeof(Vertex)*numVertices);
	for (size_t i = 0; i < numVertices; i++)
	{
		VertexData.push_back({
			{
				scene->mMeshes[0]->mVertices[i].x,
				scene->mMeshes[0]->mVertices[i].y,
				scene->mMeshes[0]->mVertices[i].z
			},
			{
				scene->mMeshes[0]->mNormals[i].x,
				scene->mMeshes[0]->mNormals[i].y,
				scene->mMeshes[0]->mNormals[i].z
			}
			});
	}
	unsigned int numFaces = scene->mMeshes[0]->mNumFaces;
	Indices.reserve(sizeof(unsigned int)*numFaces * 3);
	for (size_t i = 0; i < numFaces; i++)
	{
		Indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[0]);
		Indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[1]);
		Indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[2]);
	}
}

Vertex* Mesh::getVerticesAddr()
{	
	return VertexData.data();
}

unsigned int * Mesh::getIndicesAddr()
{
	return Indices.data();
}

unsigned int Mesh::getVerticesByteSize()
{
	return sizeof(Vertex) * VertexData.size();
}

unsigned int Mesh::getIndicesByteSize()
{
	return sizeof(unsigned int) * Indices.size();
}

unsigned int Mesh::getVerticesSize()
{
	return VertexData.size();
}

unsigned int Mesh::getIndicesSize()
{
	return Indices.size();
}
