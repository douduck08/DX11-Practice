#pragma once
#include "ResourceManager.h"
#include "Bindable.h"
#include <string>

class DepthStencilState : public Bindable, public Resource
{
public:
	enum class Mode
	{
		LessWrite,
		Equal,
	};

	DepthStencilState(Graphics& graphics, Mode mode)
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

		if (mode == Mode::LessWrite)
		{
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		}
		else if (mode == Mode::Equal)
		{
			dsDesc.DepthFunc = D3D11_COMPARISON_EQUAL;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		}

		GetDevice(graphics)->CreateDepthStencilState(&dsDesc, &pState);
	}

	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->OMSetDepthStencilState(pState.Get(), 0u);
	}

	static std::string GetUID(Mode mode)
	{
		using namespace std::string_literals;

		std::string modeName;
		if (mode == Mode::LessWrite)
		{
			modeName = "LessWrite";
		}
		else if (mode == Mode::Equal)
		{
			modeName = "Equal";
		}
		return typeid(DepthStencilState).name() + "#"s + modeName;
	}

private:
	ComPtr<ID3D11DepthStencilState> pState;
};