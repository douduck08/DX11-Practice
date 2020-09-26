#pragma once
#include <DirectXMath.h>

class Frustum
{
	friend class DMath;

public:
	Frustum(float fovY, float aspectRatio, float nearZ, float farZ);
	void UpdatePlanes(const DirectX::XMFLOAT4X4& viewProj);

private:
	float fovY;
	float aspectRatio;
	float nearZ;
	float farZ;

	DirectX::XMFLOAT4 planes[6];
	// TODO: DirectX::XMFLOAT4 points[6];
};