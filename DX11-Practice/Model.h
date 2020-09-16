#pragma once
#include <string>
#include <DirectXMath.h>
#include "Geometry.h"
#include "Drawable.h"
#include "ConstantBuffer.h"
#include "SceneNode.h"
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using Microsoft::WRL::ComPtr;

class Model : public Drawable
{
public:
	Model(Graphics& graphics, Geometry& geometry);
	Model(Graphics& graphics, const aiMesh& mesh, const aiMaterial& material);
	~Model() = default;
	
	void AttachToNode(SceneNode* pNode);
	void Draw(Graphics& graphics);

private:
	void SetGeometry(Graphics& graphics, Geometry& geometry);
	void SetMesh(Graphics& graphics, const aiMesh& mesh);
	void SetMaterial(Graphics& graphics, const aiMaterial& material);

private:
	struct ModelTransform
	{
		DirectX::XMFLOAT4X4 model;
	};

	SceneNode *pSceneNode = nullptr;
	VertexConstantBuffer<ModelTransform>* pTransformbuffer = nullptr;
};