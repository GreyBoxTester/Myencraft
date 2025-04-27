#include "Camera.h"

#define GET_TRANSFORM_CBUFF(gfx) (((cl::PerGraphicsCBuffers*)gfx.getCBuffers())->transformCBuffer)

cl::Camera::Camera(cl::Graphics& gfx, const cl::Vector2u& size)
	: pos(), rot(), viewTransform(), widthToHeight(((float)size.x) / ((float)size.y)), cBuffer(gfx, 1u)
{}

void cl::Camera::update(cl::Graphics& gfx)
{
	viewTransform = dx::XMMatrixTranslation(-pos.x, -pos.y, -pos.z) *
		dx::XMMatrixRotationY(rot.y) * dx::XMMatrixRotationX(rot.x) *
		dx::XMMatrixPerspectiveFovLH(2.094f, widthToHeight, 0.003f, 300.0f);

	CameraCBuf newConstBuff = { pos, 0.0f };
	cBuffer.update(gfx, newConstBuff);
	cBuffer.bind(gfx);
}

void cl::Camera::draw3D(cl::Graphics& gfx, cl::Drawable& drawable)
{
	drawable.vBuffer->bind(gfx);
	if (drawable.iBuffer.get() != nullptr) { drawable.iBuffer->bind(gfx); }

	dx::XMMATRIX transform = dx::XMMatrixScaling(drawable.transform.scale.x, drawable.transform.scale.y, drawable.transform.scale.z) *
		dx::XMMatrixRotationX(drawable.transform.rotation.x) * dx::XMMatrixRotationY(drawable.transform.rotation.y) * dx::XMMatrixRotationY(drawable.transform.rotation.z) *
		dx::XMMatrixTranslation(drawable.transform.position.x, drawable.transform.position.y, drawable.transform.position.z);

	TransformCBuf newConstBuff = {
		dx::XMMatrixTranspose(transform * viewTransform),
		dx::XMMatrixTranspose(transform)
	};

	GET_TRANSFORM_CBUFF(gfx).update(gfx, newConstBuff);
	GET_TRANSFORM_CBUFF(gfx).bind(gfx);

	drawable.material->bind(gfx);

	if (drawable.iBuffer.get() != nullptr) { gfx.drawIndexed(drawable.iBuffer->getCount()); }
	else { gfx.draw(drawable.vBuffer->getCount()); }
}

dx::XMMATRIX& cl::Camera::getViewTransformMatrix()
{
	return viewTransform;
}
