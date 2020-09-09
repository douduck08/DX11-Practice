#pragma once
#include "DWin32.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class Graphics
{
	friend class Bindable;
	friend class Drawable;

public:
	Graphics(HWND hWnd, UINT width, UINT height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
	
	void BeginFrame(float r, float g, float b);
	void BeginFrame(float* backcolor);
	void EndFrame();

private:
	ComPtr<ID3D11Device> pDevice;
	ComPtr<IDXGISwapChain> pSwap;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<ID3D11RenderTargetView> pTarget;
	ComPtr<ID3D11DepthStencilView> pDSV;
};