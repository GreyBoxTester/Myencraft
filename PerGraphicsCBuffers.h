#pragma once
#include "ConstantBuffer.h"
#include "Color.h"
#include <DirectXMath.h>

#define GET_TRANSFORM_CBUFF(gfx) (((cl::PerGraphicsCBuffers*)gfx.getCBuffers())->transformCBuffer)
#define GET_MATERIAL_CBUFF(gfx) (((cl::PerGraphicsCBuffers*)gfx.getCBuffers())->materialCBuffer)
#define GET_TIME_CBUFF(gfx) (((cl::PerGraphicsCBuffers*)gfx.getCBuffers())->timeCBuffer)

namespace dx = DirectX;

namespace cl
{
	struct MaterialCBuf
	{
		ColorFRGB color;
		float specularPower;
	};

	struct TransformCBuf
	{
		dx::XMMATRIX view;
		dx::XMMATRIX transform;
	};

	struct TimeCBuf
	{
		float time;
		float p1, p2, p3;
	};

	class PerGraphicsCBuffers
	{
	public:
		PerGraphicsCBuffers(Graphics& gfx) : materialCBuffer(gfx, 0u), transformCBuffer(gfx, 0u), timeCBuffer(gfx, 1u) {}
		PSConstantBuffer<MaterialCBuf> materialCBuffer;
		VSConstantBuffer<TransformCBuf> transformCBuffer;
		VSConstantBuffer<TimeCBuf> timeCBuffer;
	};
}