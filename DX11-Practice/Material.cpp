#include "Material.h"

Material::Material(Graphics& graphics, const std::string& name, const std::string& vsFile, const std::string& psFile)
	: name(name)
{
	pVertexShader = ResourceManager::Resolve<VertexShader>(graphics, vsFile);
	pPixelShader = ResourceManager::Resolve<PixelShader>(graphics, psFile);
}

void Material::Bind(Graphics& graphics) noexcept
{
	pVertexShader->Bind(graphics);
	pPixelShader->Bind(graphics);
	for (auto& tex : pTextureViews)
	{
		tex->Bind(graphics);
	}
}

void Material::AddTextureView(std::shared_ptr<TextureView> textureView)
{
	pTextureViews.push_back(textureView);
}
