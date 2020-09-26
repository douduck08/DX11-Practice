#pragma once
#include <DirectXMath.h>

class Bounds
{
	friend class DMath;

public:
	Bounds() = default;
	void Reset();
	void AddPoint(float x, float y, float z);
	Bounds Transform(const DirectX::XMFLOAT4X4& transform);

private:
	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;
};