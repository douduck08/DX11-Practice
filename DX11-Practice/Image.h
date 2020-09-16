#pragma once
#include <dxtex/DirectXTex.h>

class Image
{
public:
	Image(Image&& source) noexcept = default;
	Image(Image&) = delete;
	Image& operator=(Image&& donor) noexcept = default;
	Image& operator=(const Image&) = delete;

	static Image LoadFromFile(const std::string& filePath);

	unsigned int GetWidth() const noexcept;
	unsigned int GetHeight() const noexcept;
	uint8_t* GetPixels();
	const uint8_t* GetPixelsConst() const;

private:
	Image(DirectX::ScratchImage scratch) noexcept;

private:
	DirectX::ScratchImage scratch;
};