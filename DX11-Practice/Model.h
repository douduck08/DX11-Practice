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
	Model(Graphics& graphics, Geometry& geometry, const std::wstring& vsFile, const std::wstring& psFile);
	Model(Graphics& graphics, const aiMesh& mesh, const std::wstring& vsFile, const std::wstring& psFile);
	~Model() = default;
	
	void AttachToNode(SceneNode* pNode);
	void Draw(Graphics& graphics);

private:
	void SetGeometry(Graphics& graphics, Geometry& geometry);
	void SetMesh(Graphics& graphics, const aiMesh& mesh);
	void SetShader(Graphics& graphics, const std::wstring& vsFile, const std::wstring& psFile);

private:
	struct ModelTransform
	{
		DirectX::XMFLOAT4X4 model;
	};

	SceneNode *pSceneNode = nullptr;
	VertexConstantBuffer<ModelTransform>* pTransformbuffer = nullptr;
};