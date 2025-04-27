#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "NonCopyable.h"
#include "Vector2.h"
#pragma comment(lib, "d3d11.lib")

namespace wrl = Microsoft::WRL;

namespace cl
{
	class Graphics : public NonCopyable
	{
		friend class Bindable;
		friend class Window;
	private:
		Graphics(HWND hWnd, const Vector2u& size);//only Window can instantiate Graphics object
	public:
		void swapBuffer();
		void clear(float red, float green, float blue);
		void setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology);
		void draw(unsigned int count);
		void drawIndexed(unsigned int count);

		const void* getCBuffers();
	private:
		wrl::ComPtr<ID3D11Device> device;
		wrl::ComPtr<IDXGISwapChain> swapChain;
		wrl::ComPtr<ID3D11DeviceContext> deviceContext;
		wrl::ComPtr<ID3D11RenderTargetView> targetView;
		wrl::ComPtr<ID3D11DepthStencilView> depthStencilView;
		void* perGfxCBuffers;
	};
}
