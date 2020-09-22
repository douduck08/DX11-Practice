#pragma once
#include "ConstantBuffer.h"
#include <memory>

class PerFrameConstantBuffer : public Bindable
{
public:
	PerFrameConstantBuffer(Graphics& graphics);
	void Bind(Graphics& graphics) noexcept override;
	void SetAmbientColor(float r, float g, float b);

private:
	struct PerFrameData
	{
		float ambientColor[4];
	};

	bool needUpdate;
	PerFrameData perFrameData;
	std::unique_ptr<SharedConstantBuffer<PerFrameData>> pPerFrameDataBuffer = nullptr;
};