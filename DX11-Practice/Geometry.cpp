#include "Geometry.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma comment(lib,"assimp-vc140-mt.lib")

Geometry Geometry::GenerateCube()
{
	const float size = 0.5f;

	std::vector<Geometry::Vertex> vertices = {
		// Back face
		{-0.5, -0.5,  0.5, 0, 0, 1 },
		{ 0.5, -0.5,  0.5, 0, 0, 1 },
		{ 0.5,  0.5,  0.5, 0, 0, 1 },
		{ -0.5, 0.5,  0.5, 0, 0, 1 },

		// Front face
		{ -0.5, -0.5, -0.5, 0, 0, -1 },
		{ -0.5,  0.5, -0.5, 0, 0, -1 },
		{  0.5,  0.5, -0.5, 0, 0, -1 },
		{  0.5, -0.5, -0.5, 0, 0, -1 },

		// Top face
		{ -0.5,  0.5, -0.5, 0, 1, 0 },
		{ -0.5,  0.5,  0.5, 0, 1, 0 },
		{  0.5,  0.5,  0.5, 0, 1, 0 },
		{  0.5,  0.5, -0.5, 0, 1, 0 },

		// Bottom face
		{ -0.5, -0.5, -0.5, 0, -1, 0 },
		{  0.5, -0.5, -0.5, 0, -1, 0 },
		{  0.5, -0.5,  0.5, 0, -1, 0 },
		{ -0.5, -0.5,  0.5, 0, -1, 0 },

		// Left face
		{ -0.5,  0.5, -0.5, -1, 0, 0 },
		{ -0.5, -0.5, -0.5, -1, 0, 0 },
		{ -0.5, -0.5,  0.5, -1, 0, 0 },
		{ -0.5,  0.5,  0.5, -1, 0, 0 },

		// Right face
		{ 0.5, -0.5,  0.5, 1, 0, 0 },
		{ 0.5, -0.5, -0.5, 1, 0, 0 },
		{ 0.5,  0.5, -0.5, 1, 0, 0 },
		{ 0.5,  0.5,  0.5, 1, 0, 0 },
	};

	std::vector<unsigned short> indices = {
		0,  1,  2,  0,  2,  3,    // back
		4,  5,  6,  4,  6,  7,    // front
		8,  9,  10, 8,  10, 11,   // top
		12, 13, 14, 12, 14, 15,   // bottom
		16, 17, 18, 16, 18, 19,   // left
		20, 21, 22, 20, 22, 23    // right
	};

	return { std::move(vertices), std::move(indices) };
}

Geometry Geometry::GenerateSphere(float radius, int widthSegment, int heightSegment)
{
	const float PI = 3.1415926536f;
	widthSegment = std::max(widthSegment, 4);
	heightSegment = std::max(heightSegment, 4);

	std::vector<Geometry::Vertex> vertices;
	std::vector<unsigned short> indices;

	for (float y = 0; y <= heightSegment; y++)
	{
		float theta = y * PI / heightSegment;
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);

		for (float x = 0; x <= widthSegment; x++)
		{
			float phi = x * 2 * PI / widthSegment;
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);

			float position[3], normal[3], texcoord[2];
			normal[0] = cosPhi * sinTheta;   // x
			normal[1] = cosTheta;            // y
			normal[2] = sinPhi * sinTheta;   // z

			position[0] = radius * normal[0];
			position[1] = radius * normal[1];
			position[2] = radius * normal[2];

			texcoord[0] = 1 - (x / widthSegment); // u
			texcoord[1] = 1 - (y / heightSegment); // v

			vertices.push_back({ position[0], position[1], position[2], normal[0], normal[1], normal[2] });
		}
	}

	for (int y = 0; y < heightSegment; y++)
	{
		for (int x = 0; x < widthSegment; x++)
		{
			int first = (y * (widthSegment + 1)) + x;
			int second = first + widthSegment + 1;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}

	return { std::move(vertices), std::move(indices) };
}

Geometry Geometry::LoadFromFile(const std::string fileName)
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(fileName,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals
	);

	std::vector<Geometry::Vertex> vertices;
	std::vector<unsigned short> indices;

	// for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
	{
		const auto& mesh = pScene->mMeshes[0];
		for (unsigned int idx = 0; idx < mesh->mNumVertices; idx++)
		{
			vertices.push_back(
				{ 
					mesh->mVertices[idx].x, mesh->mVertices[idx].y, mesh->mVertices[idx].z,
					mesh->mNormals[idx].x, mesh->mNormals[idx].y, mesh->mNormals[idx].z
				}
			);
		}
		for (unsigned int idx = 0; idx < mesh->mNumFaces; idx++)
		{
			const auto& face = mesh->mFaces[idx];
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}
	}

	return { std::move(vertices), std::move(indices) };
}
