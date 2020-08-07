#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX

#include <d3d11.h>
#include <d3dcompiler.h>
#include <cmath>	
#include <DirectXMath.h>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <wrl.h>

#include "Camera.h"
#include "drawable.h"
#include "light.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib,"D3DCompiler.lib")


#define fullscreen 0

#if (fullscreen == 0)
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#else
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#endif

#ifdef _DEBUG
#define DEVICE_DEBUG D3D11_CREATE_DEVICE_DEBUG
#else
#define DEVICE_DEBUG NULL
#endif
class Graphics
{
public:
	
private:	
	Microsoft::WRL::ComPtr<IDXGISwapChain>				swapchain;
	Microsoft::WRL::ComPtr<ID3D11Device>				dev;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			devcon;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		backbuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pLPS;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pLayout;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		pDSState;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pDepthStencil;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		pDSV;	
	
	
	

private:

	Camera cam;
	XMMATRIX view;
	XMMATRIX projection = XMMatrixPerspectiveLH(0.1f, 0.1f*((float)SCREEN_HEIGHT / (float)SCREEN_WIDTH), 0.1f, 50.0f);


public:
	void InitD3D(HWND hWnd);     
	void CleanD3D(); 
	void renderframe(float yoffset, bool moveforward,bool movebackward, bool nz, bool pz, bool nx, bool px);
	void clearRenderTarget();
	void Present();
	void initPipeline();
	void initGraphics();
	DirectX::XMVECTOR mPosition = { 0.0f,0.0f,-1.0f };

public:
	Graphics();
	~Graphics();
private:
	
	Drawable Suzan { "monkey.obj",devcon,dev,view,projection };
	Light light { "light.obj",devcon,dev,view,projection ,{},{4.0f,0.0f,0.0f } };
	
};

