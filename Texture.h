#pragma once
#include "Bindable.h"
#include "Image.h"

namespace cl
{
	class Texture : public Bindable
	{
	public:
		Texture(Graphics& gfx, Image& img, unsigned int textureID);
		void bind(Graphics& gfx) override;
	private:
		wrl::ComPtr<ID3D11ShaderResourceView> textureView;
		unsigned int id;
	};
}
