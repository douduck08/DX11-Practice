#pragma once
#include "ConstantBuffer.h"
#include <DirectXMath.h>
#include <memory>

class CameraConstantBuffer : public Bindable
{
public:
	CameraConstantBuffer(Graphics& graphics);
	void Bind(Graphics& graphics) noexcept override;
	void SetProjectMatrix(const DirectX::XMFLOAT4X4& project);
	void SetViewMatrix(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& viewProj);
	void SetPosition(float x, float y, float z);
	
	DirectX::XMFLOAT4X4 GetProjectMatrix();
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetViewProjectMatrix();

private:
	struct CameraData
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 project;
		DirectX::XMFLOAT4X4 viewProject;
		float position[4];
	};

	bool needUpdate;
	CameraData cameraData;
	std::unique_ptr<SharedConstantBuffer<CameraData>> pCameraBuffer = nullptr;
};