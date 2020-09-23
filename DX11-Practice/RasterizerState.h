#pragma once
#include "Bindable.h"

class RasterizerState : public Bindable
{
public:
	enum class Mode
	{
		Default,
		TwoSide,
		Shadow,
	};

	RasterizerState(Graphics& graphics, Mode mode)
	{
		D3D11_RASTERIZER_DESC rasterizerDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		rasterizerDesc.CullMode = mode == Mode::TwoSide ? D3D11_CULL_NONE : D3D11_CULL_BACK;

		if (mode == Mode::Shadow)
		{
			rasterizerDesc.DepthBias = 100000;
			rasterizerDesc.DepthBiasClamp = 0.0f;
			rasterizerDesc.SlopeScaledDepthBias = 1.0f;
		}

		GetDevice(graphics)->CreateRasterizerState(&rasterizerDesc, &pRasterizerState);
	}

	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->RSSetState(pRasterizerState.Get());
	}

private:
	ComPtr<ID3D11RasterizerState> pRasterizerState;
};