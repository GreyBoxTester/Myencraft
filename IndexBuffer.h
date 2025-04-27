#pragma once
#include "Bindable.h"

namespace cl
{
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(Graphics& gfx, unsigned int* indices, unsigned int count);
		void bind(Graphics& gfx) override;
		unsigned int getCount();
	private:
		unsigned int count;
		wrl::ComPtr<ID3D11Buffer> buffer;
	};
}
