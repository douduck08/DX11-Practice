#pragma once
#include "ResourceManager.h"
#include "Bindable.h"
#include "Image.h"

class TextureView : public Bindable, public Resource
{
public:
	TextureView(Graphics& graphics, const std::string& filePath, UINT slot)
		: slot(slot)
	{
		auto img = Image::LoadFromFile(filePath);

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = img.GetWidth();
		textureDesc.Height = img.GetHeight();
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		// write image data into top mip level
		ComPtr<ID3D11Texture2D> pTexture;
		GetDevice(graphics)->CreateTexture2D(&textureDesc, nullptr, &pTexture);
		GetContext(graphics)->UpdateSubresource(
			pTexture.Get(), 0u, nullptr, img.GetPixelsConst(), img.GetWidth() * sizeof(unsigned int), 0u
		);

		// generate the mip chain using the gpu rendering pipeline
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;
		GetDevice(graphics)->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pTextureView);
		GetContext(graphics)->GenerateMips(pTextureView.Get());
	}

	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->PSSetShaderResources(slot, 1u, pTextureView.GetAddressOf());
	}

	static std::string GetUID(const std::string& filePath, UINT slot)
	{
		using namespace std::string_literals;
		return typeid(TextureView).name() + "#"s + filePath;
	}

private:
	UINT slot;
	ComPtr<ID3D11ShaderResourceView> pTextureView;
};