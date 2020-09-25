#pragma once
#include <DirectXMath.h>
#include "SceneObject.h"
#include "Camera.h"
#include "CameraConstantBuffer.h"
#include "ConstantBuffer.h"
#include "ShadowMap.h"
#include <memory>

enum class LightType {
	Point,
	Directional
};

class Light : public SceneObject
{
	friend class Scene;
	friend class DebugGuiWindow;

public:
	Light(Graphics& graphics, LightType type, float r, float g, float b, float intensity = 1, bool castShadow = false);
	void SetLightType(Graphics& graphics, LightType newType, bool castShadow);
	void SetColor(float r, float g, float b);
	void SetIntensity(float intensity);

	bool IsCastingShadow();
	void CaculateShadowData(Camera* camera);
	
	void BindShadowData(Graphics& graphics);
	void BindShadowMap(Graphics& graphics);
	void SetShadowMapAsRenderTarget(Graphics& graphics);
	void ClearShadowMap(Graphics& graphics);

private:
	LightType type;
	float lightColor[3];
	float intensity;

	struct ShadowData
	{
		DirectX::XMFLOAT4X4 shadowMatrix;
		DirectX::XMFLOAT4 shadowMapSize;
	};

	ShadowData shadowData;
	bool castShadow;
	std::unique_ptr<CameraConstantBuffer> pCameraBuffer = nullptr;
	std::unique_ptr<SharedConstantBuffer<ShadowData>> pShadowDataBuffer = nullptr;
	std::unique_ptr<ShadowMap> pShadowMap = nullptr;
};
