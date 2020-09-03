#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Bindable.h"
#include "ConstantBuffer.h"

class Camera : Bindable
{
public:
	Camera(Graphics& graphics, float fovY, float aspectRatio, float nearZ, float farZ);
	void Bind(Graphics& graphics) noexcept override;
	void SetCamera(float originX, float originY, float originZ, float radius, float pitch, float yaw, float roll);

private:
	struct CameraMatrices
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX project;
	};

	float fovY;
	float aspectRatio;
	float nearZ;
	float farZ;

	CameraMatrices matrices;
	std::unique_ptr<VertexConstantBuffer<CameraMatrices>> pTransformbuffer = nullptr;
};