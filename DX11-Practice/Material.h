#pragma once
#include "ResourceManager.h"
#include "Bindable.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "SamplerState.h"
#include "TextureView.h"
#include "ConstantBuffer.h"
#include <memory>
#include <vector>

class Material : public Bindable, public Resource
{
public:
	Material(Graphics& graphics, const std::string& name, const std::string& vsFile, const std::string& psFile);
	void Bind(Graphics& graphics) noexcept override;
	void AddTextureView(std::shared_ptr<TextureView> textureView);
	void SetSpecularMapEnable();
	void SetNormalMapEnable();

	static std::string GetUID(const std::string& name, const std::string& vsFile, const std::string& psFile)
	{
		using namespace std::string_literals;
		return typeid(Material).name() + "#"s + name;
	}

private:
	std::string name;
	std::shared_ptr<VertexShader> pVertexShader;
	std::shared_ptr<PixelShader> pPixelShader;
	std::shared_ptr<SamplerState> pSamplerState;
	std::vector<std::shared_ptr<TextureView>> pTextureViews;

	struct MaterialData
	{
		int useSpecularMap;
		int useNormalMap;
		int pad0;
		int pad1;
	} materialData;
	std::unique_ptr<PixelConstantBuffer<MaterialData>> pMaterialBuffer = nullptr;
};