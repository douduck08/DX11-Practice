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
	void AddLight(std::shared_ptr<SceneNode> pNode, std::unique_ptr<Light> pLight);
	std::shared_ptr<SceneNode> CreateChildSceneNode(const std::string& name);
	void RecalculateId();

private:
	void UpdateLightConstantBuffer(Graphics& graphics);

private:
	float backcolor[3];

	std::unique_ptr<Camera> pCamera;
	std::shared_ptr<SceneNode> pRootNode;

	std::vector<std::unique_ptr<Model>> pModels;
	std::vector<std::unique_ptr<Light>> pLights;

	std::unique_ptr<LightConstantBuffer> pLightBuffer = nullptr;
};