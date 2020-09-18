#include "Camera.h"

Camera::Camera(Graphics& graphics, float fovY, float aspectRatio, float nearZ, float farZ)
	: fovY(fovY), aspectRatio(aspectRatio), nearZ(nearZ), farZ(farZ)
{
	pCameraBuffer = std::make_unique<CameraConstantBuffer>(graphics);
	pCameraBuffer->SetProjectMatrix(DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ));
}

void Camera::Bind(Graphics& graphics)
{
	pCameraBuffer->Bind(graphics);
}

void Camera::SetCameraView(float originX, float originY, float originZ, float radius, float pitch, float yaw, float roll)
{
	using namespace DirectX;

	const auto origin = XMVectorSet(originX, originY, originZ, 1.0f);
	const auto rot = XMMatrixRotationRollPitchYaw(pitch, -yaw, roll);

	const auto up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const auto upVector = XMVector3Transform(up, rot);

	const auto forward = XMVectorSet(0.0f, 0.0f, radius, 0.0f);
	const auto lookVector = XMVector3Transform(forward, rot);
	const auto pos = origin - lookVector;

	pCameraBuffer->SetPosition(pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2]);
	pCameraBuffer->SetViewMatrix(XMMatrixLookAtLH(pos, origin, upVector));
}