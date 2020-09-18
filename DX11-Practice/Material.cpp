#include "Material.h"
#include "SlotConfig.h"

Material::Material(Graphics& graphics, const std::string& name, const std::string& vsFile, const std::string& psFile)
	: name(name)
	, materialData{ 0, 0, 0, 0 }
{
	pVertexShader = ResourceManager::Resolve<VertexShader>(graphics, vsFile);
	pPixelShader = ResourceManager::Resolve<PixelShader>(graphics, psFile);
	pSamplerState = ResourceManager::Resolve<SamplerState>(graphics, 0);

	pMaterialBuffer = std::make_unique<PixelConstantBuffer<MaterialData>>(graphics, MATERIAL_CBUFFER_SLOT);
}

void Material::Bind(Graphics& graphics) noexcept
{
	pVertexShader->Bind(graphics);
	pPixelShader->Bind(graphics);
	pSamplerState->Bind(graphics);

	for (auto& tex : pTextureViews)
	{
		tex->Bind(graphics);
	}

	pMaterialBuffer->Update(graphics, materialData);
	pMaterialBuffer->Bind(graphics);
}

void Material::AddTextureView(std::shared_ptr<TextureView> textureView)
{
	pTextureViews.push_back(textureView);
}

void Material::SetSpecularMapEnable()
{
	materialData.useSpecularMap = 1;
}

void Material::SetNormalMapEnable()
{
	materialData.useNormalMap = 1;
}
