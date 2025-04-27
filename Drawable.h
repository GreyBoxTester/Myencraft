#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Transform.h"
#include "PerGraphicsCBuffers.h"
#include "Material.h"
#include <DirectXMath.h>
#include <memory>

namespace dx = DirectX;

namespace cl
{
	class Drawable
	{
		friend class Camera;
	public:
		Drawable(std::shared_ptr<VertexBuffer> vb, std::shared_ptr<IndexBuffer> ib, std::shared_ptr<Material> m);
		Drawable(std::shared_ptr<VertexBuffer> vb, std::shared_ptr<IndexBuffer> ib, std::shared_ptr<VertexShader> vs, std::shared_ptr<PixelShader> ps, std::shared_ptr<Texture> t, std::shared_ptr<Sampler> s);
	public:
		Transform transform;
	private:
		std::shared_ptr<VertexBuffer> vBuffer;
		std::shared_ptr<IndexBuffer> iBuffer;
		std::shared_ptr<Material> material;
	};
}
