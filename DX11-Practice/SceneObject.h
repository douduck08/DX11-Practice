#pragma once
#include "SceneNode.h"

class SceneObject
{
public:
	SceneObject() = default;
	bool IsAttached();
	void SetAttachNode(std::shared_ptr<SceneNode> pNode);

protected:
	std::shared_ptr<SceneNode> pNode;
};