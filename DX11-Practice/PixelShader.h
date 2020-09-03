#pragma once
#include "Bindable.h"
#include <string>

class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& graphics, const std::wstring& filePath) 
	{
		ComPtr<ID3DBlob> pBlob;
		D3DReadFileToBlob(filePath.c_str(), &pBlob);
		graphics.GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	}

	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}

private:
	ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
};