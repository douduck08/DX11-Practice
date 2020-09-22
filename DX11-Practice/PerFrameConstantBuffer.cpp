#include "PerFrameConstantBuffer.h"
#include "SlotConfig.h"

PerFrameConstantBuffer::PerFrameConstantBuffer(Graphics& graphics)
	: needUpdate(false) , perFrameData()
{
	pPerFrameDataBuffer = std::make_unique<SharedConstantBuffer<PerFrameData>>(graphics, PER_FRAME_CBUFFER_SLOT);
}

void PerFrameConstantBuffer::Bind(Graphics& graphics) noexcept
{
	if (needUpdate) {
		needUpdate = false;
		pPerFrameDataBuffer->Update(graphics, perFrameData);
	}
	pPerFrameDataBuffer->Bind(graphics);
}

void PerFrameConstantBuffer::SetAmbientColor(float r, float g, float b)
{
	needUpdate = true;
	perFrameData.ambientColor[0] = r;
	perFrameData.ambientColor[1] = g;
	perFrameData.ambientColor[2] = b;
}
