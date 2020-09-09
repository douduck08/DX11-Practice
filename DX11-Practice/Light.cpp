#include "Light.h"
#include <DirectXMath.h>
#include "imgui/imgui.h"

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

void Light::ShowImguiWindow()
{
	ImGui::Text("Light");
	auto isPoint = type == LightType::Point;
	if (ImGui::Checkbox("Point", &isPoint))
	{
		SetLightType(isPoint ? LightType::Point : LightType::Directional);
	}
	if(ImGui::ColorEdit3("Color", lightColor))
	{
		pLightBuffer->SetColor(lightColor[0], lightColor[1], lightColor[2]);
	}
	if (ImGui::InputFloat3("Position", lightPosition))
	{
		SetPosition(lightPosition[0], lightPosition[1], lightPosition[2]);
	}

	if (ImGui::InputFloat3("Rotation", lightRotation))
	{
		SetRotation(lightRotation[0], lightRotation[1], lightRotation[2]);
	}
}
