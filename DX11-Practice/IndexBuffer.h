#pragma once
#include "Bindable.h"
#include <vector>

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& graphics, const std::vector<unsigned short>& indices)
		: count((UINT)indices.size())
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(unsigned short) * indices.size());
		bd.StructureByteStride = sizeof(unsigned short);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = indices.data();
		graphics.GetDevice()->CreateBuffer(&bd, &sd, &pIndexBuffer);
	}

	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}

	UINT GetCount() const noexcept
	{
		return count;
	}

private:
	UINT count;
	ComPtr<ID3D11Buffer> pIndexBuffer;
};
