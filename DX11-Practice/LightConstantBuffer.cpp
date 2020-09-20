#include "LightConstantBuffer.h"
#include "SlotConfig.h"

LightConstantBuffer::LightConstantBuffer(Graphics& graphics)
	: needUpdate(false)
	, lightData()
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

void LightConstantBuffer::SetLightNumber(int number)
{
	needUpdate = true;
	lightData.lightNumber = number;
}

void LightConstantBuffer::SetColor(int index, float r, float g, float b)
{
	needUpdate = true;
	lightData.lights[index].color[0] = r;
	lightData.lights[index].color[1] = g;
	lightData.lights[index].color[2] = b;
}

void LightConstantBuffer::SetPosition(int index, float x, float y, float z, float w)
{
	needUpdate = true;
	lightData.lights[index].position[0] = x;
	lightData.lights[index].position[1] = y;
	lightData.lights[index].position[2] = z;
	lightData.lights[index].position[3] = w;
}
