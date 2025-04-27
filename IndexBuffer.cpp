#include "IndexBuffer.h"

cl::IndexBuffer::IndexBuffer(cl::Graphics& gfx, unsigned int* indices, unsigned int count)
    :count(count)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = count * sizeof(unsigned int);
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0u;
	desc.MiscFlags = 0u;
	desc.StructureByteStride = sizeof(unsigned int);
	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = indices;

	getDevice(gfx)->CreateBuffer(&desc, &subData, buffer.GetAddressOf());
}

void cl::IndexBuffer::bind(cl::Graphics& gfx)
{
	getContext(gfx)->IASetIndexBuffer(buffer.Get(), DXGI_FORMAT_R32_UINT, 0u); 
}

unsigned int cl::IndexBuffer::getCount()
{
    return count;
}
