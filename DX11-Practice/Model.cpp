#include "Model.h"
#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

void Model::SetMesh(Graphics& graphics, std::vector<Geometry::Vertex>& vertices, UINT vertexCount, std::vector<unsigned short>& indices, UINT indexCount)
{
	// create vertex buffer
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(Geometry::Vertex) * vertexCount;
	bd.StructureByteStride = sizeof(Geometry::Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();
	graphics.GetDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer);

	// create index buffer
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(unsigned short) * indexCount;
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();
	graphics.GetDevice()->CreateBuffer(&ibd, &isd, &pIndexBuffer);

	this->indexCount = indexCount;
}

void Model::Draw(Graphics& graphics, float angle)
{
	// Bind vertex buffer to pipeline
	const UINT stride = sizeof(Geometry::Vertex);
	const UINT offset = 0u;
	graphics.GetContext()->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// bind index buffer
	graphics.GetContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// create constant buffer for transformation matrix
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};

	const ConstantBuffer cb =
	{
		{
			DirectX::XMMatrixRotationZ(angle) *
			DirectX::XMMatrixRotationX(angle) *
			DirectX::XMMatrixTranslation(0, 0, 5) *
			DirectX::XMMatrixPerspectiveFovLH(30, 800.f / 600.f, 0.1, 100)
		}
	};

	ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	graphics.GetDevice()->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	graphics.GetContext()->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	// create and bind vertex shader
	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(L"Shaders/VertexShader.cso", &pBlob);
	graphics.GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	graphics.GetContext()->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	// input (vertex) layout
	ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	graphics.GetDevice()->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);
	graphics.GetContext()->IASetInputLayout(pInputLayout.Get());

	// create and bind pixel shader
	ComPtr<ID3D11PixelShader> pPixelShader;
	D3DReadFileToBlob(L"Shaders/PixelShader.cso", &pBlob);
	graphics.GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	graphics.GetContext()->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	graphics.GetContext()->DrawIndexed(indexCount, 0u, 0u);
}
