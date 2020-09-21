#include "Light.h"
#include <DirectXMath.h>

Light::Light(Graphics& graphics, LightType type, float r, float g, float b)
	: type(type)
	, lightColor{ r, g, b }
{
}

void Light::SetLightType(LightType type)
{
	this->type = type;
}

void Light::SetColor(float r, float g, float b)
{
	lightColor[0] = r;
	lightColor[1] = g;
	lightColor[2] = b;
}
