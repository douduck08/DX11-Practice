#include "Scene.h"
#include <DirectXMath.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "imgui/imgui.h"

#pragma comment(lib,"assimp-vc140-mt.lib")

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

void Scene::LoadModelFromFile(Graphics& graphics, const std::string name, const std::string filePath)
{
	
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals
	);


	if (pScene != nullptr) {
		auto rootNode = std::make_unique<SceneNode>(name);
		for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
		{
			const auto& mesh = pScene->mMeshes[i];
			const auto& material = pScene->mMaterials[mesh->mMaterialIndex];
			auto model = std::make_unique<Model>(graphics, *mesh, *material);
			
			auto childNode = std::make_unique<SceneNode>(mesh->mName.C_Str());
			model->AttachToNode(childNode.get());
			rootNode->AddChild(std::move(childNode));
			this->AddModel(std::move(model));
		}
		this->AddSceneNode(std::move(rootNode));
	}
}

void Scene::RecalculateId()
{
	pRootNode->RecalculateId(0);
}

