#pragma once
#include <Windows.h>
#include "Event.h"
#include "Vector2.h"
#include "NonCopyable.h"
#include "PerGraphicsCBuffers.h"
//#include <string>

namespace cl
{
	class Window : public NonCopyable
	{
	public:
		static void initWindowClass();
		static void deinitWindowClass();

		Window(int posX, int posY, unsigned int sizeX, unsigned int sizeY, DWORD style, int nShowCmd);
		Window(const cl::Vector2i& pos, const cl::Vector2u& size, DWORD style, int nShowCmd);
		~Window();

		bool isOpen();
		void close();

		bool pollEvent(cl::Event* event_ptr);
		void waitEvent(cl::Event* event_ptr);

		void setTitle(const char* title);
		void setTitle(const wchar_t* title);
		
		Vector2u getSize() const;
		void setSize(const Vector2u& size);

		Vector2i getPosition() const;
		void setPosition(const Vector2i& position);

		void setMinSize(const Vector2u& size);
#ifndef _NO_ASPECT_RATIO_
		void setAspectRatio(const Vector2u& ratio);
#endif //_NO_ASPECT_RATIO_

		HWND getWindowHandle() const;

		cl::Graphics& gfx();
	/*public:
		Keyboard keyboard;
		Mouse mouse;*/
	private:
		static LRESULT CALLBACK WndProcInit(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		HWND hWnd;
		Event currentEvent;
		bool opened = false;

#ifndef _NO_ASPECT_RATIO_
		Vector2u aspectRatio = {0,0};
#endif //_NO_ASPECT_RATIO_
		Vector2u minSize = {0,0};
	
	private:
		Graphics* graphics = nullptr;
		PerGraphicsCBuffers* cBuffers = nullptr;
	};
}
