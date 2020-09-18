#include "Model.h"
#include "SlotConfig.h"

void Model::SetGeometry(Graphics& graphics, Geometry& geometry)
{
	auto pMesh = ResourceManager::Resolve<Mesh>(graphics, "Cube", geometry.vertices, geometry.indices);
	AddSharedBind(pMesh);
	indexCount = pMesh->GetIndexCount();

	auto pTransform = std::make_unique<VertexConstantBuffer<ModelTransform>>(graphics, TRANSFORM_CBUFFER_SLOT);
	pTransformbuffer = pTransform.get();
	AddBind(std::move(pTransform));
}

Model::Model(Graphics& graphics)
{
	auto pTransform = std::make_unique<VertexConstantBuffer<ModelTransform>>(graphics, TRANSFORM_CBUFFER_SLOT);
	pTransformbuffer = pTransform.get();
	AddBind(std::move(pTransform));
}

Model::Model(Graphics& graphics, Geometry& geometry)
{
	SetGeometry(graphics, geometry);
	AddSharedBind(ResourceManager::Resolve<Material>(
		graphics,
		"SimpleLit",
		"Shaders/SimpleLitVertexShader.cso", "Shaders/SimpleLitPixelShader.cso"
	));
}

void Model::SetMesh(const std::shared_ptr<Mesh> pMesh)
{
	AddSharedBind(pMesh);
	indexCount = pMesh->GetIndexCount();
}

void Model::SetMaterial(const std::shared_ptr<Material> pMaterial)
{
	AddSharedBind(pMaterial);
}

void Model::AddBind(std::unique_ptr<Bindable> bind)
{
	binds.push_back(std::move(bind));
}

void Model::AddSharedBind(std::shared_ptr<Bindable> bind)
{
	sharedBinds.push_back(std::move(bind));
}

void Model::Draw(Graphics& graphics)
{
	if (IsAttached()) {
		ModelTransform data;
		data.model = pNode->GetTransform();
		pTransformbuffer->Update(graphics, data);
		
		for (auto& b : binds)
		{
			b->Bind(graphics);
		}
		for (auto& b : sharedBinds)
		{
			b->Bind(graphics);
		}
		graphics.DrawIndexed(indexCount);
	}
}
