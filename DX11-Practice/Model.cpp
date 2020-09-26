#include "Model.h"
#include "SlotConfig.h"
#include "DMath.h"

Model::Model(Graphics& graphics)
{
	pTransformBuffer = std::make_unique<VertexConstantBuffer<ModelTransform>>(graphics, TRANSFORM_CBUFFER_SLOT);
}

void Model::SetGeometry(Graphics& graphics, const std::string& meshName, Geometry& geometry)
{
	SetMesh(ResourceManager::Resolve<Mesh>(graphics, meshName, geometry.vertices, geometry.indices));
	SetMaterial(ResourceManager::Resolve<Material>(
		graphics,
		"DefaultLit",
		"Shaders/LitVertexShader.cso", "Shaders/LitPixelShader.cso"
	));
}

void Model::SetMesh(const std::shared_ptr<Mesh> pMesh)
{
	this->pMesh = pMesh;
	//AddSharedBind(pMesh);
	indexCount = pMesh->GetIndexCount();
}

void Model::SetMaterial(const std::shared_ptr<Material> pMaterial)
{
	this->pMaterial = pMaterial;
	//AddSharedBind(pMaterial);
}

void Model::UpdateTransform(Graphics& graphics)
{
	if (IsAttached()) {
		ModelTransform data;
		data.model = pNode->GetTransform();
		pTransformBuffer->Update(graphics, data);

		bounds = pMesh->GetBounds(data.model);
	}
}

void Model::UpdateVisible(const Frustum& frustum)
{
	if (!IsAttached())
	{
		isVisible = false;
		return;
	}

	isVisible = DMath::BoundsInFrustum(frustum, bounds);
}

void Model::Draw(Graphics& graphics, bool depthMode)
{
	if (isVisible) {
		pTransformBuffer->Bind(graphics);
		for (auto& b : binds)
		{
			b->Bind(graphics);
		}
		for (auto& b : sharedBinds)
		{
			b->Bind(graphics);
		}
		pMesh->Bind(graphics);
		pMaterial->SetDepthModeEnable(depthMode);
		pMaterial->Bind(graphics);
		graphics.DrawIndexed(indexCount);
	}
}

void Model::AddBind(std::unique_ptr<Bindable> bind)
{
	binds.push_back(std::move(bind));
}

void Model::AddSharedBind(std::shared_ptr<Bindable> bind)
{
	sharedBinds.push_back(std::move(bind));
}
