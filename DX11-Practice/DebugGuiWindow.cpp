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
	ImGui::Columns(2);

	std::stack<SceneNode*> stack;
	stack.push(scene.pRootNode.get());
	while (stack.size() > 0)
	{
		auto current = stack.top();
		stack.pop();

		const int selectedId = (pSelectedNode == nullptr) ? -1 : pSelectedNode->id;
		const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
			| ((current->id == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
			| ((current->pChildren.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);

		const auto expanded = ImGui::TreeNodeEx(
			(void*)(intptr_t)current->id, node_flags, (std::to_string(current->id) + ": " + current->name).c_str()
		);

		if (ImGui::IsItemClicked())
		{
			pSelectedNode = current;
		}

		if (expanded)
		{
			for (const auto& pChild : current->pChildren)
			{
				stack.push(pChild.get());;
			}
			ImGui::TreePop();
		}
	}

	ImGui::NextColumn();
	if (pSelectedNode != nullptr)
	{
		ImGui::InputFloat3("Position", &(pSelectedNode->position.x));
		ImGui::InputFloat3("Rotation", &(pSelectedNode->rotation.x));
		ImGui::InputFloat3("Scale", &(pSelectedNode->scale.x));
	}
	ImGui::End();

	ImGui::ColorEdit3("Backcolor", scene.backcolor);

	ShowCameraWindow(*scene.pCamera);
	ShowLightWindow(*scene.pLight);
}

void DebugGuiWindow::ShowCameraWindow(Camera& camera)
{
	const float degree2rad = 3.1415926f / 180.0f;
	static float radius = 10, pitch = 0, yaw = 0, roll = 0;

	ImGui::Text("Camera");
	ImGui::SliderFloat("Radius", &radius, 0.1f, 100.0f);
	ImGui::SliderFloat("Pitch", &pitch, -89.0f, 89.0f);
	ImGui::SliderFloat("Yaw", &yaw, -180.0f, 180.0f);
	ImGui::SliderFloat("Roll", &roll, -180.0f, 180.0f);
	if (ImGui::Button("Reset"))
	{
		radius = 10, pitch = 0, yaw = 0, roll = 0;
	}

	camera.SetCameraView(0, 0, 0, radius, pitch * degree2rad, yaw * degree2rad, roll * degree2rad);
}

void DebugGuiWindow::ShowLightWindow(Light& light)
{
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
}
