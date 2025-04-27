#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Drawable.h"
#include "ConstantBuffer.h"
#include "Material.h"
#include <DirectXMath.h>

namespace dx = DirectX;

namespace cl
{
	struct CameraCBuf
	{
		Vector3f pos;
		float unused;
	};

	class Camera
	{
	public:
		Camera(Graphics& gfx, const Vector2u& size);
		void update(Graphics& gfx);
		void draw3D(Graphics& gfx, Drawable& drawable);
		dx::XMMATRIX& getViewTransformMatrix();
	public:
		Vector3f pos;
		Vector2f rot;
	private:
		cl::PSConstantBuffer<CameraCBuf> cBuffer;
		dx::XMMATRIX viewTransform;
		float widthToHeight;
	};
}
