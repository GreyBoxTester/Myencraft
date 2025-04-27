#pragma once
#include <Windows.h>

#pragma comment(lib, "gdiplus.lib")
#include <gdiplus.h>

namespace cl
{
	class GdiplusManager
	{
	public:
		static void use()
		{
			if (userCount == 0u)
			{
				Gdiplus::GdiplusStartup(&token, &stInp, NULL);
			}
			userCount++;
		}

		static void release()
		{
			userCount--;
			if (userCount == 0u)
			{
				Gdiplus::GdiplusShutdown(token);
			}
		}

	private:
		static Gdiplus::GdiplusStartupInput stInp;
		static ULONG_PTR token;
		static unsigned int userCount;
	};
}
