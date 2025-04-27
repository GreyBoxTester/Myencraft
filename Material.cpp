#include "Material.h"

cl::Material::Material(cl::Graphics& gfx, const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath)
	: 
	vShader(std::make_shared<cl::VertexShader>(gfx, vertexShaderPath)), 
	pShader(std::make_shared<cl::PixelShader>(gfx, pixelShaderPath)), 
	texture(nullptr), sampler(nullptr)
{}

cl::Material::Material(cl::Graphics& gfx, const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath, cl::Image& texture)
	: 
	vShader(std::make_shared<cl::VertexShader>(gfx, vertexShaderPath)),
	pShader(std::make_shared<cl::PixelShader>(gfx, pixelShaderPath)),
	texture(std::make_shared<cl::Texture>(gfx, texture, 0u)),
	sampler(std::make_shared<cl::Sampler>(gfx))
{}

cl::Material::Material(cl::Graphics& gfx, std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader, Image& texture)
	: 
	vShader(vertexShader), pShader(pixelShader), 
	texture(std::make_shared<cl::Texture>(gfx, texture, 0u)), 
	sampler(std::make_shared<cl::Sampler>(gfx))
{}

cl::Material::Material(Graphics& gfx, std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader, Image& texture, std::shared_ptr<Sampler> sampler)
	: 
	vShader(vertexShader), pShader(pixelShader), 
	texture(std::make_shared<cl::Texture>(gfx, texture, 0u)), 
	sampler(sampler)
{}

cl::Material::Material(std::shared_ptr<cl::VertexShader> vertexShader, std::shared_ptr<cl::PixelShader> pixelShader, std::shared_ptr<cl::Texture> texture, std::shared_ptr<cl::Sampler> sampler)
	: vShader(vertexShader), pShader(pixelShader), texture(texture), sampler(sampler)
{}

void cl::Material::bind(cl::Graphics& gfx)
{
	vShader->bind(gfx);
	pShader->bind(gfx);
	if (texture.get() != nullptr) { texture->bind(gfx); }
	if (sampler.get() != nullptr) { sampler->bind(gfx); }

	MaterialCBuf newConstBuff = { color, specularPower };
	GET_MATERIAL_CBUFF(gfx).update(gfx, newConstBuff);
	GET_MATERIAL_CBUFF(gfx).bind(gfx);
}

std::shared_ptr<cl::VertexShader> cl::Material::shareVertexShader()
{
	return vShader;
}

std::shared_ptr<cl::PixelShader> cl::Material::sharePixelShader()
{
	return pShader;
}

std::shared_ptr<cl::Texture> cl::Material::shareTexture()
{
	return texture;
}

std::shared_ptr<cl::Sampler> cl::Material::shareSampler()
{
	return sampler;
}
