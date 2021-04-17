#include "light.h"

Light::Light(const char * file, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pdevcon, Microsoft::WRL::ComPtr<ID3D11Device>& pdev, XMMATRIX & view, XMMATRIX & proj)
	:
	Drawable(file,pdevcon,pdev,view,proj)
{
}

Light::Light(const char * file, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pdevcon, Microsoft::WRL::ComPtr<ID3D11Device>& pdev, XMMATRIX & view, XMMATRIX & proj, XMFLOAT3 rot, XMVECTOR pos)
	:
	Drawable(file, pdevcon, pdev, view, proj,rot,pos)
{
}

void Light::setLightConstantBuffer()
{
	D3D11_BUFFER_DESC bd2 = {};
	bd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd2.Usage = D3D11_USAGE_DYNAMIC;
	bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd2.MiscFlags = 0u;
	bd2.ByteWidth = sizeof(lightcb);
	bd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA sd2 = {};
	sd2.pSysMem = &lightcb;
	sd2.SysMemPitch = 0;
	sd2.SysMemSlicePitch = 0;
	if (HRESULT hr = pdev->CreateBuffer(&bd2, &sd2, &pLightBuffer) != S_OK) {
		throw hr;
	};

	pdevcon->PSSetConstantBuffers(0, 1, pLightBuffer.GetAddressOf());	
}

void Light::updateLightConstantBuffer()
{
	lightcb.color.x = color[0];
	lightcb.color.y = color[1];
	lightcb.color.z = color[2];

	D3D11_MAPPED_SUBRESOURCE s;
	pdevcon->Map(pLightBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &s);
	memcpy(s.pData, &lightcb, sizeof(lightcb));
	pdevcon->Unmap(pLightBuffer.Get(), NULL);
}

size_t Light::Draw()
{
	this->setLightConstantBuffer();
	this->setConstantBuffer();
	this->setVertexBuffer();
	this->setIndexBuffer();
	this->updateConstantBuffer(view, projection);
	this->updateLightConstantBuffer();
	pdevcon->DrawIndexed(mesh.getIndicesSize(), 0, 0);
	
	return this->mesh.getVerticesSize();
}

void Light::moveForward(float speed)
{	
	pos += DEFAULT_FORWARD*speed;
	XMStoreFloat4(&lightcb.pos, pos);
}

void Light::moveBackward(float speed)
{
	pos += DEFAULT_BACKWARD*speed;	
	XMStoreFloat4(&lightcb.pos, pos);
	
}

void Light::moveLeft(float speed)
{
	pos += DEFAULT_LEFT * speed;
	XMStoreFloat4(&lightcb.pos, pos);
}

void Light::moveRight(float speed)
{
	pos += DEFAULT_RIGHT * speed;
	XMStoreFloat4(&lightcb.pos, pos);
}
