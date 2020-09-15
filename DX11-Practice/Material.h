#pragma once
#include "ResourceManager.h"
#include "Bindable.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <memory>

class Material : public Bindable, public Resource
{
public:
	Material(Graphics& graphics, const std::string& name, const std::string& vsFile, const std::string& psFile);
	void Bind(Graphics& graphics) noexcept override;

	static std::shared_ptr<Material> Resolve(Graphics& gfx, const std::string& name, const std::string& vsFile, const std::string& psFile)
	{
		return ResourceManager::Resolve<Material>(gfx, name, vsFile, psFile);
	}

	static std::string GetUID(const std::string& name, const std::string& vsFile, const std::string& psFile)
	{
		using namespace std::string_literals;
		return typeid(Material).name() + "#"s + name;
	}

private:
	std::string name;
	std::shared_ptr<VertexShader> pVertexShader;
	std::shared_ptr<PixelShader> pPixelShader;
};