#pragma once
#include "ConstantBuffer.h"
#include <memory>

class LightConstantBuffer : public Bindable
{
public:
	LightConstantBuffer(Graphics& graphics);
	void Bind(Graphics& graphics) noexcept override;
	void SetLightNumber(int number);
	void SetColor(int index, float r, float g, float b, float intensity);
	void SetPosition(int index, float x, float y, float z, float w);

private:
	struct LightData
	{
		struct Light
		{
			float color[4];
			float position[4];
		} lights[8];
		int lightNumber;
		int pad0;
		int pad1;
		int pad2;
	};

	bool needUpdate;
	LightData lightData;
	std::unique_ptr<SharedConstantBuffer<LightData>> pLightBuffer = nullptr;
};