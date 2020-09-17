#pragma once
#include <string>
#include <DirectXMath.h>
#include "Geometry.h"
#include "Drawable.h"
#include "ConstantBuffer.h"
#include "SceneNode.h"
#include "Mesh.h"
#include "Material.h"
#include <memory>

class Model : public Drawable
{
public:
	Model(Graphics& graphics);
	Model(Graphics& graphics, Geometry& geometry);
	~Model() = default;
	
	void SetMesh(const std::shared_ptr<Mesh> pMesh);
	void SetMaterial(const std::shared_ptr<Material> pMaterial);
	void AttachToNode(SceneNode* pNode);
	void Draw(Graphics& graphics);

private:
	void SetGeometry(Graphics& graphics, Geometry& geometry);

private:
	struct ModelTransform
	{
		DirectX::XMFLOAT4X4 model;
	};

	SceneNode *pSceneNode = nullptr;
	VertexConstantBuffer<ModelTransform>* pTransformbuffer = nullptr;
};