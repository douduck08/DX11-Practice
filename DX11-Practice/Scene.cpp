#include "Scene.h"
#include <DirectXMath.h>

Scene::Scene(Graphics& graphics)
	: backcolor{ 0.2f, 0.8f, 0.8f }
{
	pCamera = std::make_unique<Camera>(graphics, 0.3f * 3.1415926f, 1280.f / 720.f, 0.1f, 1000);
	pRootNode = std::make_unique<SceneNode>("Root");

	pLightBuffer = std::make_unique<LightConstantBuffer>(graphics);
}

float* Scene::GetBackcolor()
{
	return backcolor;
}

void Scene::Draw(Graphics& graphics)
{
	pCamera->Bind(graphics);

	UpdateLightConstantBuffer(graphics);
	pLightBuffer->Bind(graphics);
	
	pRootNode->RecalculateTransform(DirectX::XMMatrixIdentity());

	for (auto& m : pModels)
	{
		m->Draw(graphics);
	}
}

void Scene::AddModel(std::shared_ptr<SceneNode> pNode, std::unique_ptr<Model> pModel)
{
	pModel->SetAttachNode(pNode);
	pModels.push_back(std::move(pModel));
}

void Scene::AddLight(std::shared_ptr<SceneNode> pNode, std::unique_ptr<Light> pLight)
{
	pLight->SetAttachNode(pNode);
	pLights.push_back(std::move(pLight));
}

std::shared_ptr<SceneNode> Scene::CreateChildSceneNode(const std::string& name)
{
	return pRootNode->CreateChild(name);
}

void Scene::RecalculateId()
{
	pRootNode->RecalculateId(0);
}

void Scene::UpdateLightConstantBuffer(Graphics& graphics)
{
	int index = 0;
	for (auto& l : pLights)
	{
		if (l->IsAttached())
		{
			if (l->type == LightType::Point)
			{
				auto position = l->pNode->GetPosition();
				pLightBuffer->SetPosition(index, position.x, position.y, position.z, 1);
			}
			else if (l->type == LightType::Directional)
			{
				const float deg2rad = 3.14159265f / 180.0f;
				auto rotation = l->pNode->GetRotation();
				auto rot = DirectX::XMMatrixRotationRollPitchYaw(rotation.x * deg2rad, rotation.y * deg2rad, rotation.z * deg2rad);
				auto dir = DirectX::XMVector3Transform(DirectX::XMVectorSet(0, 0, 1, 0), rot);
				pLightBuffer->SetPosition(index, dir.m128_f32[0], dir.m128_f32[1], dir.m128_f32[2], 0);
			}
			index += 1;
		}
	}
	pLightBuffer->SetLightNumber(index);
}

