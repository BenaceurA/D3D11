#pragma once
#include "drawable.h"
#include <wrl.h>

class Light : Drawable
{
public:
	Light(const char* file, Microsoft::WRL::ComPtr<ID3D11DeviceContext> & pdevcon, Microsoft::WRL::ComPtr<ID3D11Device> & pdev, XMMATRIX& view, XMMATRIX& proj);
	Light(const char* file, Microsoft::WRL::ComPtr<ID3D11DeviceContext> & pdevcon, Microsoft::WRL::ComPtr<ID3D11Device> & pdev, XMMATRIX& view, XMMATRIX& proj, XMFLOAT3 rot, XMVECTOR pos);
	Light() = delete;
	~Light() = default;
	struct lightConstantBuffer {
		XMFLOAT4 pos;
	}lightcb;
	
	void setLightConstantBuffer();
	void updateLightConstantBuffer();
	virtual size_t Draw();
	void moveForward(float speed);
	void moveBackward(float speed);
	void moveLeft(float speed);
	void moveRight(float speed);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pLightBuffer;
};

