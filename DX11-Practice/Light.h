#pragma once
#include <DirectXMath.h>
#include <memory>
#include "LightConstantBuffer.h"

class Light
{
public:
	Light(Graphics& graphics, float r, float g, float b);
	void Bind(Graphics& graphics);
	void SetColor(float r, float g, float b);
	void SetPosition(float x, float y, float z);
	void ShowImguiWindow();

private:
	float lightColor[3];
	float lightPosition[3];
	std::unique_ptr<LightConstantBuffer> pLightBuffer = nullptr;
};
