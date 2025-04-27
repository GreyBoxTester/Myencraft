#pragma once
#include "Bindable.h"

namespace cl
{
	class Blender : public Bindable
	{
	public:
		Blender(Graphics& gfx);
		void bind(Graphics& gfx) override;
	private:
		wrl::ComPtr<ID3D11BlendState> blendState;
	};
}
