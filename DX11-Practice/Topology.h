#pragma once
#include "ResourceManager.h"
#include "Bindable.h"
#include <string>

class Topology : public Bindable, public Resource
{
public:
	Topology(Graphics& graphics, D3D11_PRIMITIVE_TOPOLOGY type)
		: type(type)
	{
	}

	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->IASetPrimitiveTopology(type);
	}

	static std::string GetUID(D3D11_PRIMITIVE_TOPOLOGY type)
	{
		using namespace std::string_literals;
		return typeid(Topology).name() + "#"s + std::to_string(type);
	}

private:
	D3D11_PRIMITIVE_TOPOLOGY type;
};
