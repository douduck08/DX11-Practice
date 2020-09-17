#pragma once
#include <vector>
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "SceneNode.h"

class Scene
{
public:
	Scene(Graphics& graphics);
	float* GetBackcolor();
	void Draw(Graphics& graphics);
	void ShowImguiWindow();

	void AddModel(std::unique_ptr<Model> model);
	std::shared_ptr<SceneNode> CreateChildSceneNode(const std::string& name);
	void RecalculateId();

private:
	float backcolor[3];

	std::unique_ptr<Camera> pCamera;
	std::unique_ptr<Light> pLight;
	std::shared_ptr<SceneNode> pRootNode;

	std::vector<std::unique_ptr<Model>> models;
};