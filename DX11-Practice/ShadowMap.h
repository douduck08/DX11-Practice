#pragma once
#include "ResourceManager.h"
#include "Bindable.h"

class ShadowMap : public Bindable
{
public:
	ShadowMap(Graphics& graphics, UINT width, UINT height, UINT slot)
		: width(width), height(height), slot(slot)
	{
		// create depth stensil texture
		ComPtr<ID3D11Texture2D> pShadowMap;
		D3D11_TEXTURE2D_DESC depthDesc = {};
		depthDesc.Width = width;
		depthDesc.Height = height;
		depthDesc.MipLevels = 1u;
		depthDesc.ArraySize = 1u;
		depthDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		depthDesc.SampleDesc.Count = 1u;
		depthDesc.SampleDesc.Quality = 0u;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		GetDevice(graphics)->CreateTexture2D(&depthDesc, nullptr, &pShadowMap);

		// create view of depth stensil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0u;
		GetDevice(graphics)->CreateDepthStencilView(pShadowMap.Get(), &dsvDesc, &pShadowMapView);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = depthDesc.MipLevels;
		GetDevice(graphics)->CreateShaderResourceView(pShadowMap.Get(), &srvDesc, &pShaderResourceView);

		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };
		samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		GetDevice(graphics)->CreateSamplerState(&samplerDesc, &pSamplerState);
	}

	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->PSSetShaderResources(slot, 1u, pShaderResourceView.GetAddressOf());
		GetContext(graphics)->PSSetSamplers(1u, 1, pSamplerState.GetAddressOf());
	}

	void SetRenderTarget(Graphics& graphics)
	{
		GetContext(graphics)->OMSetRenderTargets(0u, nullptr, pShadowMapView.Get());

		// configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = static_cast<FLOAT>(width);
		vp.Height = static_cast<FLOAT>(height);
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		GetContext(graphics)->RSSetViewports(1u, &vp);
	}

	void Clear(Graphics& graphics)
	{
		GetContext(graphics)->ClearDepthStencilView(pShadowMapView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

private:
	UINT slot;
	UINT width;
	UINT height;
	ComPtr<ID3D11DepthStencilView> pShadowMapView;
	ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
	ComPtr<ID3D11SamplerState> pSamplerState;
};