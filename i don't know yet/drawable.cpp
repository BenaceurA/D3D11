#include "drawable.h"


Drawable::Drawable(const char* file,Microsoft::WRL::ComPtr<ID3D11DeviceContext> & pdevcon, Microsoft::WRL::ComPtr<ID3D11Device> & pdev,XMMATRIX& view, XMMATRIX& proj)
	:
	pdevcon(pdevcon),
	pdev(pdev),
	view(view),
	projection(proj)
{
	if (!this->loadMesh(file, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices)) {
		throw std::runtime_error("failed to load mesh");
	};
}

Drawable::Drawable(const char* file,Microsoft::WRL::ComPtr<ID3D11DeviceContext> & pdevcon, Microsoft::WRL::ComPtr<ID3D11Device> & pdev, XMMATRIX& view, XMMATRIX& proj, XMFLOAT3 rot, XMVECTOR pos)
	:
	pdevcon(pdevcon),
	pdev(pdev),
	view(view),
	projection(proj),
	rot( rot),
	pos( pos)
{
	if (!this->loadMesh(file, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices)) {
		throw std::runtime_error("failed to load mesh");
	};
}

bool Drawable::loadMesh(const char * filename, unsigned int flags)
{
	return mesh.importfile(filename, flags);
}

void Drawable::setConstantBuffer()
{
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(Drawable::cb);
	bd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = &cb;

	pdev->CreateBuffer(&bd, &sd, &pConstantBuffer);
	pdevcon->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());
}

void Drawable::setVertexBuffer()
{
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = this->getVerticesByteSize();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	pdev->CreateBuffer(&bd, NULL, &pVertexBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	pdevcon->Map(pVertexBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData,getVerticesAdresse(), getVerticesByteSize());
	pdevcon->Unmap(pVertexBuffer.Get(), NULL);

	UINT stride = sizeof(Drawable::Vertex);
	UINT offset = 0;

	pdevcon->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);
}

void Drawable::setIndexBuffer()
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = getIndicesByteSize();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = getIndicesAdresse();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	pdev->CreateBuffer(&bufferDesc, &InitData, &pIndexBuffer);

	pdevcon->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	pdevcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void Drawable::updateConstantBuffer(XMMATRIX view, XMMATRIX projection)
{
	forward		= XMVector2TransformCoord(DEFAULT_FORWARD, XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z));
	backward	= XMVector2TransformCoord(DEFAULT_BACKWARD, XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z));
	left		= XMVector2TransformCoord(DEFAULT_LEFT, XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z));
	right		= XMVector2TransformCoord(DEFAULT_RIGHT, XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z));
	up			= XMVector2TransformCoord(DEFAULT_UP, XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z));
	down		= XMVector2TransformCoord(DEFAULT_DOWN, XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z));

	XMFLOAT3 pos_f;
	XMStoreFloat3(&pos_f, pos);

	world = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z)*XMMatrixTranslation(pos_f.x, pos_f.y, pos_f.z);

	cb = { XMMatrixTranspose(world*view*projection), XMMatrixTranspose(world) };
	

	D3D11_MAPPED_SUBRESOURCE ms;
	pdevcon->Map(pConstantBuffer.Get() , NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, &cb, sizeof(cb));
	pdevcon->Unmap(pConstantBuffer.Get(), NULL);
}

size_t Drawable::Draw()
{
	this->setConstantBuffer();
	this->setVertexBuffer();
	this->setIndexBuffer();
	this->updateConstantBuffer(view, projection);
	pdevcon->DrawIndexed(mesh.indices.size(), 0, 0);
	return this->mesh.vertices.size();
}

size_t Drawable::getVerticesByteSize()
{
	return sizeof(Vertex)*mesh.vertices.size();
}

size_t Drawable::getIndicesByteSize()
{
	return sizeof(unsigned int)*mesh.indices.size();
}

Drawable::Vertex* Drawable::getVerticesAdresse()
{
	return mesh.vertices.data();
}

unsigned int* Drawable::getIndicesAdresse()
{
	return mesh.indices.data();
}

bool Drawable::Mesh::importfile(const char * filename, unsigned int flags)
{

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, flags);
	if (!scene)
	{
		return false;
	}
	unsigned int numVertices = scene->mMeshes[0]->mNumVertices;
	vertices.reserve(sizeof(Vertex)*numVertices);
	for (size_t i = 0; i < numVertices; i++)
	{
		vertices.push_back({
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
	indices.reserve(sizeof(unsigned int)*numFaces * 3);
	for (size_t i = 0; i < numFaces; i++)
	{
		indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[0]);
		indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[1]);
		indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[2]);
	}
	return true;
}