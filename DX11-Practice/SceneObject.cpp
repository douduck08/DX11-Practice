#include "SceneObject.h"

bool SceneObject::IsAttached()
{
	return pNode != nullptr;
}

void SceneObject::SetAttachNode(std::shared_ptr<SceneNode> pNode)
{
	this->pNode = pNode;
}

void SceneObject::SetPosition(float x, float y, float z)
{
	pNode->SetPosition(x, y, z);
}

void SceneObject::SetRotation(float x, float y, float z)
{
	pNode->SetRotation(x, y, z);
}

void SceneObject::SetScale(float x, float y, float z)
{
	pNode->SetScale(x, y, z);
}
