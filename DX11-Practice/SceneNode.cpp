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
	pParent.reset();
}

std::shared_ptr<SceneNode> SceneNode::GetPointer()
{
	return shared_from_this();
}

std::shared_ptr<SceneNode> SceneNode::CreateChild(const std::string& name)
{
	auto pChild = std::make_shared<SceneNode>(name);
	AddChild(pChild);
	return pChild;
}

void SceneNode::AddChild(std::shared_ptr<SceneNode> pChild)
{
	pChildren.push_back(pChild);
	pChild->pParent = GetPointer();
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

	for (auto& pChild : pChildren)
	{
		pChild->RecalculateTransform(matrix);
	}
}

int SceneNode::RecalculateId(int base)
{
	id = base;
	for (auto& pChild : pChildren)
	{
		base = pChild->RecalculateId(base + 1);
	}
	return base;
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
		| ((pChildren.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);
	
	const auto expanded = ImGui::TreeNodeEx(
		(void*)(intptr_t)id, node_flags, (std::to_string(id) + ": " + name).c_str()
	);

	if (ImGui::IsItemClicked())
	{
		pSelectedNode = const_cast<SceneNode*>(this);
	}
	
	if (expanded)
	{
		for (const auto& pChild : pChildren)
		{
			pChild->ShowImguiTree(pSelectedNode);
		}
		ImGui::TreePop();
	}
}
