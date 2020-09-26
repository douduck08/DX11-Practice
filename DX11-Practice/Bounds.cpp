#include "Bounds.h"
#include <limits>
#include <algorithm>

void Bounds::Reset()
{
	minX = minY = minZ = std::numeric_limits<float>::max();
	maxX = maxY = maxZ = -std::numeric_limits<float>::max();
}

void Bounds::AddPoint(float x, float y, float z)
{
	minX = std::min(minX, x);
	maxX = std::max(maxX, x);
	minY = std::min(minY, y);
	maxY = std::max(maxY, y);
	minZ = std::min(minZ, z);
	maxZ = std::max(maxZ, z);
}

Bounds Bounds::Transform(const DirectX::XMFLOAT4X4& transform)
{
	auto transform_mt = DirectX::XMLoadFloat4x4(&transform);
	DirectX::XMFLOAT3 p;

	Bounds newBounds;
	newBounds.Reset();

	auto point = DirectX::XMVector4Transform(DirectX::XMVectorSet(minX, minY, minZ, 1.0f), transform_mt);
	DirectX::XMStoreFloat3(&p, point);
	newBounds.AddPoint(p.x, p.y, p.z);

	point = DirectX::XMVector4Transform(DirectX::XMVectorSet(maxX, minY, minZ, 1.0f), transform_mt);
	DirectX::XMStoreFloat3(&p, point);
	newBounds.AddPoint(p.x, p.y, p.z);

	point = DirectX::XMVector4Transform(DirectX::XMVectorSet(minX, maxY, minZ, 1.0f), transform_mt);
	DirectX::XMStoreFloat3(&p, point);
	newBounds.AddPoint(p.x, p.y, p.z);

	point = DirectX::XMVector4Transform(DirectX::XMVectorSet(maxX, maxY, minZ, 1.0f), transform_mt);
	DirectX::XMStoreFloat3(&p, point);
	newBounds.AddPoint(p.x, p.y, p.z);

	point = DirectX::XMVector4Transform(DirectX::XMVectorSet(minX, minY, maxZ, 1.0f), transform_mt);
	DirectX::XMStoreFloat3(&p, point);
	newBounds.AddPoint(p.x, p.y, p.z);

	point = DirectX::XMVector4Transform(DirectX::XMVectorSet(maxX, minY, maxZ, 1.0f), transform_mt);
	DirectX::XMStoreFloat3(&p, point);
	newBounds.AddPoint(p.x, p.y, p.z);

	point = DirectX::XMVector4Transform(DirectX::XMVectorSet(minX, maxY, maxZ, 1.0f), transform_mt);
	DirectX::XMStoreFloat3(&p, point);
	newBounds.AddPoint(p.x, p.y, p.z);

	point = DirectX::XMVector4Transform(DirectX::XMVectorSet(maxX, maxY, maxZ, 1.0f), transform_mt);
	DirectX::XMStoreFloat3(&p, point);
	newBounds.AddPoint(p.x, p.y, p.z);

	return newBounds;
}
