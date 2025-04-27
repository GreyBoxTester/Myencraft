#include "Texture.h"

cl::Texture::Texture(cl::Graphics& gfx, cl::Image& img, unsigned int textureID)
	:id(textureID)
{
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = img.getWidth();
	texDesc.Height = img.getHeight();
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = img.getData();
	subData.SysMemPitch = img.getWidth() * sizeof(cl::ColorBGRA);
	wrl::ComPtr<ID3D11Texture2D> tex;

	getDevice(gfx)->CreateTexture2D(&texDesc, &subData, &tex);

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Texture2D.MostDetailedMip = 0;

	getDevice(gfx)->CreateShaderResourceView(tex.Get(), &viewDesc, &textureView);
}

void cl::Texture::bind(cl::Graphics& gfx)
{
	getContext(gfx)->PSSetShaderResources(id, 1u, textureView.GetAddressOf());
}
