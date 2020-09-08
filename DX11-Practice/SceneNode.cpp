#include "SceneNode.h"
#include "imgui/imgui.h"

SceneNode::SceneNode(const std::string& name)
	: id(0)
	, name(name)
	, position(0, 0, 0)
	, rotation(0, 0, 0)
	, scale(1, 1, 1)
	, transform()
{
}

SceneNode::SceneNode(int id, const std::string& name)
	: id(id)
	, name(name)
	, position(0, 0, 0)
	, rotation(0, 0, 0)
	, scale(1, 1, 1)
	, transform()
{
}

void SceneNode::AddChild(std::unique_ptr<SceneNode> pChild)
{
	children.push_back(std::move(pChild));
}

void SceneNode::RecalculateTransform(DirectX::FXMMATRIX parantTransform)
{
	const float deg2rad = 3.14159265f / 180.0f;

	auto matrix = 
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(rotation.x * deg2rad, rotation.y * deg2rad, rotation.z * deg2rad) *
		DirectX::XMMatrixTranslation(position.x, position.y, position.z) *
		parantTransform;

	DirectX::XMStoreFloat4x4(&transform, matrix);

	for (auto& node : children)
	{
		node->RecalculateTransform(matrix);
	}
}

void SceneNode::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void SceneNode::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

void SceneNode::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

DirectX::XMFLOAT4X4 SceneNode::GetTransform()
{
	return transform;
}

void SceneNode::ShowImguiTree(SceneNode*& pSelectedNode)
{
	const int selectedId = (pSelectedNode == nullptr) ? -1 : pSelectedNode->id;
	const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
		| ((id == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
		| ((children.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);
	
	const auto expanded = ImGui::TreeNodeEx(
		(void*)(intptr_t)id, node_flags, name.c_str()
	);

	if (ImGui::IsItemClicked())
	{
		pSelectedNode = const_cast<SceneNode*>(this);
	}
	
	if (expanded)
	{
		for (const auto& pChild : children)
		{
			pChild->ShowImguiTree(pSelectedNode);
		}
		ImGui::TreePop();
	}
}
