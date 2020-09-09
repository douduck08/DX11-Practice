#include "LightConstantBuffer.h"
#include "SlotConfig.h"

LightConstantBuffer::LightConstantBuffer(Graphics& graphics)
	: needUpdate(false)
	, lightData({ 1, 1, 1, 1, 0, 0, 0, 1 })
{
	pLightBuffer = std::make_unique<SharedConstantBuffer<LightData>>(graphics, lightData, LIGHT_CBUFFER_SLOT);
}

void LightConstantBuffer::Bind(Graphics& graphics) noexcept
{
	if (needUpdate) {
		needUpdate = false;
		pLightBuffer->Update(graphics, lightData);
	}
	pLightBuffer->Bind(graphics);
}

void LightConstantBuffer::SetColor(float r, float g, float b)
{
	needUpdate = true;
	lightData.lightColor[0] = r;
	lightData.lightColor[1] = g;
	lightData.lightColor[2] = b;
}

void LightConstantBuffer::SetPosition(float x, float y, float z, float w)
{
	needUpdate = true;
	lightData.lightPosition[0] = x;
	lightData.lightPosition[1] = y;
	lightData.lightPosition[2] = z;
	lightData.lightPosition[3] = w;
}
