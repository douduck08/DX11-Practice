#include "DebugGuiWindow.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"
#include "imgui/imgui.h"
#include <string>

void DebugGuiWindow::ShowResourceWindow()
{
	auto& manager = ResourceManager::Instance();

	ImGui::Begin("ResourceManager");
	auto total = "Resource number: " + std::to_string(manager.resources.size());
	ImGui::Text(total.c_str());

	ImGui::Separator();
	for (auto element : manager.resources)
	{
		auto info = std::to_string(element.second.use_count() - 1) + " usage: " + element.first;
		ImGui::Text(info.c_str());
	}
	ImGui::End();
}

void DebugGuiWindow::ShowSceneWindow(Scene& scene)
{
	static SceneNode* pSelectedNode;
	ShowSceneHierarchy(scene, pSelectedNode);
	ShowSceneProperties(scene, pSelectedNode);
}

void DebugGuiWindow::ShowSceneHierarchy(Scene& scene, SceneNode*& pSelectedNode)
{
	ImGui::Begin("Hierarchy");
	ShowSceneNode(scene.pRootNode.get(), pSelectedNode);
	ImGui::End();
}

void DebugGuiWindow::ShowSceneNode(SceneNode* node, SceneNode*& pSelectedNode)
{
	const int selectedId = (pSelectedNode == nullptr) ? -1 : pSelectedNode->id;
	const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
		| ((node->id == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
		| ((node->pChildren.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);

	const auto expanded = ImGui::TreeNodeEx(
		(void*)(intptr_t)node->id, node_flags, (std::to_string(node->id) + ": " + node->name).c_str()
	);

	if (ImGui::IsItemClicked())
	{
		pSelectedNode = node;
	}

	if (expanded)
	{
		for (const auto& pChild : node->pChildren)
		{
			ShowSceneNode(pChild.get(), pSelectedNode);
		}
		ImGui::TreePop();
	}
}

void DebugGuiWindow::ShowSceneProperties(Scene& scene, SceneNode*& pSelectedNode)
{
	ImGui::Begin("Properties");
	{
		ImGui::Text("Scene");
		ImGui::ColorEdit3("Backcolor", scene.backcolor);
		ImGui::ColorEdit3("Ambient Color", scene.ambientColor);
	}

	{
		ImGui::Text("Active Camera");
		const auto cameraName = scene.pMainCamera->pNode->name;

		if (ImGui::Button(cameraName.c_str()))
		{
			ImGui::OpenPopup("camera_popup");
		}

		if (ImGui::BeginPopup("camera_popup"))
		{
			for (int i = 0; i < scene.pCameras.size(); i++)
			{
				if (ImGui::Selectable(scene.pCameras[i]->pNode->name.c_str()))
				{
					scene.pMainCamera = scene.pCameras[i];
				}
			}
			ImGui::EndPopup();
		}
	}
	
	{
		static int selectedLight = -1;
		const auto lightName = selectedLight == -1 ? "Select Light.." : scene.pLights[selectedLight]->pNode->name;
		
		ImGui::Separator();
		ImGui::Text("Light");
		if (ImGui::Button(lightName.c_str()))
		{
			ImGui::OpenPopup("light_popup");
		}

		if (ImGui::BeginPopup("light_popup"))
		{
			for (int i = 0; i < scene.pLights.size(); i++)
			{
				if (ImGui::Selectable(scene.pLights[i]->pNode->name.c_str()))
				{
					selectedLight = i;
				}
			}
			ImGui::EndPopup();
		}

		if (selectedLight != -1)
		{
			ImGui::ColorEdit3("Color", scene.pLights[selectedLight]->lightColor);
			ImGui::SliderFloat("Intensity", &scene.pLights[selectedLight]->intensity, 0, 5);
		}
		else
		{
			ImGui::NewLine();
			ImGui::NewLine();
		}
	}

	{
		ImGui::Separator();
		ImGui::Text("Selected Node");
		if (pSelectedNode != nullptr)
		{
			ImGui::InputFloat3("Position", &(pSelectedNode->position.x));
			ImGui::InputFloat3("Rotation", &(pSelectedNode->rotation.x));
			ImGui::InputFloat3("Scale", &(pSelectedNode->scale.x));
		}
		else
		{
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::NewLine();
		}
	}
	ImGui::End();
}
