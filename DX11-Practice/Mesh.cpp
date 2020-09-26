#include "Mesh.h"

Mesh::Mesh(Graphics& graphics, const std::string& name, std::vector<Geometry::Vertex> vertices, std::vector<unsigned short> indices)
	: name(name)
{
	pTopology = ResourceManager::Resolve<Topology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pVertexBuffer = std::make_unique<VertexBuffer>(graphics, vertices);
	pIndexBuffer = std::make_unique<IndexBuffer>(graphics, indices);
	indexCount = pIndexBuffer->GetCount();

	bounds.Reset();
	for (auto& vert : vertices)
	{
		bounds.AddPoint(vert.pos.x, vert.pos.y, vert.pos.z);
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

Bounds Mesh::GetBounds(const DirectX::XMFLOAT4X4& transform)
{
	return bounds.Transform(transform);
}
