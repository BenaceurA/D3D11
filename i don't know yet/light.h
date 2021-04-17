#pragma once
#include "drawable.h"
#include <wrl.h>

class Light : public Drawable
{
public:
	Light(const char* file, Microsoft::WRL::ComPtr<ID3D11DeviceContext> & pdevcon, Microsoft::WRL::ComPtr<ID3D11Device> & pdev, XMMATRIX& view, XMMATRIX& proj);
	Light(const char* file, Microsoft::WRL::ComPtr<ID3D11DeviceContext> & pdevcon, Microsoft::WRL::ComPtr<ID3D11Device> & pdev, XMMATRIX& view, XMMATRIX& proj, XMFLOAT3 rot, XMVECTOR pos);

	struct lightConstantBuffer {
		XMFLOAT4 pos;
		XMFLOAT3 color;
		float pad;
	}lightcb;
	
	void setLightConstantBuffer();
	void updateLightConstantBuffer();
	size_t Draw();
	void moveForward(float speed);
	void moveBackward(float speed);
	void moveLeft(float speed);
	void moveRight(float speed);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pLightBuffer;
public:
	float color[3] = { 0.8f,0.8f,0.8f };
};

