#include "Camera.h"

Camera::Camera(Graphics& graphics, float fovY, float aspectRatio, float nearZ, float farZ)
	: frustum(fovY, aspectRatio, nearZ, farZ)
{
	pCameraBuffer = std::make_unique<CameraConstantBuffer>(graphics);
	pCameraBuffer->SetProjectMatrix(DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ));
}

void Camera::Bind(Graphics& graphics)
{
	SetCameraView();
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

DirectX::XMFLOAT3 Camera::GetPosition()
{
	return pNode->GetPosition();
}

void Camera::UpdateFrustomPlanes()
{
    DirectX::XMFLOAT4X4 viewProj;
    DirectX::XMStoreFloat4x4(&viewProj, pCameraBuffer->GetViewProjectMatrix());
    frustum.UpdatePlanes(viewProj);
}

const Frustum& Camera::GetFrustum()
{
    return frustum;
}

void Camera::SetCameraView()
{
	const auto position = pNode->GetPosition();
	pCameraBuffer->SetPosition(position.x, position.y, position.z);

	const auto transform = pNode->GetTransform();
	auto transform_mt = DirectX::XMLoadFloat4x4(&transform);
	auto view_mt = DirectX::XMMatrixInverse(nullptr, transform_mt);
	pCameraBuffer->SetViewMatrix(view_mt);
}
