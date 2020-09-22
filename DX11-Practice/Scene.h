#pragma once
#include <vector>
#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "PerFrameConstantBuffer.h"
#include "DepthStencilBuffer.h"
#include "BlendState.h"
#include "DepthStencilState.h"

class Scene
{
	friend class DebugGuiWindow;

public:
	Scene(Graphics& graphics);
	void Draw(Graphics& graphics);
	void AddModel(std::shared_ptr<SceneNode> pNode, std::unique_ptr<Model> pModel);
	void AddLight(std::shared_ptr<SceneNode> pNode, std::unique_ptr<Light> pLight);
	std::shared_ptr<SceneNode> CreateChildSceneNode(const std::string& name);
	void RecalculateId();

private:
	void UpdateLightConstantBuffer(Graphics& graphics);

private:
	float backcolor[3];
	float ambientColor[3];

	std::shared_ptr<SceneNode> pRootNode;
	std::unique_ptr<Camera> pCamera;
	std::vector<std::unique_ptr<Model>> pModels;
	std::vector<std::unique_ptr<Light>> pLights;

	std::unique_ptr<PerFrameConstantBuffer> pFrameConstantBuffer = nullptr;
	std::unique_ptr<LightConstantBuffer> pLightBuffer = nullptr;

	std::unique_ptr<DepthStencilBuffer> pDepthBuffer;
	std::unique_ptr<BlendState> pDefaultState;
	std::unique_ptr<BlendState> pZeroState;
	std::unique_ptr<DepthStencilState> pLessWriteState;
	std::unique_ptr<DepthStencilState> pEqualState;
};