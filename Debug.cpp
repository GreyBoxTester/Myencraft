//#ifdef _DEBUG
#include "Debug.h"
#include <Windows.h>

cl::Debug::Debug() {}

cl::Debug& cl::Debug::operator<< (const bool val)
{
	if (val) { OutputDebugStringA("1"); }
	else { OutputDebugStringA("0"); }
	return *this;
}

cl::Debug& cl::Debug::operator<< (const char let)
{
	char str[2] = { let, '\0' };
	OutputDebugStringA(str);
	return *this;
}

cl::Debug& cl::Debug::operator<< (const wchar_t let)
{
	wchar_t str[2] = { let, '\0' };
	OutputDebugStringW(str);
	return *this;
}

cl::Debug& cl::Debug::operator<< (const int8_t num)
{
	OutputDebugStringA(std::to_string(num).c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const uint8_t num)
{
	OutputDebugStringA(std::to_string(num).c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const short num)
{
	OutputDebugStringA(std::to_string(num).c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const unsigned short num)
{
	OutputDebugStringA(std::to_string(num).c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const int num)
{
	OutputDebugStringA(std::to_string(num).c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const unsigned int num)
{
	OutputDebugStringA(std::to_string(num).c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const long num)
{
	OutputDebugStringA(std::to_string(num).c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const unsigned long num)
{
	OutputDebugStringA(std::to_string(num).c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const long long num)
{
	OutputDebugStringA(std::to_string(num).c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const unsigned long long num)
{
	OutputDebugStringA(std::to_string(num).c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const float num)
{
	OutputDebugStringA(std::to_string(num).c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const double num)
{
	OutputDebugStringA(std::to_string(num).c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const cl::Vector2s& vec)
{
	OutputDebugStringA(('{' + std::to_string(vec.x) + ',' + std::to_string(vec.y) + '}').c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const cl::Vector2i& vec)
{
	OutputDebugStringA(('{' + std::to_string(vec.x) + ',' + std::to_string(vec.y) + '}').c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const cl::Vector2u& vec)
{
	OutputDebugStringA(('{' + std::to_string(vec.x) + ',' + std::to_string(vec.y) + '}').c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const cl::Vector2f& vec)
{
	OutputDebugStringA(('{' + std::to_string(vec.x) + ',' + std::to_string(vec.y) + '}').c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<<(const Vector3s& vec)
{
	OutputDebugStringA(('{' + std::to_string(vec.x) + ',' + std::to_string(vec.y) + ',' + std::to_string(vec.z) + '}').c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<<(const Vector3i& vec)
{
	OutputDebugStringA(('{' + std::to_string(vec.x) + ',' + std::to_string(vec.y) + ',' + std::to_string(vec.z) + '}').c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<<(const Vector3u& vec)
{
	OutputDebugStringA(('{' + std::to_string(vec.x) + ',' + std::to_string(vec.y) + ',' + std::to_string(vec.z) + '}').c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<<(const Vector3f& vec)
{
	OutputDebugStringA(('{' + std::to_string(vec.x) + ',' + std::to_string(vec.y) + ',' + std::to_string(vec.z) + '}').c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<<(const ColorBGRA& col)
{
	OutputDebugStringA(('{' + std::to_string(col.r) + ',' + std::to_string(col.g) + ',' + std::to_string(col.b) + ',' + std::to_string(col.a) + '}').c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const char* str)
{
	OutputDebugStringA(str);
	return *this;
}

cl::Debug& cl::Debug::operator<< (const wchar_t* str)
{
	OutputDebugStringW(str);
	return *this;
}

cl::Debug& cl::Debug::operator<< (const std::string& str)
{
	OutputDebugStringA(str.c_str());
	return *this;
}

cl::Debug& cl::Debug::operator<< (const std::wstring& str)
{
	OutputDebugStringW(str.c_str());
	return *this;
}

void cl::Debug::newLine()
{
	char str[2] = { '\n', '\0' };
	OutputDebugStringA(str);
}

//#endif