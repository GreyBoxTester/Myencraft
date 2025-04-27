#include "Window.h"
//#include <thread>
#include <string>

#define WINDOW_CLASS_NAME L"cl_window"

void cl::Window::initWindowClass()
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.hIcon = nullptr;
	wc.hIconSm = nullptr;
	wc.lpszMenuName = nullptr;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpfnWndProc = &WndProcInit;
	wc.lpszClassName = WINDOW_CLASS_NAME;

	RegisterClassEx(&wc);

	SetProcessDPIAware();
}

void cl::Window::deinitWindowClass()
{
	UnregisterClass(WINDOW_CLASS_NAME, GetModuleHandle(nullptr));
}

cl::Window::Window(int posX, int posY, unsigned int sizeX, unsigned int sizeY, DWORD style, int nShowCmd)
{
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = sizeX;
	rect.bottom = sizeY;
	AdjustWindowRect(&rect, style, false);//need only size of window

	hWnd = CreateWindowEx(
		0, WINDOW_CLASS_NAME,
		L"Window",
		style,
		posX, posY, rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, GetModuleHandle(nullptr), this
	);
	if (!hWnd) { PostQuitMessage(69); return; }

	ShowWindow(hWnd, nShowCmd);
	opened = true;

	graphics = new cl::Graphics(hWnd, cl::Vector2u(sizeX, sizeY));
	cBuffers = new cl::PerGraphicsCBuffers(*graphics);
	graphics->perGfxCBuffers = cBuffers;
}

cl::Window::Window(const cl::Vector2i& pos, const cl::Vector2u& size, DWORD style, int nShowCmd)
{
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = size.x;
	rect.bottom = size.y;
	AdjustWindowRect(&rect, style, false);//need only size of window

	hWnd = CreateWindowEx(
		0, WINDOW_CLASS_NAME,
		L"Window",
		style,
		pos.x, pos.y, rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, GetModuleHandle(nullptr), this
	);
	if (!hWnd) { PostQuitMessage(69); return; }

	ShowWindow(hWnd, nShowCmd);
	opened = true;

	graphics = new cl::Graphics(hWnd, size);
	cBuffers = new cl::PerGraphicsCBuffers(*graphics);
	graphics->perGfxCBuffers = cBuffers;
}

cl::Window::~Window()
{
	if (opened)
	{
		close();
	}
	delete cBuffers;
	delete graphics;
	cBuffers = nullptr;
	graphics = nullptr;
}

bool cl::Window::isOpen() { return opened; }

void cl::Window::close()
{
	opened = false;
	DestroyWindow(hWnd);
}

bool cl::Window::pollEvent(cl::Event* eventPtr)
{
	MSG message;
	if (PeekMessage(&message, hWnd, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
		*eventPtr = currentEvent;
		return true;
	}
	else return false;
}

void cl::Window::waitEvent(cl::Event* event_ptr)
{
	MSG message;
	if (GetMessage(&message, hWnd, 0, 0) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
		*event_ptr = currentEvent;
	}
}

void cl::Window::setTitle(const char* title)
{
	SetWindowTextA(hWnd, title);
}

void cl::Window::setTitle(const wchar_t* title)
{
	SetWindowTextW(hWnd, title);
}

cl::Vector2u cl::Window::getSize() const
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	return cl::Vector2u(rect.right - rect.left, rect.bottom - rect.top);
}

void cl::Window::setSize(const cl::Vector2u& size)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	rect.right = rect.left + size.x;
	rect.bottom = rect.top + size.y;
	AdjustWindowRect(&rect, GetWindowLong(hWnd, GWL_STYLE), false);
	MoveWindow(hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, false);
}

cl::Vector2i cl::Window::getPosition() const
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	return cl::Vector2i(rect.left, rect.top);
}

void cl::Window::setPosition(const cl::Vector2i& position)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	MoveWindow(hWnd, position.x, position.y, rect.right - rect.left, rect.bottom - rect.top, false);
}

void cl::Window::setMinSize(const cl::Vector2u& size)
{
	minSize = size;
}

#ifndef _NO_ASPECT_RATIO_
void cl::Window::setAspectRatio(const cl::Vector2u& ratio)
{
	aspectRatio = ratio;
}
#endif //_NO_ASPECT_RATIO_

LRESULT CALLBACK cl::Window::WndProcInit(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		Window* wndPtr = (Window*)(((CREATESTRUCTW*)lParam)->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)wndPtr);

		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)&Window::WndProcStatic);

		return wndPtr->WndProc(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK cl::Window::WndProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* wndPtr = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (wndPtr)
	{
		return wndPtr->WndProc(hWnd, msg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

#define RECT_PTR(x) ((RECT*)(x))
#define MINMAXINFO_PTR(x) ((MINMAXINFO*)(x))

LRESULT cl::Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		currentEvent.type = EventType::Closed;
		return 0;
	case WM_SIZE:
		currentEvent.type = EventType::Resized;
		return DefWindowProc(hWnd, msg, wParam, lParam);

#ifndef _NO_ASPECT_RATIO_
	case WM_SIZING:
		if (aspectRatio.x != 0 && aspectRatio.y != 0)
		{
			switch (wParam)
			{
			case WMSZ_TOP:
			case WMSZ_BOTTOM:
				RECT_PTR(lParam)->right = (RECT_PTR(lParam)->bottom - RECT_PTR(lParam)->top) * aspectRatio.x / aspectRatio.y + RECT_PTR(lParam)->left;
				break;
			case WMSZ_TOPLEFT:
			case WMSZ_TOPRIGHT:
				RECT_PTR(lParam)->top = RECT_PTR(lParam)->bottom - ((RECT_PTR(lParam)->right - RECT_PTR(lParam)->left) * aspectRatio.y / aspectRatio.x);
				break;
			default:
				RECT_PTR(lParam)->bottom = (RECT_PTR(lParam)->right - RECT_PTR(lParam)->left) * aspectRatio.y / aspectRatio.x + RECT_PTR(lParam)->top;
			}
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
#endif //_NO_ASPECT_RATIO_
	case WM_GETMINMAXINFO:
		if (minSize.x != 0 && minSize.y != 0)
		{
			MINMAXINFO_PTR(lParam)->ptMinTrackSize.x = minSize.x;
			MINMAXINFO_PTR(lParam)->ptMinTrackSize.y = minSize.y;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	/*//========MOUSE========
	case WM_MOUSEMOVE:
		mouse.onMove(MOUSE_POS(lParam));
		return DefWindowProc(hWnd, msg, wParam, lParam);
	case WM_MBUTTONDOWN:
		if (wParam == MK_LBUTTON) { mouse.onLeftButtonDown(); }
		if (wParam == MK_RBUTTON) { mouse.onRightButtonDown(); }
		return DefWindowProc(hWnd, msg, wParam, lParam);
	case WM_MBUTTONUP:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	//======KEYBOARD=======
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		keyboard.onKeyDown((char)wParam);
		return DefWindowProc(hWnd, msg, wParam, lParam);
	case WM_KEYUP:
	case WM_SYSKEYUP:
		keyboard.onKeyUp((char)wParam);
		return DefWindowProc(hWnd, msg, wParam, lParam);*/
	case WM_CHAR:
		currentEvent.type = EventType::TextEntered;
		currentEvent.letter = (char)wParam;
		return DefWindowProc(hWnd, msg, wParam, lParam);
	default:
		currentEvent.type = EventType::Unhandled;
		return DefWindowProc(hWnd, msg, wParam, lParam);
	//case WM_KEYDOWN: SetWindowText(hWnd, L"Alexey"); break;
	/*case WM_LBUTTONDOWN:
		POINTS pt = MAKEPOINTS(lParam);
		std::wstring s = L"mouse pos: " + std::to_wstring(pt.x) + L',' + std::to_wstring(pt.y);
		OutputDebugString(s.c_str());
		break;*/
	}
}

HWND cl::Window::getWindowHandle() const
{
	return hWnd;
}

cl::Graphics& cl::Window::gfx()
{
	return (*graphics);
}
