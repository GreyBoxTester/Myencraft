#include "Mouse.h"

cl::Vector2i cl::Mouse::getMousePos()
{
	POINT pt;
	GetCursorPos(&pt);
	return cl::Vector2i(pt.x, pt.y);
}

cl::Vector2i cl::Mouse::getMousePos(const cl::Window& relativeTo)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(relativeTo.getWindowHandle(), &pt);
	return cl::Vector2i(pt.x, pt.y);
}

void cl::Mouse::setMousePos(const cl::Vector2i& pos)
{
	SetCursorPos(pos.x, pos.y);
}

void cl::Mouse::setMousePos(const cl::Vector2i& pos, const cl::Window& relativeTo)
{
	POINT pt = { pos.x, pos.y };
	ClientToScreen(relativeTo.getWindowHandle(), &pt);
	SetCursorPos(pt.x, pt.y);
}

bool cl::Mouse::isPressed(cl::Mouse::Button button)
{
	return GetAsyncKeyState((int)button);
}
