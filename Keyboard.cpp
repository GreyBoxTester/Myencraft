#include "Keyboard.h"

bool cl::Keyboard::isKeyPressed(unsigned char virtualKeyCode)
{
	return GetAsyncKeyState(virtualKeyCode);
}
