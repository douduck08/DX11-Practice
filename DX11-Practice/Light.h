#pragma once
#include <DirectXMath.h>
#include <memory>
#include "LightConstantBuffer.h"

enum class LightType {
	Point, Directional
};

class Light
{
	friend class DebugGuiWindow;

public:
	Light(Graphics& graphics, LightType type, float r, float g, float b);
	void Bind(Graphics& graphics);
	void SetLightType(LightType newType);
	void SetColor(float r, float g, float b);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

private:
	LightType type;
	float lightColor[3];
	float lightPosition[3];
	float lightRotation[3];
	std::unique_ptr<LightConstantBuffer> pLightBuffer = nullptr;
};
