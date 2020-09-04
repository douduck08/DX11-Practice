#include "Camera.h"

Camera::Camera(Graphics& graphics, float fovY, float aspectRatio, float nearZ, float farZ)
	: fovY(fovY), aspectRatio(aspectRatio), nearZ(nearZ), farZ(farZ)
{
	matrices.view = DirectX::XMMatrixIdentity();
	matrices.project = DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);

	pCameraBuffer = std::make_unique<SharedConstantBuffer<CameraData>>(graphics, matrices, 0u);
}

void Camera::Bind(Graphics& graphics) noexcept
{
	pCameraBuffer->Update(graphics, matrices);
	pCameraBuffer->Bind(graphics);
}

void Camera::SetCamera(float originX, float originY, float originZ, float radius, float pitch, float yaw, float roll)
{
	using namespace DirectX;

	const auto origin = XMVectorSet(originX, originY, originZ, 1.0f);
	const auto rot = XMMatrixRotationRollPitchYaw(pitch, -yaw, roll);

	const auto up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const auto upVector = XMVector3Transform(up, rot);

	const auto forward = XMVectorSet(0.0f, 0.0f, radius, 0.0f);
	const auto lookVector = XMVector3Transform(forward, rot);
	const auto pos = origin - lookVector;

	matrices.position = pos;
	matrices.view = XMMatrixLookAtLH(pos, origin, upVector);
}
