#pragma once
#include "World.h"
#include "ConstantBuffer.h"
#include "Material.h"
#include "Frustum.h"
#include "Position.h"
#include <DirectXMath.h>

namespace dx = DirectX;

namespace cl
{
	struct CameraCBuf
	{
		cl::Vector3f pos;
		float unused;
	};
}

namespace mc
{
	class RenderCamera
	{
	public:
		RenderCamera(cl::Graphics& gfx, const cl::Vector2u& size, cl::VertexShader* terrainVS, cl::VertexShader* plantsVS, cl::VertexShader* waterVS, const Position* playerPos);
		void update(cl::Graphics& gfx);
		void draw(cl::Graphics& gfx, const World& chunk);
		const dx::XMMATRIX& getViewTransform() const;
	public:
		const Position* playerPos;
		cl::Vector2f rot;
	private:
		cl::PSConstantBuffer<cl::CameraCBuf> cBuffer;
		dx::XMMATRIX viewTransform;
		Frustum frustum;
		float widthToHeight;
		cl::VertexShader* terrainVS;
		cl::VertexShader* plantsVS;
		cl::VertexShader* waterVS;
	};
}

