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
	enum class Mode
	{
		Perspective,
		Orthographic,
	};

	Camera(Graphics& graphics);
	void SetPerspective(float fovY, float aspectRatio, float nearZ, float farZ);
	void SetOrthographic(float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ);
	void BindCameraBuffer(Graphics& graphics);
	void SetViewMatrix(DirectX::XMFLOAT4X4 view);
	void UpdateTransform();
	void UpdateFrustomPlanes();
	const Frustum& GetFrustum();

private:
	Mode mode;
	Frustum frustum;
	std::unique_ptr<CameraConstantBuffer> pCameraBuffer = nullptr;
};