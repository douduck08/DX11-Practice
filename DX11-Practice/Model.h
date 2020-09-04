#pragma once
#include <string>
#include <DirectXMath.h>
#include "Graphics.h"
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
	void UpdateTransform(float x, float y, float z, float pitch = 0, float yaw = 0, float roll = 0);

private:
	struct ModelTransform
	{
		DirectX::XMMATRIX objectToWorld;
	};

	ModelTransform transform = { DirectX::XMMatrixIdentity() };
	VertexConstantBuffer<ModelTransform>* pTransformbuffer = nullptr;
};