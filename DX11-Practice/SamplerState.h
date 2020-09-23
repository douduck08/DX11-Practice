#pragma once
#include "ResourceManager.h"
#include "Bindable.h"
#include <string>

class SamplerState : public Bindable, public Resource
{
public:
	SamplerState(Graphics& graphics, UINT slot)
		: slot(slot)
	{
		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

		GetDevice(graphics)->CreateSamplerState(&samplerDesc, &pSamplerState);
	}

	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->PSSetSamplers(slot, 1, pSamplerState.GetAddressOf());
	}

	static std::string GetUID(UINT slot)
	{
		using namespace std::string_literals;
		return typeid(SamplerState).name() + "#"s + std::to_string(slot);
	}


private:
	UINT slot;
	ComPtr<ID3D11SamplerState> pSamplerState;
};