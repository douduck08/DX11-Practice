#pragma once
#include "Bindable.h"
#include <vector>

class VertexBuffer : public Bindable
{
public:
	template<class T>
	VertexBuffer(Graphics& graphics, const std::vector<T>& vertices)
		: stride((UINT)sizeof(T))
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(T) * vertices.size());
		bd.StructureByteStride = sizeof(T);
		
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		graphics.GetDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer);
	}

	void Bind(Graphics& graphics) noexcept override
	{
		const UINT offset = 0u;
		GetContext(graphics)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	}

private:
	UINT stride;
	ComPtr<ID3D11Buffer> pVertexBuffer;
};