#include "Bindable.h"

ID3D11Device* cl::Bindable::getDevice(cl::Graphics& gfx)
{
	return gfx.device.Get();
}

ID3D11DeviceContext* cl::Bindable::getContext(cl::Graphics& gfx)
{
	return gfx.deviceContext.Get();
}
