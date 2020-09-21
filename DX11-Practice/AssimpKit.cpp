#include "AssimpKit.h"
#include "Model.h"
#include <filesystem>

#pragma comment(lib,"assimp-vc140-mt.lib")

std::shared_ptr<SceneNode> AssimpKit::LoadModelFromFile(Graphics& graphics, Scene& targetScene, const std::string& name, const std::string& filePath)
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	const std::string rootPath = std::filesystem::path(filePath).parent_path().string() + "\\";
	if (pScene != nullptr) {
		auto pParentNode = targetScene.CreateChildSceneNode(name);
		for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
		{
			const auto& mesh = pScene->mMeshes[i];
			const auto& material = pScene->mMaterials[mesh->mMaterialIndex];
			
			auto model = std::make_unique<Model>(graphics);
			model->SetMesh(AssimpKit::ParseMesh(graphics, *mesh));
			model->SetMaterial(AssimpKit::ParseMaterial(graphics, *material, rootPath));

			auto pChildNode = pParentNode->CreateChild(mesh->mName.C_Str());
			targetScene.AddModel(pChildNode, std::move(model));
		}

		return pParentNode;
	}
	return nullptr;
}

std::shared_ptr<Mesh> AssimpKit::ParseMesh(Graphics& graphics, const aiMesh& mesh)
{
	std::vector<Geometry::Vertex> vertices;
	std::vector<unsigned short> indices;

	for (unsigned int idx = 0; idx < mesh.mNumVertices; idx++)
	{
		if (mesh.HasTangentsAndBitangents())
		{
			vertices.push_back(
				{
					mesh.mVertices[idx].x, mesh.mVertices[idx].y, mesh.mVertices[idx].z,
					mesh.mNormals[idx].x, mesh.mNormals[idx].y, mesh.mNormals[idx].z,
					mesh.mTextureCoords[0][idx].x, mesh.mTextureCoords[0][idx].y,
					mesh.mTangents[idx].x, mesh.mTangents[idx].y, mesh.mTangents[idx].z,
					mesh.mBitangents[idx].x, mesh.mBitangents[idx].y, mesh.mBitangents[idx].z,
				}
			);
		}
		else
		{
			vertices.push_back(
				{
					mesh.mVertices[idx].x, mesh.mVertices[idx].y, mesh.mVertices[idx].z,
					mesh.mNormals[idx].x, mesh.mNormals[idx].y, mesh.mNormals[idx].z,
					mesh.mTextureCoords[0][idx].x, mesh.mTextureCoords[0][idx].y,
					0, 0, 0,
					0, 0, 0,
				}
			);
		}
	}
	for (unsigned int idx = 0; idx < mesh.mNumFaces; idx++)
	{
		const auto& face = mesh.mFaces[idx];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	auto pMesh = ResourceManager::Resolve<Mesh>(graphics, mesh.mName.C_Str(), vertices, indices);
	return pMesh;
}

std::shared_ptr<Material> AssimpKit::ParseMaterial(Graphics& graphics, const aiMaterial& material, const std::string& fileRootPath)
{
	aiString name;
	material.Get(AI_MATKEY_NAME, name);

	auto pMaterial = ResourceManager::Resolve<Material>(
		graphics,
		name.C_Str(),
		"Shaders/LitVertexShader.cso", "Shaders/LitPixelShader.cso"
	);

	aiString texFileName;
	if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
	{
		auto pTex = ResourceManager::Resolve<TextureView>(graphics, fileRootPath + texFileName.C_Str(), 0);
		pMaterial->AddTextureView(pTex);
		pMaterial->SetDiffuseMapEnable();
	}
	if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
	{
		auto pTex = ResourceManager::Resolve<TextureView>(graphics, fileRootPath + texFileName.C_Str(), 1);
		pMaterial->AddTextureView(pTex);
		pMaterial->SetSpecularMapEnable();
	}
	if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
	{
		auto pTex = ResourceManager::Resolve<TextureView>(graphics, fileRootPath + texFileName.C_Str(), 2);
		pMaterial->AddTextureView(pTex);
		pMaterial->SetNormalMapEnable();
	}

	return pMaterial;
}
