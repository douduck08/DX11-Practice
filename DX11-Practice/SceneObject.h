#pragma once
#include "SceneNode.h"

class SceneObject
{
	friend class Scene;

public:
	SceneObject() = default;
	bool IsAttached();
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

protected:
	void SetAttachNode(std::shared_ptr<SceneNode> pNode);

protected:
	std::shared_ptr<SceneNode> pNode;
};