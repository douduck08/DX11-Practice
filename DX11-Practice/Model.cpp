#include "Model.h"
#include "SlotConfig.h"



Model::Model(Graphics& graphics)
{
	pTransformBuffer = std::make_unique<VertexConstantBuffer<ModelTransform>>(graphics, TRANSFORM_CBUFFER_SLOT);
}

Model::Model(Graphics& graphics, const std::string& meshName, Geometry& geometry)
{
	pTransformBuffer = std::make_unique<VertexConstantBuffer<ModelTransform>>(graphics, TRANSFORM_CBUFFER_SLOT);
	SetGeometry(graphics, meshName, geometry);
	SetMaterial(ResourceManager::Resolve<Material>(
		graphics,
		"DefaultLit",
		"Shaders/LitVertexShader.cso", "Shaders/LitPixelShader.cso"
	));
}

void Model::SetGeometry(Graphics& graphics, const std::string& meshName, Geometry& geometry)
{
	auto pMesh = ResourceManager::Resolve<Mesh>(graphics, meshName, geometry.vertices, geometry.indices);
	AddSharedBind(pMesh);
	indexCount = pMesh->GetIndexCount();
}

void Model::SetMesh(const std::shared_ptr<Mesh> pMesh)
{
	AddSharedBind(pMesh);
	indexCount = pMesh->GetIndexCount();
}

void Model::SetMaterial(const std::shared_ptr<Material> pMaterial)
{
	this->pMaterial = pMaterial;
	//AddSharedBind(pMaterial);
}

void Model::AddBind(std::unique_ptr<Bindable> bind)
{
	binds.push_back(std::move(bind));
}

void Model::AddSharedBind(std::shared_ptr<Bindable> bind)
{
	sharedBinds.push_back(std::move(bind));
}

void Model::Draw(Graphics& graphics, bool depthMode)
{
	if (IsAttached()) {
		ModelTransform data;
		data.model = pNode->GetTransform();
		pTransformBuffer->Update(graphics, data);
		pTransformBuffer->Bind(graphics);
		
		for (auto& b : binds)
		{
			b->Bind(graphics);
		}
		for (auto& b : sharedBinds)
		{
			b->Bind(graphics);
		}

		pMaterial->SetDepthModeEnable(depthMode);
		pMaterial->Bind(graphics);
		graphics.DrawIndexed(indexCount);
	}
}
