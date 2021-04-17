#include "drawable.h"
#include "Mesh.h"


Drawable::Drawable(const char* file,Microsoft::WRL::ComPtr<ID3D11DeviceContext> & pdevcon, Microsoft::WRL::ComPtr<ID3D11Device> & pdev,XMMATRIX& view, XMMATRIX& proj)
	:
	pdevcon(pdevcon),
	pdev(pdev),
	view(view),
	projection(proj),
	mesh(file, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices)
{
}

Drawable::Drawable(const char* file,Microsoft::WRL::ComPtr<ID3D11DeviceContext> & pdevcon, Microsoft::WRL::ComPtr<ID3D11Device> & pdev, XMMATRIX& view, XMMATRIX& proj, XMFLOAT3 rot, XMVECTOR pos)
	:
	pdevcon(pdevcon),
	pdev(pdev),
	view(view),
	projection(proj),
	rot( rot),
	pos( pos),
	mesh(file, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices)
{	
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
	bd.ByteWidth = mesh.getVerticesByteSize();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	pdev->CreateBuffer(&bd, NULL, &pVertexBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	pdevcon->Map(pVertexBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData,mesh.getVerticesAddr(), mesh.getVerticesByteSize());
	pdevcon->Unmap(pVertexBuffer.Get(), NULL);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	pdevcon->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);
}

void Drawable::setIndexBuffer()
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = mesh.getIndicesByteSize();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = mesh.getIndicesAddr();
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
	pdevcon->DrawIndexed(mesh.getIndicesSize(), 0, 0);
	
	return this->mesh.getVerticesSize();
}

void Drawable::SetPosition(XMVECTOR pos)
{
	this->pos = pos;
}

XMVECTOR Drawable::GetPosition()
{
	return pos;
}

void Drawable::SetRotation(XMFLOAT3 rot)
{
	this->rot = rot;
}

XMFLOAT3 Drawable::GetRotation()
{
	return rot;
}
