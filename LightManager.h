#pragma once
//#include "Bindable.h"
#include "Vector3.h"
#include "Color.h"
#include "ConstantBuffer.h"
#include <vector>

#define MAX_POINT_LIGHTS 4

namespace cl
{
	struct PointLightData
	{
		Vector3f pos;
		float fallofIntensity = 0.0f;
		float modifier = 0.0f;
		ColorFRGB col;
	};

	struct DirectionalLightData
	{
		ColorFRGB col;
		float padding;
		Vector3f dir;
	};

	//===================

	struct PointLightCBuff
	{
		PointLightData pointLights[MAX_POINT_LIGHTS];
	};

	struct DirectionalLightCBuff
	{
		DirectionalLightData dirLight;
		float padding;
	};

	class LightManager
	{
	public:
		LightManager(Graphics& gfx);
		PointLightData* addPointLight(const PointLightData& plData);
		PointLightData* addPointLight(const Vector3f& position, const ColorFRGB& color, float fallofIntensity, float modifier);
		void clearPointLights();
		void updatePointLightCBuffer(Graphics& gfx);

		void setDirectionalLight(const Vector3f& direction, const ColorFRGB& color);
		void updateDirectionalLightCBuffer(Graphics& gfx);

		//void bind(Graphics& gfx) override;
	private:
		PSConstantBuffer<PointLightCBuff> plCbuff;
		PSConstantBuffer<DirectionalLightCBuff> dlCbuff;
		PointLightCBuff plBuffer;
		DirectionalLightCBuff dlBuffer;
		unsigned int plLength;
	};
}
