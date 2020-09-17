#include "Model.h"
#include "SlotConfig.h"

void Model::SetGeometry(Graphics& graphics, Geometry& geometry)
{
	auto pMesh = ResourceManager::Resolve<Mesh>(graphics, "Cube", geometry.vertices, geometry.indices);
	AddSharedBind(pMesh);
	SetIndexCount(pMesh->GetIndexCount());

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
	SetIndexCount(pMesh->GetIndexCount());
}

void Model::SetMaterial(const std::shared_ptr<Material> pMaterial)
{
	AddSharedBind(pMaterial);
}

void Model::AttachToNode(SceneNode* pNode)
{
	pSceneNode = pNode;
}

void Model::Draw(Graphics& graphics)
{
	ModelTransform data;
	if (pSceneNode) {
		data.model = pSceneNode->GetTransform();
	}
	else {
		DirectX::XMStoreFloat4x4(&(data.model), DirectX::XMMatrixIdentity());
	}

	pTransformbuffer->Update(graphics, data);
	Drawable::Draw(graphics);
}
