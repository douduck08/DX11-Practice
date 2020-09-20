#pragma once
#include <DirectXMath.h>
#include "SceneObject.h"
#include "LightConstantBuffer.h"
#include <memory>

enum class LightType {
	Point, Directional
};

class Light : public SceneObject
{
	friend class Scene;
	friend class DebugGuiWindow;

public:
	Light(Graphics& graphics, LightType type, float r, float g, float b);
	void SetLightType(LightType newType);
	void SetColor(float r, float g, float b);

private:
	LightType type;
	float lightColor[3];
};
