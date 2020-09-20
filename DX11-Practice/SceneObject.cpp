#include "SceneObject.h"

bool SceneObject::IsAttached()
{
	return pNode != nullptr;
}

void SceneObject::SetAttachNode(std::shared_ptr<SceneNode> pNode)
{
	this->pNode = pNode;
}
