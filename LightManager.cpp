#include "LightManager.h"

cl::LightManager::LightManager(cl::Graphics& gfx)
	: plLength(0), plCbuff(gfx, 2u), dlCbuff(gfx, 3u)
{}

cl::PointLightData* cl::LightManager::addPointLight(const cl::PointLightData& plData)
{
	if (plLength == MAX_POINT_LIGHTS) { return nullptr; }
	plBuffer.pointLights[plLength] = plData;
	plLength++;
	return &plBuffer.pointLights[plLength - 1u];
}

cl::PointLightData* cl::LightManager::addPointLight(const cl::Vector3f& position, const cl::ColorFRGB& color, float fallofIntensity, float modifier)
{
	if (plLength == MAX_POINT_LIGHTS) { return nullptr; }
	plBuffer.pointLights[plLength].pos = position;
	plBuffer.pointLights[plLength].col = color;
	plBuffer.pointLights[plLength].fallofIntensity = fallofIntensity;
	plBuffer.pointLights[plLength].modifier = modifier;
	plLength++;
	return &plBuffer.pointLights[plLength - 1u];
}

void cl::LightManager::clearPointLights()
{
	memset(plBuffer.pointLights, 0, sizeof(plBuffer.pointLights));
}

void cl::LightManager::updatePointLightCBuffer(cl::Graphics& gfx)
{
	plCbuff.update(gfx, plBuffer);
	plCbuff.bind(gfx);
}

void cl::LightManager::setDirectionalLight(const Vector3f& direction, const ColorFRGB& color)
{
	dlBuffer.dirLight = { color, 1.0f, direction };
}

void cl::LightManager::updateDirectionalLightCBuffer(Graphics& gfx)
{
	dlCbuff.update(gfx, dlBuffer);
	dlCbuff.bind(gfx);
}
