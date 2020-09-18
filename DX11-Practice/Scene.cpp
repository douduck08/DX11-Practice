#include "Scene.h"
#include <DirectXMath.h>
#include "imgui/imgui.h"

Scene::Scene(Graphics& graphics)
	: backcolor{ 0.2f, 0.8f, 0.8f }
{
	pCamera = std::make_unique<Camera>(graphics, 0.3f * 3.1415926f, 1280.f / 720.f, 0.1f, 1000);
	pLight = std::make_unique<Light>(graphics, LightType::Directional, 1.0f, 1.0f, 1.0f);
	pLight->SetPosition(0, 10, 0);
	pRootNode = std::make_unique<SceneNode>("Root");
}

float* Scene::GetBackcolor()
{
	return backcolor;
}

void Scene::Draw(Graphics& graphics)
{
	pCamera->Bind(graphics);
	pLight->Bind(graphics);
	pRootNode->RecalculateTransform(DirectX::XMMatrixIdentity());

	for (auto& m : pModels)
	{
		m->Draw(graphics);
	}
}

void Scene::ShowImguiWindow()
{
	static SceneNode* pSelectedNode;

	ImGui::Begin("Hierarchy");
	ImGui::Columns(2);
	pRootNode->ShowImguiTree(pSelectedNode);

	ImGui::NextColumn();
	if (pSelectedNode != nullptr)
	{
		ImGui::InputFloat3("Position", &(pSelectedNode->position.x));
		ImGui::InputFloat3("Rotation", &(pSelectedNode->rotation.x));
		ImGui::InputFloat3("Scale", &(pSelectedNode->scale.x));
	}
	ImGui::End();

	ImGui::ColorEdit3("Backcolor", backcolor);
	pCamera->ShowImguiWindow();
	pLight->ShowImguiWindow();
}

void Scene::AddModel(std::shared_ptr<SceneNode> pNode, std::unique_ptr<Model> pModel)
{
	pModel->SetAttachNode(pNode);
	pModels.push_back(std::move(pModel));
}

std::shared_ptr<SceneNode> Scene::CreateChildSceneNode(const std::string& name)
{
	return pRootNode->CreateChild(name);
}

void Scene::RecalculateId()
{
	pRootNode->RecalculateId(0);
}

