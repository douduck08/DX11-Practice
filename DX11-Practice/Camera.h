#pragma once
#include <DirectXMath.h>
#include <memory>
#include "CameraConstantBuffer.h"

class Camera
{
	friend class DebugGuiWindow;

public:
	Camera(Graphics& graphics, float fovY, float aspectRatio, float nearZ, float farZ);
	void Bind(Graphics& graphics);
	void SetCameraView(float originX, float originY, float originZ, float radius, float pitch, float yaw, float roll);

private:
	float fovY;
	float aspectRatio;
	float nearZ;
	float farZ;

	std::unique_ptr<CameraConstantBuffer> pCameraBuffer = nullptr;
};