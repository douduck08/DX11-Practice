#include "DebugGuiWindow.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"
#include "imgui/imgui.h"
#include <stack>

void DebugGuiWindow::ShowSceneHierarchy(Scene& scene)
{
	static SceneNode* pSelectedNode;

	ImGui::Begin("Hierarchy");
	ShowSceneNode(scene.pRootNode.get(), pSelectedNode);
	ImGui::End();

	ImGui::Begin("Properties");
	ImGui::Text("Scene Properties");
	ImGui::ColorEdit3("Backcolor", scene.backcolor);
	ImGui::ColorEdit3("Ambient Color", scene.ambientColor);

	ImGui::Text("Node Properties");
	if (pSelectedNode != nullptr)
	{
		ImGui::InputFloat3("Position", &(pSelectedNode->position.x));
		ImGui::InputFloat3("Rotation", &(pSelectedNode->rotation.x));
		ImGui::InputFloat3("Scale", &(pSelectedNode->scale.x));
	}
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

void DebugGuiWindow::ShowCameraWindow(Camera& camera)
{
	
}

void DebugGuiWindow::ShowLightWindow(Light& light)
{
	/*
	ImGui::Text("Light");
	auto isPoint = light.type == LightType::Point;
	if (ImGui::Checkbox("Point", &isPoint))
	{
		light.SetLightType(isPoint ? LightType::Point : LightType::Directional);
	}
	if (ImGui::ColorEdit3("Color", light.lightColor))
	{
		light.pLightBuffer->SetColor(light.lightColor[0], light.lightColor[1], light.lightColor[2]);
	}
	if (ImGui::InputFloat3("Position", light.lightPosition))
	{
		light.SetPosition(light.lightPosition[0], light.lightPosition[1], light.lightPosition[2]);
	}

	if (ImGui::InputFloat3("Rotation", light.lightRotation))
	{
		light.SetRotation(light.lightRotation[0], light.lightRotation[1], light.lightRotation[2]);
	}
	*/
}


