#pragma once
#include "Texture.h"
#include "Sampler.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "Color.h"
#include "PerGraphicsCBuffers.h"
#include <memory>

namespace cl
{
	class Material : public Bindable
	{
	public:
		Material(Graphics& gfx, const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath);
		Material(Graphics& gfx, const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath, Image& texture);
		Material(Graphics& gfx, std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader, Image& texture);
		Material(Graphics& gfx, std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader, Image& texture, std::shared_ptr<Sampler> sampler);
		Material(std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader, std::shared_ptr<Texture> texture, std::shared_ptr<Sampler> sampler);
		void bind(Graphics& gfx) override;

		std::shared_ptr<VertexShader> shareVertexShader();
		std::shared_ptr<PixelShader> sharePixelShader();
		std::shared_ptr<Texture> shareTexture();
		std::shared_ptr<Sampler> shareSampler();
	public:
		cl::ColorFRGB color = {1.0f, 1.0f, 1.0f};
		float specularPower = 512.0f;
	private:
		std::shared_ptr<VertexShader> vShader;
		std::shared_ptr<PixelShader> pShader;
		std::shared_ptr<Texture> texture;
		std::shared_ptr<Sampler> sampler;
	};
}
