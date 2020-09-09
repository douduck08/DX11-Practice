#pragma once
#include "ConstantBuffer.h"
#include <memory>

class LightConstantBuffer : public Bindable
{
public:
	LightConstantBuffer(Graphics& graphics);
	void Bind(Graphics& graphics) noexcept override;
	void SetColor(float r, float g, float b);
	void SetPosition(float x, float y, float z, float w);

private:
	struct LightData
	{
		float lightColor[4];
		float lightPosition[4];
	};

	bool needUpdate;
	LightData lightData;
	std::unique_ptr<SharedConstantBuffer<LightData>> pLightBuffer = nullptr;
};