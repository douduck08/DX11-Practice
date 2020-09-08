#include "Scene.h"
#include "Geometry.h"
#include "imgui/imgui.h"
#include <DirectXMath.h>

Scene::Scene(Graphics& graphics)
	: backcolor{ 0.2f, 0.8f, 0.8f }
{
	pCamera = std::make_unique<Camera>(graphics, 0.3f * 3.1415926f, 1280.f / 720.f, 0.1f, 100);
	pLight = std::make_unique<Light>(graphics, 1.0f, 1.0f, 1.0f);
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

	for (auto& m : models)
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

void Scene::AddModel(std::unique_ptr<Model> model)
{
	models.push_back(std::move(model));
}

void Scene::AddSceneNode(std::unique_ptr<SceneNode> SceneNode)
{
	pRootNode->AddChild(std::move(SceneNode));
}

