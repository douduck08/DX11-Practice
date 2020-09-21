#pragma once
#include <DirectXMath.h>
#include <memory>
#include "SceneObject.h"
#include "CameraConstantBuffer.h"

class Camera : public SceneObject
{
	friend class DebugGuiWindow;

public:
	Camera(Graphics& graphics, float fovY, float aspectRatio, float nearZ, float farZ);
	void Bind(Graphics& graphics);
	void SetCameraView(float originX, float originY, float originZ, float radius, float pitch, float yaw, float roll);

private:
	void SetCameraView();

private:
	float fovY;
	float aspectRatio;
	float nearZ;
	float farZ;

	std::unique_ptr<CameraConstantBuffer> pCameraBuffer = nullptr;
};