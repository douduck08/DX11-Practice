#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Bindable.h"
#include "ConstantBuffer.h"

class Light : public Bindable
{
public:
	Light(Graphics& graphics, float r, float g, float b);
	void Bind(Graphics& graphics) noexcept override;
	void SetColor(float r, float g, float b);
	void SetPosition(float x, float y, float z);

private:
	struct LightData
	{
		DirectX::XMVECTOR lightColor;
		DirectX::XMVECTOR lightPosition;
	};

	LightData lightData;
	std::unique_ptr<SharedConstantBuffer<LightData>> pLightBuffer = nullptr;
};
