#include "Mesh.h"

Mesh::Mesh(Graphics& graphics, const std::string& name, std::vector<Geometry::Vertex> vertices, std::vector<unsigned short> indices)
	: name(name)
{
	pVertexBuffer = std::make_unique<VertexBuffer>(graphics, vertices);
	pIndexBuffer = std::make_unique<IndexBuffer>(graphics, indices);
	indexCount = pIndexBuffer->GetCount();
}

void Mesh::Bind(Graphics& graphics) noexcept
{
	pVertexBuffer->Bind(graphics);
	pIndexBuffer->Bind(graphics);
}

UINT Mesh::GetIndexCount() const noexcept
{
	return indexCount;
}
