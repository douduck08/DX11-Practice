#pragma once
#include "ConstantBuffer.h"
#include <DirectXMath.h>
#include <memory>

class CameraConstantBuffer : public Bindable
{
public:
	CameraConstantBuffer(Graphics& graphics);
	void Bind(Graphics& graphics) noexcept override;
	void SetViewMatrix(DirectX::XMMATRIX matrix);
	void SetProjectMatrix(DirectX::XMMATRIX matrix);
	void SetPosition(float x, float y, float z);

private:
	struct CameraData
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX project;
		float position[4];
	};

	bool needUpdate;
	CameraData cameraData;
	std::unique_ptr<SharedConstantBuffer<CameraData>> pCameraBuffer = nullptr;
};