#pragma once
#include <DirectXMath.h>
#include <memory>
#include "CameraConstantBuffer.h"

class Camera
{
public:
	Camera(Graphics& graphics, float fovY, float aspectRatio, float nearZ, float farZ);
	void Bind(Graphics& graphics);
	void SetCameraView(float originX, float originY, float originZ, float radius, float pitch, float yaw, float roll);
	void ShowImguiWindow();

private:
	float fovY;
	float aspectRatio;
	float nearZ;
	float farZ;

	std::unique_ptr<CameraConstantBuffer> pCameraBuffer = nullptr;
};