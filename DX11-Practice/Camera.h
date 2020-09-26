#pragma once
#include <DirectXMath.h>
#include <memory>
#include "SceneObject.h"
#include "Frustum.h"
#include "CameraConstantBuffer.h"

class Camera : public SceneObject
{
	friend class DebugGuiWindow;

public:
	Camera(Graphics& graphics, float fovY, float aspectRatio, float nearZ, float farZ);
	void Bind(Graphics& graphics);
	void SetCameraView(float originX, float originY, float originZ, float radius, float pitch, float yaw, float roll);
	DirectX::XMFLOAT3 GetPosition();

	void UpdateFrustomPlanes();
	const Frustum& GetFrustum();

private:
	void SetCameraView();

private:
	Frustum frustum;
	std::unique_ptr<CameraConstantBuffer> pCameraBuffer = nullptr;
};