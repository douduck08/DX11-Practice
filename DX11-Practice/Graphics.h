#pragma once
#include "DWin32.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class Graphics
{
	friend class Bindable;

public:
	Graphics(HWND hWnd, UINT width, UINT height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
	UINT GetWidth();
	UINT GetHeight();

	void BeginFrame();
	void EndFrame();
	void SetColorBufferAsRenderTarget(ID3D11DepthStencilView* depth);
	void ClearColorBuffer(float r, float g, float b);
	void DrawIndexed(UINT indexCount, UINT startIndexLocation = 0u, INT baseVertexLocation = 0u);

private:
	UINT width;
	UINT height;
	ComPtr<ID3D11Device> pDevice;
	ComPtr<IDXGISwapChain> pSwap;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<ID3D11RenderTargetView> pTarget;
};