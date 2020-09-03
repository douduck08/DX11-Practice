#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include <DirectXMath.h>
#include "Geometry.h"
#include "Drawable.h"

#include "ConstantBuffer.h"

using Microsoft::WRL::ComPtr;

class Model : public Drawable
{
public:
	Model() = default;
	~Model() = default;

	void SetGeometry(Graphics& graphics, Geometry& geometry);
	void SetShader(Graphics& graphics, const std::wstring& vsFile, const std::wstring& psFile);
	void Draw(Graphics& graphics, float angle);

	void UpdateTransform(float angle, float x, float y, float z) {
		// test code
		transform.objectToClip =
			DirectX::XMMatrixRotationZ(angle) *
			DirectX::XMMatrixRotationX(angle) *
			DirectX::XMMatrixTranslation(x, y, z) *
			DirectX::XMMatrixPerspectiveFovLH(30, 800.f / 600.f, 0.1f, 100);
	}

private:
	struct ModelTransform
	{
		DirectX::XMMATRIX objectToClip;
	};

	ModelTransform transform;
	VertexConstantBuffer<ModelTransform>* pTransformbuffer = nullptr;
};