#include "Drawable.h"

cl::Drawable::Drawable(std::shared_ptr<VertexBuffer> vb, std::shared_ptr<IndexBuffer> ib, std::shared_ptr<Material> m)
	: vBuffer(vb), iBuffer(ib), material(m)
{}

cl::Drawable::Drawable(std::shared_ptr<cl::VertexBuffer> vb, std::shared_ptr<cl::IndexBuffer> ib, std::shared_ptr<cl::VertexShader> vs, std::shared_ptr<cl::PixelShader> ps, std::shared_ptr<cl::Texture> t, std::shared_ptr<cl::Sampler> s)
	: vBuffer(vb), iBuffer(ib), material(std::make_shared<cl::Material>(vs, ps, t, s)), transform()
{}
