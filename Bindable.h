#pragma once
#include "Graphics.h"

namespace cl
{
	class Bindable
	{
	public:
		virtual void bind(Graphics& gfx) = 0;
	protected:
		ID3D11Device* getDevice(Graphics& gfx);
		ID3D11DeviceContext* getContext(Graphics& gfx);
	};
}
