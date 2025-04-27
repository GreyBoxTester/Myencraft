#pragma once
#include "Bindable.h"

namespace cl
{
	class VertexBuffer : public Bindable
	{
	public:
		template<typename V>
		VertexBuffer(Graphics& gfx, const V* verticies, unsigned int count)
			:count(count), stride(sizeof(V))
		{
			D3D11_BUFFER_DESC desc = {};
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.ByteWidth = count * sizeof(V);
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = 0u;
			desc.MiscFlags = 0u;
			desc.StructureByteStride = sizeof(V);
			D3D11_SUBRESOURCE_DATA subData = {};
			subData.pSysMem = verticies;

			getDevice(gfx)->CreateBuffer(&desc, &subData, buffer.GetAddressOf());
		}

		void bind(Graphics& gfx) override;
		unsigned int getCount();
	private:
		unsigned int count;
		unsigned int stride;
		wrl::ComPtr<ID3D11Buffer> buffer;
	};
}
