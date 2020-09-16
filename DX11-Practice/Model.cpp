#include "Model.h"
#include "SlotConfig.h"
#include "Mesh.h"
#include "Material.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

void Model::SetGeometry(Graphics& graphics, Geometry& geometry)
{
	auto pMesh = ResourceManager::Resolve<Mesh>(graphics, "Cube", geometry.vertices, geometry.indices);
	AddSharedBind(pMesh);
	SetIndexCount(pMesh->GetIndexCount());

	auto pTransform = std::make_unique<VertexConstantBuffer<ModelTransform>>(graphics, TRANSFORM_CBUFFER_SLOT);
	pTransformbuffer = pTransform.get();
	AddBind(std::move(pTransform));
}

void Model::SetMesh(Graphics& graphics, const aiMesh& mesh)
{
	std::vector<Geometry::Vertex> vertices;
	std::vector<unsigned short> indices;

	for (unsigned int idx = 0; idx < mesh.mNumVertices; idx++)
	{
		vertices.push_back(
			{
				mesh.mVertices[idx].x, mesh.mVertices[idx].y, mesh.mVertices[idx].z,
				mesh.mNormals[idx].x, mesh.mNormals[idx].y, mesh.mNormals[idx].z,
				mesh.mTextureCoords[0][idx].x, mesh.mTextureCoords[0][idx].y
			}
		);
	}
	for (unsigned int idx = 0; idx < mesh.mNumFaces; idx++)
	{
		const auto& face = mesh.mFaces[idx];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	auto pMesh = ResourceManager::Resolve<Mesh>(graphics, mesh.mName.C_Str(), vertices, indices);
	AddSharedBind(pMesh);
	SetIndexCount(pMesh->GetIndexCount());

	auto pTransform = std::make_unique<VertexConstantBuffer<ModelTransform>>(graphics, TRANSFORM_CBUFFER_SLOT);
	pTransformbuffer = pTransform.get();
	AddBind(std::move(pTransform));
}

void Model::SetMaterial(Graphics& graphics, const aiMaterial& material)
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
		std::string rootPath = "Models/Sponza/";
		auto pTex = ResourceManager::Resolve<TextureView>(graphics, rootPath.append(texFileName.C_Str()), 0);
		pMaterial->AddTextureView(pTex);
	}

	AddSharedBind(pMaterial);
}

Model::Model(Graphics& graphics, Geometry& geometry)
{
	SetGeometry(graphics, geometry);
	AddSharedBind(ResourceManager::Resolve<Material>(
		graphics,
		"SimpleLit",
		"Shaders/SimpleLitVertexShader.cso", "Shaders/SimpleLitPixelShader.cso"
	));
}

Model::Model(Graphics& graphics, const aiMesh& mesh, const aiMaterial& material)
{
	SetMesh(graphics, mesh);
	SetMaterial(graphics, material);
}

void Model::AttachToNode(SceneNode* pNode)
{
	pSceneNode = pNode;
}

void Model::Draw(Graphics& graphics)
{
	ModelTransform data;
	if (pSceneNode) {
		data.model = pSceneNode->GetTransform();
	}
	else {
		DirectX::XMStoreFloat4x4(&(data.model), DirectX::XMMatrixIdentity());
	}

	pTransformbuffer->Update(graphics, data);
	Drawable::Draw(graphics);
}
