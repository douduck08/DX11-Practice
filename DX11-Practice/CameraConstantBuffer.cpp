#include "CameraConstantBuffer.h"
#include "SlotConfig.h"

CameraConstantBuffer::CameraConstantBuffer(Graphics& graphics)
	: needUpdate(false)
	, cameraData{}
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

void CameraConstantBuffer::SetProjectMatrix(const DirectX::XMFLOAT4X4& project)
{
	needUpdate = true;
	cameraData.project = project;
}

void CameraConstantBuffer::SetViewMatrix(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& viewProj)
{
	needUpdate = true;
	cameraData.view = view;
	cameraData.viewProject = viewProj;
}

void CameraConstantBuffer::SetPosition(float x, float y, float z)
{
	needUpdate = true;
	cameraData.position[0] = x;
	cameraData.position[1] = y;
	cameraData.position[2] = z;
}

DirectX::XMFLOAT4X4 CameraConstantBuffer::GetProjectMatrix()
{
	return cameraData.project;
}

DirectX::XMFLOAT4X4 CameraConstantBuffer::GetViewMatrix()
{
	return cameraData.view;
}

DirectX::XMFLOAT4X4 CameraConstantBuffer::GetViewProjectMatrix()
{
	return cameraData.viewProject;
}

