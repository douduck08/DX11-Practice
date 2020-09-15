#include "Model.h"
#include "SlotConfig.h"
#include "Mesh.h"
#include "Material.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

void Model::SetGeometry(Graphics& graphics, Geometry& geometry)
{
	auto mesh = ResourceManager::Resolve<Mesh>(graphics, "Cube", geometry.vertices, geometry.indices);
	AddSharedBind(mesh);
	SetIndexCount(mesh->GetIndexCount());

	auto pt = std::make_unique<VertexConstantBuffer<ModelTransform>>(graphics, TRANSFORM_CBUFFER_SLOT);
	pTransformbuffer = pt.get();
	AddBind(std::move(pt));
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
				mesh.mNormals[idx].x, mesh.mNormals[idx].y, mesh.mNormals[idx].z
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

	auto mymesh = ResourceManager::Resolve<Mesh>(graphics, mesh.mName.C_Str(), vertices, indices);
	AddSharedBind(mymesh);
	SetIndexCount(mymesh->GetIndexCount());

	auto pt = std::make_unique<VertexConstantBuffer<ModelTransform>>(graphics, TRANSFORM_CBUFFER_SLOT);
	pTransformbuffer = pt.get();
	AddBind(std::move(pt));
}

void Model::SetShader(Graphics& graphics, const std::string& vsFile, const std::string& psFile)
{
	AddSharedBind(ResourceManager::Resolve<Material>(graphics, "SimpleLit", vsFile, psFile));
}

Model::Model(Graphics& graphics, Geometry& geometry, const std::string& vsFile, const std::string& psFile)
{
	SetGeometry(graphics, geometry);
	SetShader(graphics, vsFile, psFile);
}

Model::Model(Graphics& graphics, const aiMesh& mesh, const std::string& vsFile, const std::string& psFile)
{
	SetMesh(graphics, mesh);
	SetShader(graphics, vsFile, psFile);
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
