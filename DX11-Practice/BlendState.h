#pragma once
#include "ResourceManager.h"
#include "Bindable.h"
#include <string>

class BlendState : public Bindable, public Resource
{
public:
	enum class Mode
	{
		Default,
		MaskZero,
	};

	BlendState(Graphics& graphics, Mode mode)
	{
		D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} };
		auto& brt = blendDesc.RenderTarget[0];
		if (mode == Mode::MaskZero)
		{
			brt.RenderTargetWriteMask = 0u;
		}

		GetDevice(graphics)->CreateBlendState(&blendDesc, &pState);
	}

	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->OMSetBlendState(pState.Get(), nullptr, 0xFFFFFFFFu);
	}

	static std::string GetUID(Mode mode)
	{
		using namespace std::string_literals;

		std::string modeName;
		if (mode == Mode::MaskZero)
		{
			modeName = "MaskZero";
		}
		else
		{
			modeName = "Default";
		}
		return typeid(BlendState).name() + "#"s + modeName;
	}

private:
	ComPtr<ID3D11BlendState> pState;
};