#include "CameraConstantBuffer.h"
#include "SlotConfig.h"

CameraConstantBuffer::CameraConstantBuffer(Graphics& graphics)
	: needUpdate(false)
	, cameraData{ DirectX::XMMatrixIdentity(), DirectX::XMMatrixIdentity(), { 0, 0, 0, 1 } }
{
	pCameraBuffer = std::make_unique<SharedConstantBuffer<CameraData>>(graphics, cameraData, CAMERA_CBUFFER_SLOT);
}

void CameraConstantBuffer::Bind(Graphics& graphics) noexcept
{
	if (needUpdate) {
		needUpdate = false;
		pCameraBuffer->Update(graphics, cameraData);
	}
	pCameraBuffer->Bind(graphics);
}

void CameraConstantBuffer::SetViewMatrix(DirectX::XMMATRIX matrix)
{
	needUpdate = true;
	cameraData.view = matrix;
}

void CameraConstantBuffer::SetProjectMatrix(DirectX::XMMATRIX matrix)
{
	needUpdate = true;
	cameraData.project = matrix;
}

void CameraConstantBuffer::SetPosition(float x, float y, float z)
{
	needUpdate = true;
	cameraData.position[0] = x;
	cameraData.position[1] = y;
	cameraData.position[2] = z;
}
