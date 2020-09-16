#pragma once
#include "ResourceManager.h"
#include "Bindable.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Topology.h"
#include "Geometry.h"
#include <memory>

class Mesh : public Bindable, public Resource
{
public:
	Mesh(Graphics& graphics, const std::string& name, std::vector<Geometry::Vertex> vertices, std::vector<unsigned short> indices);
	void Bind(Graphics& graphics) noexcept override;
	UINT GetIndexCount() const noexcept;

	static std::string GetUID(const std::string& name, std::vector<Geometry::Vertex> vertices, std::vector<unsigned short> indices)
	{
		using namespace std::string_literals;
		return typeid(Mesh).name() + "#"s + name;
	}

private:
	std::string name;
	std::shared_ptr<Topology> pTopology;
	std::unique_ptr<VertexBuffer> pVertexBuffer;
	std::unique_ptr<IndexBuffer> pIndexBuffer;
	UINT indexCount = 0;
};