#include "Model.h"
#include "SlotConfig.h"

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

static float XMPlaneDotCoord(const DirectX::XMFLOAT4* plane, DirectX::XMVECTOR point)
{
	using namespace DirectX;
	auto dot = XMPlaneDotCoord(XMLoadFloat4(plane), point);
	return dot.m128_f32[0];
}

void Model::UpdateVisible(DirectX::XMFLOAT4 frustumPlanes[6])
{

	if (!IsAttached())
	{
		isVisible = false;
		return;
	}

	for (int i = 0; i < 6; i++)
	{
		int out = 0;
		out += ((XMPlaneDotCoord(&frustumPlanes[i], DirectX::XMVectorSet(bounds.minX, bounds.minY, bounds.minZ, 1.0f)) < 0.0) ? 1 : 0);
		out += ((XMPlaneDotCoord(&frustumPlanes[i], DirectX::XMVectorSet(bounds.maxX, bounds.minY, bounds.minZ, 1.0f)) < 0.0) ? 1 : 0);
		out += ((XMPlaneDotCoord(&frustumPlanes[i], DirectX::XMVectorSet(bounds.minX, bounds.maxY, bounds.minZ, 1.0f)) < 0.0) ? 1 : 0);
		out += ((XMPlaneDotCoord(&frustumPlanes[i], DirectX::XMVectorSet(bounds.maxX, bounds.maxY, bounds.minZ, 1.0f)) < 0.0) ? 1 : 0);
		out += ((XMPlaneDotCoord(&frustumPlanes[i], DirectX::XMVectorSet(bounds.minX, bounds.minY, bounds.maxZ, 1.0f)) < 0.0) ? 1 : 0);
		out += ((XMPlaneDotCoord(&frustumPlanes[i], DirectX::XMVectorSet(bounds.maxX, bounds.minY, bounds.maxZ, 1.0f)) < 0.0) ? 1 : 0);
		out += ((XMPlaneDotCoord(&frustumPlanes[i], DirectX::XMVectorSet(bounds.minX, bounds.maxY, bounds.maxZ, 1.0f)) < 0.0) ? 1 : 0);
		out += ((XMPlaneDotCoord(&frustumPlanes[i], DirectX::XMVectorSet(bounds.maxX, bounds.maxY, bounds.maxZ, 1.0f)) < 0.0) ? 1 : 0);
		if (out == 8) {
			isVisible = false;
			return;
		}
	}

	isVisible = true;
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
