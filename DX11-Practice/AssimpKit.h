#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Graphics.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Mesh.h"
#include "Material.h"
#include <memory>

class AssimpKit
{
public:
	static SceneNode* LoadModelFromFile(Graphics& graphics, Scene& targetScene, const std::string& name, const std::string& filePath);
	static std::shared_ptr<Mesh> ParseMesh(Graphics& graphics, const aiMesh& mesh);
	static std::shared_ptr<Material> ParseMaterial(Graphics& graphics, const aiMaterial& material, const std::string& fileRootPath);
};