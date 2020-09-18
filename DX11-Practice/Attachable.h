#pragma once
#include "SceneNode.h"

class Attachable
{
public:
	Attachable() = default;
	bool IsAttached();
	void SetAttachNode(std::shared_ptr<SceneNode> pNode);

protected:
	std::shared_ptr<SceneNode> pNode;
};