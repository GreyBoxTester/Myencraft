#pragma once
#include "Bindable.h"

namespace cl
{
	class Sampler : public Bindable
	{
	public:
		Sampler(Graphics& gfx);
		void bind(Graphics& gfx) override;
	private:
		wrl::ComPtr<ID3D11SamplerState> sampler;
	};
}
