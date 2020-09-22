#include "Light.h"
#include <DirectXMath.h>

Light::Light(Graphics& graphics, LightType type, float r, float g, float b, float intensity)
	: type(type), lightColor{ r, g, b }, intensity(intensity)
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

void Light::SetIntensity(float intensity)
{
	this->intensity = intensity;
}
