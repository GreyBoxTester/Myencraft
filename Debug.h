#pragma once
//#ifdef _DEBUG
#include "NonCopyable.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Color.h"
#include <string>

namespace cl
{
	class Debug : public NonCopyable
	{
	public:
		Debug();

		Debug& operator<< (const bool val);
		Debug& operator<< (const char let);
		Debug& operator<< (const wchar_t let);
		Debug& operator<< (const int8_t num);
		Debug& operator<< (const uint8_t num);
		Debug& operator<< (const short num);
		Debug& operator<< (const unsigned short num);
		Debug& operator<< (const int num);
		Debug& operator<< (const unsigned int num);
		Debug& operator<< (const long num);
		Debug& operator<< (const unsigned long num);
		Debug& operator<< (const long long num);
		Debug& operator<< (const unsigned long long num);
		Debug& operator<< (const float num);
		Debug& operator<< (const double num);
		Debug& operator<< (const Vector2s& vec);
		Debug& operator<< (const Vector2i& vec);
		Debug& operator<< (const Vector2u& vec);
		Debug& operator<< (const Vector2f& vec);
		Debug& operator<< (const Vector3s& vec);
		Debug& operator<< (const Vector3i& vec);
		Debug& operator<< (const Vector3u& vec);
		Debug& operator<< (const Vector3f& vec);
		Debug& operator<< (const ColorBGRA& col);
		Debug& operator<< (const char* str);
		Debug& operator<< (const wchar_t* str);
		Debug& operator<< (const std::string& str);
		Debug& operator<< (const std::wstring& str);

		void newLine();
	};

	static Debug debug;
}
//#endif