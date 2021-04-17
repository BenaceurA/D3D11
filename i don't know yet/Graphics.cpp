#include "Graphics.h"

void Graphics::InitD3D(HWND hWnd)
{
	
	DXGI_SWAP_CHAIN_DESC scd;
	

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	
	scd.BufferCount = 1;                                    
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  
	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      
	scd.OutputWindow = hWnd;                                
	scd.SampleDesc.Count = 1u;                               
	scd.Windowed = !fullscreen;   
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	
	if (D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon) != S_OK) {
		throw std::runtime_error("CreateDeviceAndSwapChain FAIL");
	}

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	devcon->RSSetViewports(1, &viewport);

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;

	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	
	if (dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer) != S_OK)
	{
		throw std::runtime_error("CreateRenderTargetView FAIL");
	}
	pBackBuffer->Release();

	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = (UINT)SCREEN_WIDTH;
	descDepth.Height = (UINT)SCREEN_HEIGHT;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (dev->CreateTexture2D(&descDepth, NULL, &pDepthStencil) != S_OK)
	{
		throw std::runtime_error("CreateTexture2D Fail");
	}

	D3D11_DEPTH_STENCIL_DESC DSP = {};
	DSP.DepthEnable = true;
	DSP.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DSP.DepthFunc = D3D11_COMPARISON_LESS;
	

	if (dev->CreateDepthStencilState(&DSP, &pDSState) != S_OK) {
		throw std::runtime_error("DepthStencilState FAIL!");
	};
	devcon->OMSetDepthStencilState(pDSState.Get(), 1u);
	
	D3D11_DEPTH_STENCIL_VIEW_DESC DSV = {};
	DSV.Format = DXGI_FORMAT_D32_FLOAT;
	DSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSV.Texture2D.MipSlice = 0u;

	
	if (dev->CreateDepthStencilView(pDepthStencil.Get(), &DSV, &pDSV) != S_OK) {
		throw std::runtime_error("DepthStencilView FAIL!");
	}
  

	//set the render target as the back buffer
	devcon->OMSetRenderTargets(1, backbuffer.GetAddressOf(), pDSV.Get());

	//init imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(dev.Get(), devcon.Get());

}

void Graphics::CleanD3D(void)
{
	swapchain->SetFullscreenState(FALSE, NULL);

}

void Graphics::renderframe(float yoffset, bool moveforward, bool movebackward, bool nz,bool pz, bool nx, bool px)
{
	//camera
	cam.rotateCamera(0.0f, yoffset+ 3.14159f, 0.0f);
	if (moveforward)
	{
		cam.moveForward(0.05f);
	}
	else if (movebackward)
	{
		cam.moveBackward(0.05f);
	}
	view = cam.updateViewMatrix();
	

	if (pz)
	{
		light.moveForward(0.05);
	}
	if (nz)
	{
		light.moveBackward(0.05);
	}
	if (px)
	{
		light.moveRight(0.05);
	}
	if (nx)
	{
		light.moveLeft(0.05);
	}
	
	UINT total = 0;

	//frustumcull
	frustumCull.extractFrustum();
	
	devcon->PSSetShader(pLPS.Get(), 0, 0);	
	total += light.Draw();

	devcon->PSSetShader(pPS.Get(), 0, 0);
	total += Suzan.Draw();

	{
		XMFLOAT3 r = Suzan.GetRotation();
		XMFLOAT3 p;
		DirectX::XMStoreFloat3(&p, Suzan.GetPosition());
		
		ImGui::Begin("test?");
		ImGui::Text("vertices : %d", total);
		ImGui::Checkbox("Draw Mesh", &Suzan.drawMesh);
		ImGui::Checkbox("Draw HitBox", &Suzan.drawBox);

		ImGui::SliderFloat("X rot", &r.x, 0.0f, 6.282f);
		ImGui::SliderFloat("X pos", &p.x, 0.0f, 10.0f);

		ImGui::ColorPicker3("light color", light.color);
		ImGui::End();


		Suzan.SetRotation({ r.x,0.0f,0.0f });
		Suzan.SetPosition({ p.x,0.0f,0.0f });
		
	}
}

void Graphics::clearRenderTarget()
{
	float c[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	devcon->ClearRenderTargetView(backbuffer.Get(), c);
	devcon->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::Present()
{
	swapchain->Present(0, 0);
}

void Graphics::initPipeline()
{
	
	ID3DBlob *VS, *PS ,*LPS;
	
	D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, &VS, nullptr);
	D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &PS, nullptr);
	D3DCompileFromFile(L"lightpixelshader.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &LPS, nullptr);

	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);
	dev->CreatePixelShader(LPS->GetBufferPointer(), LPS->GetBufferSize(), NULL, &pLPS);

	devcon->VSSetShader(pVS.Get(), 0, 0);
	

	const short ied_size = 2;

	D3D11_INPUT_ELEMENT_DESC ied[ied_size] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	dev->CreateInputLayout(ied, ied_size, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devcon->IASetInputLayout(pLayout.Get());	
}

void Graphics::initGraphics()
{
	
}


