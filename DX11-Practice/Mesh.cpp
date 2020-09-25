#include "Mesh.h"

static void AddPoint(Bounds& bounds, float x, float y, float z)
{
	bounds.minX = std::min(bounds.minX, x);
	bounds.maxX = std::max(bounds.maxX, x);
	bounds.minY = std::min(bounds.minY, y);
	bounds.maxY = std::max(bounds.maxY, y);
	bounds.minZ = std::min(bounds.minZ, z);
	bounds.maxZ = std::max(bounds.maxZ, z);
}

Mesh::Mesh(Graphics& graphics, const std::string& name, std::vector<Geometry::Vertex> vertices, std::vector<unsigned short> indices)
	: name(name)
{
	pTopology = ResourceManager::Resolve<Topology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pVertexBuffer = std::make_unique<VertexBuffer>(graphics, vertices);
	pIndexBuffer = std::make_unique<IndexBuffer>(graphics, indices);
	indexCount = pIndexBuffer->GetCount();

	auto it = vertices.begin();
	bounds.minX = bounds.maxX = it->pos.x;
	bounds.minY = bounds.maxY = it->pos.y;
	bounds.minZ = bounds.maxZ = it->pos.z;
	for (++it; it != vertices.end(); ++it)
	{
		AddPoint(bounds, it->pos.x, it->pos.y, it->pos.z);
	}
}

void Mesh::Bind(Graphics& graphics) noexcept
{
	pTopology->Bind(graphics);
	pVertexBuffer->Bind(graphics);
	pIndexBuffer->Bind(graphics);
}

UINT Mesh::GetIndexCount() const noexcept
{
	return indexCount;
}

Bounds Mesh::GetBounds()
{
	return bounds;
}

Bounds Mesh::GetBounds(DirectX::XMFLOAT4X4& transform)
{
	auto transform_mt = DirectX::XMLoadFloat4x4(&transform);
	auto point = DirectX::XMVector4Transform(DirectX::XMVectorSet(bounds.minX, bounds.minY, bounds.minZ, 1.0f), transform_mt);
	
	DirectX::XMFLOAT3 p;
	DirectX::XMStoreFloat3(&p, point);
	Bounds newBounds;
	newBounds.minX = newBounds.maxX = p.x;
	newBounds.minY = newBounds.maxY = p.y;
	newBounds.minZ = newBounds.maxZ = p.z;

	point = DirectX::XMVector4Transform(DirectX::XMVectorSet(bounds.maxX, bounds.minY, bounds.minZ, 1.0f), transform_mt);
	DirectX::XMStoreFloat3(&p, point);
	AddPoint(newBounds, p.x, p.y, p.z);

	point = DirectX::XMVector4Transform(DirectX::XMVectorSet(bounds.minX, bounds.maxY, bounds.minZ, 1.0f), transform_mt);
	DirectX::XMStoreFloat3(&p, point);
	AddPoint(newBounds, p.x, p.y, p.z);

	point = DirectX::XMVector4Transform(DirectX::XMVectorSet(bounds.maxX, bounds.maxY, bounds.minZ, 1.0f), transform_mt);
	DirectX::XMStoreFloat3(&p, point);
	AddPoint(newBounds, p.x, p.y, p.z);

	point = DirectX::XMVector4Transform(DirectX::XMVectorSet(bounds.minX, bounds.minY, bounds.maxZ, 1.0f), transform_mt);
	DirectX::XMStoreFloat3(&p, point);
	AddPoint(newBounds, p.x, p.y, p.z);

	point = DirectX::XMVector4Transform(DirectX::XMVectorSet(bounds.maxX, bounds.minY, bounds.maxZ, 1.0f), transform_mt);
	DirectX::XMStoreFloat3(&p, point);
	AddPoint(newBounds, p.x, p.y, p.z);

	point = DirectX::XMVector4Transform(DirectX::XMVectorSet(bounds.minX, bounds.maxY, bounds.maxZ, 1.0f), transform_mt);
	DirectX::XMStoreFloat3(&p, point);
	AddPoint(newBounds, p.x, p.y, p.z);

	point = DirectX::XMVector4Transform(DirectX::XMVectorSet(bounds.maxX, bounds.maxY, bounds.maxZ, 1.0f), transform_mt);
	DirectX::XMStoreFloat3(&p, point);
	AddPoint(newBounds, p.x, p.y, p.z);

	return newBounds;
}
