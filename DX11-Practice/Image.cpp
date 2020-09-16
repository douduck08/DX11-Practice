#include "Image.h"

#pragma comment(lib,"DirectXTex.lib")

Image Image::LoadFromFile(const std::string& filePath)
{
	DirectX::ScratchImage scratch;
	HRESULT hr = DirectX::LoadFromWICFile(std::wstring(filePath.begin(), filePath.end()).c_str(), DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, scratch);

	DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
	if (scratch.GetImage(0, 0, 0)->format != format)
	{
		DirectX::ScratchImage converted;
		hr = DirectX::Convert(
			*scratch.GetImage(0, 0, 0),
			format,
			DirectX::TEX_FILTER_DEFAULT,
			DirectX::TEX_THRESHOLD_DEFAULT,
			converted
		);

		return Image(std::move(converted));
	}

	return Image(std::move(scratch));
}

unsigned int Image::GetWidth() const noexcept
{
	return (unsigned int)scratch.GetMetadata().width;
}

unsigned int Image::GetHeight() const noexcept
{
	return (unsigned int)scratch.GetMetadata().height;
}

uint8_t* Image::GetPixels()
{
	return scratch.GetPixels();
}

const uint8_t* Image::GetPixelsConst() const
{
	return const_cast<Image*>(this)->GetPixels();
}

Image::Image(DirectX::ScratchImage scratch) noexcept
	: scratch(std::move(scratch))
{
}
