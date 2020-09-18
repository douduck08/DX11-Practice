#include "Light.h"
#include <DirectXMath.h>

Light::Light(Graphics& graphics, LightType type, float r, float g, float b)
	: type(type)
	, lightColor{ 1, 1, 1 }
	, lightPosition{ 0, 0, 0 }
{
	pLightBuffer = std::make_unique<LightConstantBuffer>(graphics);
}

void Light::Bind(Graphics& graphics)
{
	pLightBuffer->Bind(graphics);
}

void Light::SetLightType(LightType newType)
{
	type = newType;
}

void Light::SetColor(float r, float g, float b)
{
	lightColor[0] = r;
	lightColor[1] = g;
	lightColor[2] = b;
	pLightBuffer->SetColor(r, g, b);
}

void Light::SetPosition(float x, float y, float z)
{
	lightPosition[0] = x;
	lightPosition[1] = y;
	lightPosition[2] = z;

	if (type == LightType::Point)
	{
		pLightBuffer->SetPosition(x, y, z, 1);
	}
}

void Light::SetRotation(float x, float y, float z)
{
	lightRotation[0] = x;
	lightRotation[1] = y;
	lightRotation[2] = z;

	if (type == LightType::Directional)
	{
		const float deg2rad = 3.14159265f / 180.0f;
		auto rot = DirectX::XMMatrixRotationRollPitchYaw(lightRotation[0] * deg2rad, lightRotation[1] * deg2rad, lightRotation[2] * deg2rad);
		auto dir = DirectX::XMVector3Transform(DirectX::XMVectorSet(0, 0, 1, 0), rot);
		pLightBuffer->SetPosition(dir.m128_f32[0], dir.m128_f32[1], dir.m128_f32[2], 0);
	}
}
