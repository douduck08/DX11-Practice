#include "Light.h"

Light::Light(Graphics& graphics, float r, float g, float b)
{
	lightData.lightColor = { r, g, b, 1.0f };
	lightData.lightPosition = { 0.0f, 0.0f, 0.0f, 1.0f };

	pLightBuffer = std::make_unique<SharedConstantBuffer<LightData>>(graphics, lightData, 2u);
}

void Light::Bind(Graphics& graphics) noexcept
{
	pLightBuffer->Update(graphics, lightData);
	pLightBuffer->Bind(graphics);
}

void Light::SetColor(float r, float g, float b)
{
	lightData.lightColor = { r, g, b, 1.0f };
}

void Light::SetPosition(float x, float y, float z)
{
	lightData.lightPosition = { x, y, z, 1.0f };
}
