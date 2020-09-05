#include "Light.h"
#include "imgui/imgui.h"

Light::Light(Graphics& graphics, float r, float g, float b)
	: lightColor{ 1, 1, 1 }
	, lightPosition{ 0, 0, 0 }
{
	pLightBuffer = std::make_unique<LightConstantBuffer>(graphics);
}

void Light::Bind(Graphics& graphics)
{
	pLightBuffer->Bind(graphics);
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
	pLightBuffer->SetPosition(x, y, z);
}

void Light::ShowImguiWindow()
{
	ImGui::Text("Light");
	if(ImGui::ColorEdit3("Color", lightColor))
	{
		pLightBuffer->SetColor(lightColor[0], lightColor[1], lightColor[2]);
	}
	if (ImGui::InputFloat3("Position", lightPosition))
	{
		pLightBuffer->SetPosition(lightPosition[0], lightPosition[1], lightPosition[2]);
	}
}
