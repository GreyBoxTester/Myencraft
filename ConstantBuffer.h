#pragma once
#include "Bindable.h"

namespace cl
{
	template<typename C>
	class ConstantBuffer : public Bindable
	{
	public:
		ConstantBuffer(Graphics& gfx, unsigned int bufferID)
			: id(bufferID)
		{
			D3D11_BUFFER_DESC desc = {};
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = sizeof(C);
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0u;
			desc.StructureByteStride = sizeof(C);

			getDevice(gfx)->CreateBuffer(&desc, nullptr, buffer.GetAddressOf());
		}

		ConstantBuffer(Graphics& gfx, C& constants, unsigned int bufferID)
			: id(bufferID)
		{
			D3D11_BUFFER_DESC desc = {};
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = sizeof(C);
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0u;
			desc.StructureByteStride = sizeof(C);
			D3D11_SUBRESOURCE_DATA subData = {};
			subData.pSysMem = &constants;

			getDevice(gfx)->CreateBuffer(&desc, &subData, buffer.GetAddressOf());
		}

		void update(Graphics& gfx, C& constants)
		{
			D3D11_MAPPED_SUBRESOURCE msr = {};
			getContext(gfx)->Map(buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
			memcpy(msr.pData, &constants, sizeof(C));
			getContext(gfx)->Unmap(buffer.Get(), 0u);
		}
	protected:
		wrl::ComPtr<ID3D11Buffer> buffer;
		unsigned int id;
	};

	template<typename C>
	class PSConstantBuffer : public ConstantBuffer<C>
	{
	public:
		PSConstantBuffer(Graphics& gfx, unsigned int bufferID) : ConstantBuffer<C>(gfx, bufferID) {}
		PSConstantBuffer(Graphics& gfx, C& constants, unsigned int bufferID) : ConstantBuffer<C>(gfx, constants, bufferID) {}
		void bind(Graphics& gfx) override
		{
			Bindable::getContext(gfx)->PSSetConstantBuffers(ConstantBuffer<C>::id, 1u, ConstantBuffer<C>::buffer.GetAddressOf());
		}
	};

	template<typename C>
	class VSConstantBuffer : public ConstantBuffer<C>
	{
	public:
		VSConstantBuffer(Graphics& gfx, unsigned int bufferID) : ConstantBuffer<C>(gfx, bufferID) {}
		VSConstantBuffer(Graphics& gfx, C& constants, unsigned int bufferID) : ConstantBuffer<C>(gfx, constants, bufferID) {}
		void bind(Graphics& gfx) override
		{
			Bindable::getContext(gfx)->VSSetConstantBuffers(ConstantBuffer<C>::id, 1u, ConstantBuffer<C>::buffer.GetAddressOf());
		}
	};
}
