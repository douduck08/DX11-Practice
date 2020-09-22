#pragma once
#include "ResourceManager.h"
#include "Bindable.h"

class DepthStencilBuffer : public Bindable
{
public:
	DepthStencilBuffer(Graphics& graphics, UINT width, UINT height)
		: width(width), height(height)
	{
		// create depth stensil texture
		ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC depthDesc = {};
		depthDesc.Width = width;
		depthDesc.Height = height;
		depthDesc.MipLevels = 1u;
		depthDesc.ArraySize = 1u;
		depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthDesc.SampleDesc.Count = 1u;
		depthDesc.SampleDesc.Quality = 0u;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		GetDevice(graphics)->CreateTexture2D(&depthDesc, nullptr, &pDepthStencil);

		// create view of depth stensil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0u;
		GetDevice(graphics)->CreateDepthStencilView(pDepthStencil.Get(), &dsvDesc, &pDepthStencilView);
	}

	void Bind(Graphics& graphics) noexcept override
	{
		throw "DepthStencilBuffer cannot bind";;
	}

	ID3D11DepthStencilView* Get() 
	{
		return pDepthStencilView.Get();
	}

	void Clear(Graphics& graphics)
	{
		GetContext(graphics)->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

private:
	UINT width;
	UINT height;
	ComPtr<ID3D11Texture2D> pDepthStencil;
	ComPtr<ID3D11DepthStencilView> pDepthStencilView;
};