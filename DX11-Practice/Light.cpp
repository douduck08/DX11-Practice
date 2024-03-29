#include "Light.h"
#include "SlotConfig.h"
#include <DirectXMath.h>

Light::Light(Graphics& graphics, LightType type, float r, float g, float b, float intensity, bool castShadow)
	: type(type), lightColor{ r, g, b }, intensity(intensity)
	, castShadow(), shadowData()
{
	SetLightType(graphics, type, castShadow);
}

void Light::SetLightType(Graphics& graphics, LightType type, bool castShadow)
{
	this->castShadow = castShadow && type == LightType::Directional;
	this->type = type;

	if (this->castShadow && pShadowDataBuffer == nullptr)
	{
		const auto size = 2048u;
		shadowData.shadowMapSize.x = static_cast<float>(size);
		shadowData.shadowMapSize.y = static_cast<float>(size);
		shadowData.shadowMapSize.z = 1.0f / shadowData.shadowMapSize.x;
		shadowData.shadowMapSize.w = 1.0f / shadowData.shadowMapSize.y;

		pShadowCamera = std::make_unique<Camera>(graphics);
		pShadowDataBuffer = std::make_unique<SharedConstantBuffer<ShadowData>>(graphics, SHADOW_CBUFFER_SLOT);
		pShadowMap = std::make_unique<ShadowMap>(graphics, size, size, 3u);
	}
}

void Light::SetColor(float r, float g, float b)
{
	lightColor[0] = r;
	lightColor[1] = g;
	lightColor[2] = b;
}

void Light::SetIntensity(float intensity)
{
	this->intensity = intensity;
}

bool Light::IsCastingShadow()
{
	return castShadow;
}

void Light::UpdateShadowData(Camera* camera)
{
	using namespace DirectX;

	const float deg2rad = 3.14159265f / 180.0f;
	auto rotation = pNode->GetRotation();
	auto rot = XMMatrixRotationRollPitchYaw(rotation.x * deg2rad, rotation.y * deg2rad, rotation.z * deg2rad);
	
	const float distance = 200.0f;
	auto lightDir = XMVector3Transform(XMVectorSet(0, 0, 1, 0), rot);
	auto lightPos = 2.0f * distance * lightDir;
	auto targetPos = DirectX::XMVectorSet(0, 0, 0, 1);
	auto up = DirectX::XMVectorSet(0, 1, 0, 0);
	auto view_mt = XMMatrixLookAtLH(lightPos, targetPos, up);

	XMFLOAT3 centerLightSpace;
	XMStoreFloat3(&centerLightSpace, XMVector3TransformCoord(targetPos, view_mt));

	// Ortho frustum in light space encloses scene.
	float l = centerLightSpace.x - distance;
	float b = centerLightSpace.y - distance;
	float n = centerLightSpace.z - distance;
	float r = centerLightSpace.x + distance;
	float t = centerLightSpace.y + distance;
	float f = centerLightSpace.z + distance;
	auto proj_mt = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	pShadowCamera->SetOrthographic(l, r, b, t, n, f);

	XMFLOAT4X4 view;
	DirectX::XMStoreFloat4x4(&view, view_mt);
	pShadowCamera->SetViewMatrix(view);
	//pCameraBuffer->SetProjectMatrix(proj_mt);
	//pCameraBuffer->SetViewMatrix(view_mt);

	XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f
	);
	auto shadow_mt = view_mt * proj_mt * T;
	XMStoreFloat4x4(&shadowData.shadowMatrix, shadow_mt);
}

void Light::BindShadowData(Graphics& graphics)
{
	pShadowCamera->BindCameraBuffer(graphics);
	pShadowDataBuffer->Update(graphics, shadowData);
	pShadowDataBuffer->Bind(graphics);
}

const Frustum& Light::GetShadowCameraFrustum()
{
	return pShadowCamera->GetFrustum();
}

void Light::SetShadowMapAsRenderTarget(Graphics& graphics)
{
	pShadowMap->SetRenderTarget(graphics);
}

void Light::ClearShadowMap(Graphics& graphics)
{
	pShadowMap->Clear(graphics);
}

void Light::BindShadowMap(Graphics& graphics)
{
	pShadowMap->Bind(graphics);
}
