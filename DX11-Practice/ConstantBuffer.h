#pragma once
#include "Bindable.h"

template<typename T>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(Graphics& graphics, const T& data, UINT slot)
		: slot(slot)
	{
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(T);
		cbd.StructureByteStride = 0u;

		// create buffer with initial data
		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &data;
		GetDevice(graphics)->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	}

	ConstantBuffer(Graphics& graphics, UINT slot)
		: slot(slot)
	{
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = (UINT)sizeof(T);
		cbd.StructureByteStride = 0u;

		// create buffer without data
		GetDevice(graphics)->CreateBuffer(&cbd, nullptr, &pConstantBuffer);
	}

	void Update(Graphics& graphics, const T& data)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		GetContext(graphics)->Map(
			pConstantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr
		);

		memcpy(msr.pData, &data, sizeof(T));
		GetContext(graphics)->Unmap(pConstantBuffer.Get(), 0u);
	}

protected:
	ComPtr<ID3D11Buffer> pConstantBuffer;
	UINT slot;
};

template<typename T>
class VertexConstantBuffer : public ConstantBuffer<T>
{
	using ConstantBuffer<T>::pConstantBuffer;
	using ConstantBuffer<T>::slot;
	using Bindable::GetContext;
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
};

template<typename T>
class PixelConstantBuffer : public ConstantBuffer<T>
{
	using ConstantBuffer<T>::pConstantBuffer;
	using ConstantBuffer<T>::slot;
	using Bindable::GetContext;
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
};

template<typename T>
class SharedConstantBuffer : public ConstantBuffer<T>
{
	using ConstantBuffer<T>::pConstantBuffer;
	using ConstantBuffer<T>::slot;
	using Bindable::GetContext;
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
		GetContext(graphics)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
};