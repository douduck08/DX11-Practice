#pragma once
#include "ResourceManager.h"
#include "Bindable.h"
#include <string>

class PixelShader : public Bindable, public Resource
{
public:
	PixelShader(Graphics& graphics, const std::string& filePath) 
	{
		ComPtr<ID3DBlob> pBlob;
		D3DReadFileToBlob(std::wstring(filePath.begin(), filePath.end()).c_str(), &pBlob);
		graphics.GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	}

	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}

	static std::shared_ptr<PixelShader> Resolve(Graphics& gfx, const std::string& path)
	{
		return ResourceManager::Resolve<PixelShader>(gfx, path);
	}

	static std::string GetUID(const std::string& path)
	{
		using namespace std::string_literals;
		return typeid(PixelShader).name() + "#"s + path;
	}

private:
	ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
};