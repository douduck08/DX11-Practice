#pragma once
#include <vector>
#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "PerFrameConstantBuffer.h"
#include "LightConstantBuffer.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"

class Scene
{
	friend class DebugGuiWindow;

public:
	Scene(Graphics& graphics);
	void Draw(Graphics& graphics);
	std::shared_ptr<SceneNode> GetRootNode();
	void RecalculateNodeId();

	std::shared_ptr<Camera> CreateCamera(Graphics& graphics, const std::string& name);
	std::shared_ptr<Camera> CreateCamera(Graphics& graphics, std::shared_ptr<SceneNode> pParentNode, const std::string& name);
	void SetMainCamera(std::shared_ptr<Camera> pCamera);

	std::shared_ptr<Light> CreateLight(Graphics& graphics, const std::string& name);
	std::shared_ptr<Light> CreateLight(Graphics& graphics, std::shared_ptr<SceneNode> pParentNode, const std::string& name);

	std::shared_ptr<Model> CreateModel(Graphics& graphics, const std::string& name);
	std::shared_ptr<Model> CreateModel(Graphics& graphics, std::shared_ptr<SceneNode> pParentNode, const std::string& name);

private:
	void UpdateLightConstantBuffer(Graphics& graphics);

private:
	float backcolor[3];
	float ambientColor[3];

	std::shared_ptr<Camera> pMainCamera;
	std::shared_ptr<SceneNode> pRootNode;

	std::vector<std::shared_ptr<Camera>> pCameras;
	std::vector<std::shared_ptr<Light>> pLights;
	std::vector<std::shared_ptr<Model>> pModels;

	std::unique_ptr<PerFrameConstantBuffer> pFrameConstantBuffer = nullptr;
	std::unique_ptr<LightConstantBuffer> pLightBuffer = nullptr;

	std::unique_ptr<BlendState> pDefaultState;
	std::unique_ptr<BlendState> pZeroMaskState;
	std::unique_ptr<DepthStencilState> pLessWriteState;
	std::unique_ptr<DepthStencilState> pEqualState;
	std::unique_ptr<RasterizerState> pDefaultRasterizerState;
	std::unique_ptr<RasterizerState> pShadowRasterizerState;
};