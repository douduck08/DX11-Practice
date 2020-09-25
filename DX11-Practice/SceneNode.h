#pragma once
#include <string>
#include <vector>
#include <memory>
#include <DirectXMath.h>

class SceneNode : public std::enable_shared_from_this<SceneNode>
{
	friend class Scene;
	friend class DebugGuiWindow;

public:
	SceneNode(const std::string& name);
	std::shared_ptr<SceneNode> GetPointer();
	std::shared_ptr<SceneNode> CreateChildNode(const std::string& name);

	void RecalculateTransform(DirectX::FXMMATRIX parantTransform);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMFLOAT4X4 GetTransform();

	int RecalculateId(int base);

private:
	void AddChild(std::shared_ptr<SceneNode> pChild);

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