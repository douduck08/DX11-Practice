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
	Model(Graphics& graphics, Geometry& geometry, const std::string& vsFile, const std::string& psFile);
	Model(Graphics& graphics, const aiMesh& mesh, const std::string& vsFile, const std::string& psFile);
	~Model() = default;
	
	void AttachToNode(SceneNode* pNode);
	void Draw(Graphics& graphics);

private:
	void SetGeometry(Graphics& graphics, Geometry& geometry);
	void SetMesh(Graphics& graphics, const aiMesh& mesh);
	void SetShader(Graphics& graphics, const std::string& vsFile, const std::string& psFile);

private:
	struct ModelTransform
	{
		DirectX::XMFLOAT4X4 model;
	};

	SceneNode *pSceneNode = nullptr;
	VertexConstantBuffer<ModelTransform>* pTransformbuffer = nullptr;
};