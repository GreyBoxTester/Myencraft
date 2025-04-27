#pragma once
#include "Bindable.h"

namespace cl
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(Graphics& gfx, const wchar_t* filename);
		VertexShader(Graphics& gfx, const wchar_t* filename, D3D11_INPUT_ELEMENT_DESC* inputLayout, unsigned int count);
		void bind(Graphics& gfx) override;
	private:
		wrl::ComPtr<ID3D11VertexShader> shader;
		wrl::ComPtr<ID3D11InputLayout> inpLayout;
	};

	class PixelShader : public Bindable
	{
	public:
		PixelShader(Graphics& gfx, const wchar_t* filename);
		void bind(Graphics& gfx) override;
	private:
		wrl::ComPtr<ID3D11PixelShader> shader;
	};
}
