#pragma once
#include "Vector2.h"
#include "NonCopyable.h"
#include "Window.h"

namespace cl
{
	class Mouse : NonCopyable
	{
	public:
		enum class Button 
		{ 
			Left = VK_LBUTTON, 
			Right = VK_RBUTTON, 
			Middle = VK_MBUTTON 
		};
	public:
		static Vector2i getMousePos();
		static Vector2i getMousePos(const Window& relativeTo);
		static void setMousePos(const Vector2i& pos);
		static void setMousePos(const Vector2i& pos, const Window& relativeTo);
		static bool isPressed(Button button);
	};
}
