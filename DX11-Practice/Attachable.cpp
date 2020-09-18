#include "Attachable.h"

bool Attachable::IsAttached()
{
	return pNode != nullptr;
}

void Attachable::SetAttachNode(std::shared_ptr<SceneNode> pNode)
{
	this->pNode = pNode;
}
