#pragma once
#include <string>
#include <vector>
#include <memory>
#include <DirectXMath.h>

class SceneNode : public std::enable_shared_from_this<SceneNode>
{
	friend class Scene;

public:
	SceneNode(const std::string& name);
	std::shared_ptr<SceneNode> GetPointer();
	std::shared_ptr<SceneNode> CreateChild(const std::string& name);
	void AddChild(std::shared_ptr<SceneNode> pChild);

	void RecalculateTransform(DirectX::FXMMATRIX parantTransform);
	int RecalculateId(int base);
	
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	DirectX::XMFLOAT4X4 GetTransform();
	void ShowImguiTree(SceneNode*& pSelectedNode);

private:
	int id;
	std::string name;
	std::weak_ptr<SceneNode> pParent;
	std::vector<std::shared_ptr<SceneNode>> pChildren;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4X4 transform;
};