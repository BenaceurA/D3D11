#pragma once
#define NOMINMAX
#include <D3D11.h>
#include <DirectXMath.h>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <wrl.h>
#include "Mesh.h"

using namespace DirectX;

class Drawable
{	
public:

	struct ConstantBuffer {
		XMMATRIX transform;
		XMMATRIX world;
	}cb;
	
	Drawable(const char* file,Microsoft::WRL::ComPtr<ID3D11DeviceContext> & pdevcon, Microsoft::WRL::ComPtr<ID3D11Device> & pdev, XMMATRIX& view, XMMATRIX& proj);
	Drawable(const char* file,Microsoft::WRL::ComPtr<ID3D11DeviceContext> & pdevcon, Microsoft::WRL::ComPtr<ID3D11Device> & pdev, XMMATRIX& view, XMMATRIX& proj,XMFLOAT3 rot, XMVECTOR pos);

	void setConstantBuffer();
	void setVertexBuffer();
	void setIndexBuffer();
	void updateConstantBuffer(XMMATRIX view, XMMATRIX projection);

	virtual size_t Draw();
	void SetPosition(XMVECTOR pos);
	XMVECTOR GetPosition();
	void SetRotation(XMFLOAT3 rot);
	XMFLOAT3 GetRotation();
	
private:

	
	
protected:

	Mesh mesh;

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> & pdevcon;
	Microsoft::WRL::ComPtr<ID3D11Device> & pdev;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;

	XMFLOAT3 rot = { 0.0f,0.0f,0.0f };
	XMVECTOR pos = { 0.0f,0.0f,0.0f };

	XMMATRIX world;
	
	XMMATRIX& view;
	XMMATRIX& projection;
	
	XMVECTOR DEFAULT_FORWARD = { 0.0f,0.0f,1.0f };
	XMVECTOR DEFAULT_BACKWARD = { 0.0f,0.0f,-1.0f };
	XMVECTOR DEFAULT_LEFT = { -1.0f,0.0f,0.0f };
	XMVECTOR DEFAULT_RIGHT = { 1.0f,0.0f,0.0f };
	XMVECTOR DEFAULT_UP = { 0.0f,1.0f,0.0f };
	XMVECTOR DEFAULT_DOWN = { 0.0f,-1.0f,0.0f };
	XMVECTOR forward;
	XMVECTOR backward;
	XMVECTOR left;
	XMVECTOR right;
	XMVECTOR up;
	XMVECTOR down;

public:
	bool drawMesh = true;
	bool drawBox = false;
};


