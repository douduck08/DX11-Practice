#include "Scene.h"
#include <DirectXMath.h>

Scene::Scene(Graphics& graphics)
	: backcolor{ 0.2f, 0.8f, 0.8f }, ambientColor { 0.02f, 0.02f, 0.02f }
{
	pRootNode = std::make_unique<SceneNode>("Root");

	pFrameConstantBuffer = std::make_unique<PerFrameConstantBuffer>(graphics);
	pLightBuffer = std::make_unique<LightConstantBuffer>(graphics);

	pDefaultState = std::make_unique<BlendState>(graphics, BlendState::Mode::Default);
	pZeroMaskState = std::make_unique<BlendState>(graphics, BlendState::Mode::MaskZero);
	pLessWriteState = std::make_unique<DepthStencilState>(graphics, DepthStencilState::Mode::LessWrite);
	pEqualState = std::make_unique<DepthStencilState>(graphics, DepthStencilState::Mode::Equal);
	pDefaultRasterizerState = std::make_unique<RasterizerState>(graphics, RasterizerState::Mode::Default);
	pShadowRasterizerState = std::make_unique<RasterizerState>(graphics, RasterizerState::Mode::Shadow);
}

void Scene::Draw(Graphics& graphics)
{
	// recaculate transform
	pRootNode->RecalculateTransform(DirectX::XMMatrixIdentity());
	
	DirectX::XMFLOAT4 planes[6];
	pMainCamera->GetFrustumPlanes(planes);
	for (auto& m : pModels)
	{
		m->UpdateTransform(graphics);
		m->UpdateVisible(planes);
	}

	// render shadowmap
	auto& mainLight = pLights[0]; // test code
	if (mainLight->IsCastingShadow())
	{
		mainLight->SetShadowMapAsRenderTarget(graphics);
		mainLight->ClearShadowMap(graphics);
		mainLight->CaculateShadowData(pMainCamera.get());
		mainLight->BindShadowData(graphics);

		pZeroMaskState->Bind(graphics);
		pLessWriteState->Bind(graphics);
		pShadowRasterizerState->Bind(graphics);
		for (auto& m : pModels)
		{
			m->Draw(graphics, true);
		}

		graphics.SetNullRenderTarget();
		mainLight->BindShadowMap(graphics);
	}

	// update frame data
	pMainCamera->Bind(graphics);

	pFrameConstantBuffer->SetAmbientColor(ambientColor[0], ambientColor[1], ambientColor[2]);
	pFrameConstantBuffer->Bind(graphics);

	UpdateLightConstantBuffer(graphics);
	pLightBuffer->Bind(graphics);

	// clear buffers
	graphics.SetRenderTarget();
	graphics.ClearBuffer(backcolor[0], backcolor[1], backcolor[2]);
	
	// draw depth
	pZeroMaskState->Bind(graphics);
	pLessWriteState->Bind(graphics);
	pDefaultRasterizerState->Bind(graphics);
	for (auto& m : pModels)
	{
		m->Draw(graphics, true);
	}

	// shading
	pDefaultState->Bind(graphics);
	pEqualState->Bind(graphics);
	pDefaultRasterizerState->Bind(graphics);
	for (auto& m : pModels)
	{
		m->Draw(graphics);
	}
}

std::shared_ptr<SceneNode> Scene::GetRootNode()
{
	return pRootNode;
}

void Scene::RecalculateNodeId()
{
	pRootNode->RecalculateId(0);
}

std::shared_ptr<Camera> Scene::CreateCamera(Graphics& graphics, const std::string& name)
{
	auto pCameraNode = pRootNode->CreateChildNode(name);
	auto pCamera = std::make_shared<Camera>(graphics, 0.3f * 3.1415926f, graphics.GetAspectRatio(), 0.1f, 1000);
	pCamera->SetAttachNode(pCameraNode);
	pCameras.push_back(pCamera);
	return pCamera;
}

std::shared_ptr<Camera> Scene::CreateCamera(Graphics& graphics, std::shared_ptr<SceneNode> pParentNode, const std::string& name)
{
	auto pCameraNode = pParentNode->CreateChildNode(name);
	auto pCamera = std::make_shared<Camera>(graphics, 0.3f * 3.1415926f, graphics.GetAspectRatio(), 0.1f, 1000);
	pCamera->SetAttachNode(pCameraNode);
	pCameras.push_back(pCamera);
	return pCamera;
}

void Scene::SetMainCamera(std::shared_ptr<Camera> pCamera)
{
	pMainCamera = pCamera;
}

std::shared_ptr<Light> Scene::CreateLight(Graphics& graphics, const std::string& name)
{
	auto pLightNode = pRootNode->CreateChildNode(name);
	auto pLight = std::make_shared<Light>(graphics, LightType::Point, 1.0f, 1.0f, 1.0f, 1.0f, false);
	pLight->SetAttachNode(pLightNode);
	pLights.push_back(pLight);
	return pLight;
}

std::shared_ptr<Light> Scene::CreateLight(Graphics& graphics, std::shared_ptr<SceneNode> pParentNode, const std::string& name)
{
	auto pLightNode = pParentNode->CreateChildNode(name);
	auto pLight = std::make_shared<Light>(graphics, LightType::Point, 1.0f, 1.0f, 1.0f, 1.0f, false);
	pLight->SetAttachNode(pLightNode);
	pLights.push_back(pLight);
	return pLight;
}

std::shared_ptr<Model> Scene::CreateModel(Graphics& graphics, const std::string& name)
{
	auto pModelNode = pRootNode->CreateChildNode(name);
	auto pModel = std::make_shared<Model>(graphics);
	pModel->SetAttachNode(pModelNode);
	pModels.push_back(pModel);
	return pModel;
}

std::shared_ptr<Model> Scene::CreateModel(Graphics& graphics, std::shared_ptr<SceneNode> pParentNode, const std::string& name)
{
	auto pModelNode = pParentNode->CreateChildNode(name);
	auto pModel = std::make_shared<Model>(graphics);
	pModel->SetAttachNode(pModelNode);
	pModels.push_back(pModel);
	return pModel;
}

void Scene::UpdateLightConstantBuffer(Graphics& graphics)
{
	int index = 0;
	for (auto& l : pLights)
	{
		if (l->IsAttached())
		{
			if (l->type == LightType::Point)
			{
				auto position = l->pNode->GetPosition();
				pLightBuffer->SetPosition(index, position.x, position.y, position.z, 1);
			}
			else if (l->type == LightType::Directional)
			{
				const float deg2rad = 3.14159265f / 180.0f;
				auto rotation = l->pNode->GetRotation();
				auto rot = DirectX::XMMatrixRotationRollPitchYaw(rotation.x * deg2rad, rotation.y * deg2rad, rotation.z * deg2rad);
				auto dir = DirectX::XMVector3Transform(DirectX::XMVectorSet(0, 0, 1, 0), rot);
				pLightBuffer->SetPosition(index, dir.m128_f32[0], dir.m128_f32[1], dir.m128_f32[2], 0);
			}

			pLightBuffer->SetColor(index, l->lightColor[0], l->lightColor[1], l->lightColor[2], l->intensity);
			index += 1;
		}
	}
	pLightBuffer->SetLightNumber(index);
}

