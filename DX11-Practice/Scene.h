#pragma once
#include <vector>
#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"

class Scene
{
	friend class DebugGuiWindow;

public:
	Scene(Graphics& graphics);
	float* GetBackcolor();
	void Draw(Graphics& graphics);

	void AddModel(std::shared_ptr<SceneNode> pNode, std::unique_ptr<Model> pModel);
	std::shared_ptr<SceneNode> CreateChildSceneNode(const std::string& name);
	void RecalculateId();

private:
	float backcolor[3];

	std::unique_ptr<Camera> pCamera;
	std::unique_ptr<Light> pLight;
	std::shared_ptr<SceneNode> pRootNode;

	std::vector<std::unique_ptr<Model>> pModels;
};