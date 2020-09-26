#include "Camera.h"

Camera::Camera(Graphics& graphics)
{
	pCameraBuffer = std::make_unique<CameraConstantBuffer>(graphics);
}

void Camera::SetPerspective(float fovY, float aspectRatio, float nearZ, float farZ)
{
	mode = Mode::Perspective;
	DirectX::XMFLOAT4X4 proj;
	DirectX::XMStoreFloat4x4(&proj, DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ));
	pCameraBuffer->SetProjectMatrix(proj);
}

void Camera::SetOrthographic(float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ)
{
	mode = Mode::Orthographic;
	DirectX::XMFLOAT4X4 proj;
	DirectX::XMStoreFloat4x4(&proj, DirectX::XMMatrixOrthographicOffCenterLH(viewLeft, viewRight, viewBottom, viewTop, nearZ, farZ));
	pCameraBuffer->SetProjectMatrix(proj);
}

void Camera::BindCameraBuffer(Graphics& graphics)
{
	pCameraBuffer->Bind(graphics);
}

void Camera::SetViewMatrix(DirectX::XMFLOAT4X4 view)
{
	auto view_mt = DirectX::XMLoadFloat4x4(&view);
	auto proj = pCameraBuffer->GetProjectMatrix();
	auto proj_mt = DirectX::XMLoadFloat4x4(&proj);
	DirectX::XMFLOAT4X4 viewProj;
	DirectX::XMStoreFloat4x4(&viewProj, view_mt * proj_mt);
	pCameraBuffer->SetViewMatrix(view, viewProj);
}

void Camera::UpdateTransform()
{
	const auto position = pNode->GetPosition();
	pCameraBuffer->SetPosition(position.x, position.y, position.z);

	const auto transform = pNode->GetTransform();
	auto view_mt = DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&transform));
	auto proj = pCameraBuffer->GetProjectMatrix();
	auto proj_mt = DirectX::XMLoadFloat4x4(&proj);

	DirectX::XMFLOAT4X4 view, viewProj;
	DirectX::XMStoreFloat4x4(&view, view_mt);
	DirectX::XMStoreFloat4x4(&viewProj, view_mt * proj_mt);
	pCameraBuffer->SetViewMatrix(view, viewProj);
}

void Camera::UpdateFrustomPlanes()
{
    DirectX::XMFLOAT4X4 viewProj = pCameraBuffer->GetViewProjectMatrix();
    frustum.UpdatePlanes(viewProj);
}

const Frustum& Camera::GetFrustum()
{
    return frustum;
}
