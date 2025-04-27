#include "VertexBuffer.h"

void cl::VertexBuffer::bind(cl::Graphics& gfx)
{
	UINT offset = 0u;
	getContext(gfx)->IASetVertexBuffers(0u, 1u, buffer.GetAddressOf(), &stride, &offset);
}

unsigned int cl::VertexBuffer::getCount()
{
	return count;
}
