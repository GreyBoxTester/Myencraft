#include "Image.h"

cl::Image::Image(const wchar_t* path)
{
	cl::GdiplusManager::use();

	Gdiplus::Bitmap b(path);
	Gdiplus::Rect r;
	r.X = 0;
	r.Y = 0;
	r.Width = b.GetWidth();
	r.Height = b.GetHeight();
	
	Gdiplus::BitmapData data;
	b.LockBits(&r, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data);

	pixels = new cl::ColorBGRA[(size_t)r.Width * (size_t)r.Height];
	memcpy(pixels, data.Scan0, sizeof(cl::ColorBGRA) * (size_t)r.Width * (size_t)r.Height);

	width = r.Width;
	height = r.Height;

	b.UnlockBits(&data);
}

cl::Image::~Image()
{
	delete[] pixels;
	cl::GdiplusManager::release();
}

const cl::ColorBGRA* cl::Image::getData()
{
	return pixels;
}

cl::ColorBGRA cl::Image::getPixel(unsigned int x, unsigned int y)
{
	return pixels[y * width + x];
}

void cl::Image::setPixel(unsigned int x, unsigned int y, ColorBGRA col)
{
	pixels[y * width + x] = col;
}

unsigned int cl::Image::getWidth()
{
	return width;
}

unsigned int cl::Image::getHeight()
{
	return height;
}

cl::Vector2f cl::Image::toUV(unsigned int x, unsigned int y)
{
	return cl::Vector2f(((float)x) / ((float)width), ((float)y) / ((float)height));
}
