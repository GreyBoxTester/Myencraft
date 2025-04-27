#include "Sampler.h"

cl::Sampler::Sampler(cl::Graphics& gfx)
{
	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	getDevice(gfx)->CreateSamplerState(&desc, sampler.GetAddressOf());
}

void cl::Sampler::bind(cl::Graphics& gfx)
{
	getContext(gfx)->PSSetSamplers(0u, 1u, sampler.GetAddressOf());
}
