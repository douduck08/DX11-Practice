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
	~Model() = default;
	
	void SetGeometry(Graphics& graphics, const std::string& meshName, Geometry& geometry);
	void SetMesh(const std::shared_ptr<Mesh> pMesh);
	void SetMaterial(const std::shared_ptr<Material> pMaterial);
	void UpdateTransform(Graphics& graphics);
	void UpdateVisible(DirectX::XMFLOAT4 frustumPlanes[6]);
	void Draw(Graphics& graphics, bool depthMode = false);

private:
	void AddBind(std::unique_ptr<Bindable> bind);
	void AddSharedBind(std::shared_ptr<Bindable> bind);

private:
	struct ModelTransform
	{
		DirectX::XMFLOAT4X4 model;
	};

	UINT indexCount = 0;
	Bounds bounds = {};
	bool isVisible = true;

	std::unique_ptr<VertexConstantBuffer<ModelTransform>> pTransformBuffer = nullptr;
	std::shared_ptr<Mesh> pMesh = nullptr;
	std::shared_ptr<Material> pMaterial = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
	std::vector<std::shared_ptr<Bindable>> sharedBinds;
};