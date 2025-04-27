#pragma once
#include <Windows.h>
#include "NonCopyable.h"

namespace cl
{
	class Keyboard : public NonCopyable
	{
	public:
		static bool isKeyPressed(unsigned char virtualKeyCode);
	};
}
