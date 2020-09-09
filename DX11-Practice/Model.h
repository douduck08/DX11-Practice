#pragma once
#include <string>
#include <DirectXMath.h>
#include <memory>
#include "Graphics.h"
#include "Geometry.h"
#include "Drawable.h"
#include "BindableResources.h"
#include "SceneNode.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using Microsoft::WRL::ComPtr;

class Model : public Drawable
{
public:
	Model() = default;
	~Model() = default;

	void SetGeometry(Graphics& graphics, Geometry& geometry);
	void SetMesh(Graphics& graphics, const aiMesh& mesh);
	void SetShader(Graphics& graphics, const std::wstring& vsFile, const std::wstring& psFile);
	void SetSceneNode(SceneNode* pNode);
	void Draw(Graphics& graphics);

private:
	struct ModelTransform
	{
		DirectX::XMFLOAT4X4 model;
	};

	SceneNode *pSceneNode = nullptr;
	VertexConstantBuffer<ModelTransform>* pTransformbuffer = nullptr;
};