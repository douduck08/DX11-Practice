#pragma once
#include <string>
#include <DirectXMath.h>
#include "SceneObject.h"
#include "ConstantBuffer.h"
#include "Mesh.h"
#include "Material.h"
#include "Geometry.h"
#include <memory>

class Model : public SceneObject
{
public:
	Model(Graphics& graphics);
	Model(Graphics& graphics, Geometry& geometry);
	~Model() = default;
	
	void SetMesh(const std::shared_ptr<Mesh> pMesh);
	void SetMaterial(const std::shared_ptr<Material> pMaterial);
	void Draw(Graphics& graphics);

private:
	void SetGeometry(Graphics& graphics, Geometry& geometry);
	void AddBind(std::unique_ptr<Bindable> bind);
	void AddSharedBind(std::shared_ptr<Bindable> bind);

private:
	struct ModelTransform
	{
		DirectX::XMFLOAT4X4 model;
	};

	UINT indexCount = 0;
	std::unique_ptr<VertexConstantBuffer<ModelTransform>> pTransformBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
	std::vector<std::shared_ptr<Bindable>> sharedBinds;
};