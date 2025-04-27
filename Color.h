#pragma once

namespace cl
{
	class Color
	{
	public:
		Color() : r(0u), g(0u), b(0u), a(0u) {};
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : r(r), g(g), b(b), a(a) {};
		unsigned char r, g, b, a;
	};

	class ColorRGB
	{
	public:
		ColorRGB() : r(0u), g(0u), b(0u) {};
		ColorRGB(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {};
		unsigned char r, g, b;
	};

	class ColorBGRA
	{
	public:
		ColorBGRA() : r(0u), g(0u), b(0u), a(0u) {};
		ColorBGRA(unsigned char b, unsigned char g, unsigned char r, unsigned char a = 255) : b(b), g(g), r(r), a(a) {};
		unsigned char b, g, r, a;
	};

	class ColorFRGB
	{
	public:
		ColorFRGB() : r(0u), g(0u), b(0u) {};
		ColorFRGB(float r, float g, float b) : r(r), g(g), b(b) {};
		float r, g, b;
	};
}
