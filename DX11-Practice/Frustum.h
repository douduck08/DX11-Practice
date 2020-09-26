#pragma once
#include <DirectXMath.h>

class Frustum
{
	friend class DMath;

public:
	Frustum() = default;
	void UpdatePlanes(const DirectX::XMFLOAT4X4& viewProj);

private:
	DirectX::XMFLOAT4 planes[6];
	// TODO: DirectX::XMFLOAT4 points[6];
};