#pragma once
#include "ResourceManager.h"
#include "Bindable.h"
#include <string>

class VertexShader : public Bindable, public Resource
{
public:
	VertexShader(Graphics& graphics, const std::string& filePath)
	{
		ComPtr<ID3DBlob> pBlob;
		D3DReadFileToBlob(std::wstring(filePath.begin(), filePath.end()).c_str(), &pBlob);
		graphics.GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

		// input (vertex) layout
		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Texcoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Tangent", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Bitangent", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		graphics.GetDevice()->CreateInputLayout(
			ied, (UINT)std::size(ied),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&pInputLayout
		);
	}

	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
		GetContext(graphics)->IASetInputLayout(pInputLayout.Get());
	}

	static std::string GetUID(const std::string& path)
	{
		using namespace std::string_literals;
		return typeid(VertexShader).name() + "#"s + path;
	}

private:
	ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
	ComPtr<ID3D11InputLayout> pInputLayout = nullptr;
};