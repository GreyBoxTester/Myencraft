#pragma once
#include "Color.h"
#include "Vector2.h"
#include "GdiplusManager.h"

namespace cl
{
	class Image
	{
	public:
		Image(const wchar_t* path);
		~Image();
		const ColorBGRA* getData();
		ColorBGRA getPixel(unsigned int x, unsigned int y);
		void setPixel(unsigned int x, unsigned int y, ColorBGRA col);
		unsigned int getWidth();
		unsigned int getHeight();
		Vector2f toUV(unsigned int x, unsigned int y);
	private:
		ColorBGRA* pixels;
		unsigned int width;
		unsigned int height;
	};
}
