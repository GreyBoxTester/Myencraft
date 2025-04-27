#include "Graphics.h"

cl::Graphics::Graphics(HWND hWnd, const cl::Vector2u& size)
	: perGfxCBuffers(nullptr)
{
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferDesc.Width = size.x;
	scd.BufferDesc.Height = size.y;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 0;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hWnd;
	scd.Windowed = true;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = NULL;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 
		NULL,
		nullptr,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&device,
		nullptr,
		&deviceContext
	);

	wrl::ComPtr<ID3D11Resource> res;
	swapChain->GetBuffer(NULL, __uuidof(ID3D11Resource), (void**)&res);
	device->CreateRenderTargetView(res.Get(), nullptr, &targetView);

	wrl::ComPtr<ID3D11DepthStencilState> dsState;
	D3D11_DEPTH_STENCIL_DESC descDSS = {};
	descDSS.DepthEnable = true;
	descDSS.DepthFunc = D3D11_COMPARISON_LESS;
	descDSS.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	device->CreateDepthStencilState(&descDSS, &dsState);

	deviceContext->OMSetDepthStencilState(dsState.Get(), 1u);
	
	wrl::ComPtr<ID3D11Texture2D> depthStencilTex;
	D3D11_TEXTURE2D_DESC descDST = {};
	descDST.Width = size.x;
	descDST.Height = size.y;
	descDST.MipLevels = 1u;
	descDST.ArraySize = 1u;
	descDST.Format = DXGI_FORMAT_D32_FLOAT;
	descDST.SampleDesc.Count = 1u;
	descDST.SampleDesc.Quality = 0u;
	descDST.Usage = D3D11_USAGE_DEFAULT;
	descDST.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	device->CreateTexture2D(&descDST, nullptr, &depthStencilTex);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;

	device->CreateDepthStencilView(depthStencilTex.Get(), &descDSV, &depthStencilView);

	deviceContext->OMSetRenderTargets(1u, targetView.GetAddressOf(), depthStencilView.Get());

	D3D11_VIEWPORT vPort = {};
	vPort.Width = size.x;
	vPort.Height = size.y;
	vPort.MinDepth = 0;
	vPort.MaxDepth = 1;
	vPort.TopLeftX = 0;
	vPort.TopLeftY = 0;
	deviceContext->RSSetViewports(1u, &vPort);
}

void cl::Graphics::swapBuffer()
{
	swapChain->Present(1u, NULL);
}

void cl::Graphics::clear(float red, float green, float blue)
{
	float col[4] = { red, green, blue, 1.0f };
	deviceContext->ClearRenderTargetView(targetView.Get(), col);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void cl::Graphics::setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
	deviceContext->IASetPrimitiveTopology(topology);
}

void cl::Graphics::draw(unsigned int count)
{
	deviceContext->Draw(count, 0u);
}

void cl::Graphics::drawIndexed(unsigned int count)
{
	deviceContext->DrawIndexed(count, 0u, 0u);
}

const void* cl::Graphics::getCBuffers()
{
	return perGfxCBuffers;
}
