#include "RenderCamera.h"
#include "Keyboard.h"

mc::RenderCamera::RenderCamera(cl::Graphics& gfx, const cl::Vector2u& size, cl::VertexShader* terrainVS, cl::VertexShader* plantsVS, cl::VertexShader* waterVS, const mc::Position* playerPos)
	: playerPos(playerPos), rot(), viewTransform(), widthToHeight(((float)size.x) / ((float)size.y)), cBuffer(gfx, 1u), terrainVS(terrainVS), plantsVS(plantsVS), waterVS(waterVS)
{}

void mc::RenderCamera::update(cl::Graphics& gfx)
{
	cl::Vector3f cPos = playerPos->p;
	cPos.y += 0.72f;

	viewTransform = dx::XMMatrixTranslation(-cPos.x, -cPos.y, -cPos.z) *
		dx::XMMatrixRotationY(rot.y) * dx::XMMatrixRotationX(rot.x) *
		dx::XMMatrixPerspectiveFovLH((cl::Keyboard::isKeyPressed('C') ? 0.9f : 2.094f), widthToHeight, 0.003f, 300.0f);

	frustum.update(viewTransform);

	cl::CameraCBuf newConstBuff = { cPos , 0.0f };
	cBuffer.update(gfx, newConstBuff);
	cBuffer.bind(gfx);
}

void mc::RenderCamera::draw(cl::Graphics& gfx, const mc::World& world)
{
	terrainVS->bind(gfx);

	cl::MaterialCBuf newConstBuff = { cl::ColorFRGB(1.0f, 1.0f, 1.0f), 0.0f };
	GET_MATERIAL_CBUFF(gfx).update(gfx, newConstBuff);
	GET_MATERIAL_CBUFF(gfx).bind(gfx);

	mc::AABB chunkBounds;

	for (auto& c : world.chunks)
	{
		if (!c.second.meshUpdated || c.second.vertexBuffer == nullptr || c.second.indexBuffer == nullptr) { continue; }
		c.second.getBounds(&chunkBounds, playerPos->getChunkPos());
		if (frustum.isBoxInFrustum(chunkBounds))
		{
			c.second.vertexBuffer->bind(gfx);
			c.second.indexBuffer->bind(gfx);

			dx::XMMATRIX transform = dx::XMMatrixTranslation(c.second.pos.x - playerPos->getChunkPos().x, 0, c.second.pos.y - playerPos->getChunkPos().y);

			cl::TransformCBuf newConstBuff = {
				dx::XMMatrixTranspose(transform * viewTransform),
				dx::XMMatrixTranspose(transform)
			};

			GET_TRANSFORM_CBUFF(gfx).update(gfx, newConstBuff);
			GET_TRANSFORM_CBUFF(gfx).bind(gfx);

			gfx.drawIndexed(c.second.indexBuffer->getCount());
		}
	}

	plantsVS->bind(gfx);

	for (auto& c : world.chunks)
	{
		if (!c.second.meshUpdated || c.second.plantsVB == nullptr || c.second.plantsIB == nullptr) { continue; }
		c.second.getBounds(&chunkBounds, playerPos->getChunkPos());
		if (frustum.isBoxInFrustum(chunkBounds))
		{
			c.second.plantsVB->bind(gfx);
			c.second.plantsIB->bind(gfx);

			dx::XMMATRIX transform = dx::XMMatrixTranslation(c.second.pos.x - playerPos->getChunkPos().x, 0, c.second.pos.y - playerPos->getChunkPos().y);

			cl::TransformCBuf newConstBuff = {
				dx::XMMatrixTranspose(transform * viewTransform),
				dx::XMMatrixTranspose(transform)
			};

			GET_TRANSFORM_CBUFF(gfx).update(gfx, newConstBuff);
			GET_TRANSFORM_CBUFF(gfx).bind(gfx);

			gfx.drawIndexed(c.second.plantsIB->getCount());
		}
	}

	waterVS->bind(gfx);

	newConstBuff = { cl::ColorFRGB(1.0f, 1.0f, 1.0f), 512.0f };
	GET_MATERIAL_CBUFF(gfx).update(gfx, newConstBuff);
	GET_MATERIAL_CBUFF(gfx).bind(gfx);

	for (auto& c : world.chunks)
	{
		if (!c.second.meshUpdated || c.second.waterVB == nullptr || c.second.waterIB == nullptr) { continue; }
		c.second.getBounds(&chunkBounds, playerPos->getChunkPos());
		if (frustum.isBoxInFrustum(chunkBounds))
		{
			c.second.waterVB->bind(gfx);
			c.second.waterIB->bind(gfx);

			dx::XMMATRIX transform = dx::XMMatrixTranslation(c.second.pos.x - playerPos->getChunkPos().x, 0, c.second.pos.y - playerPos->getChunkPos().y);

			cl::TransformCBuf newConstBuff = {
				dx::XMMatrixTranspose(transform * viewTransform),
				dx::XMMatrixTranspose(transform)
			};

			GET_TRANSFORM_CBUFF(gfx).update(gfx, newConstBuff);
			GET_TRANSFORM_CBUFF(gfx).bind(gfx);

			gfx.drawIndexed(c.second.waterIB->getCount());
		}
	}
}

const dx::XMMATRIX& mc::RenderCamera::getViewTransform() const
{
	return viewTransform;
}
