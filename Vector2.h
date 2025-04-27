#pragma once

namespace cl
{
	template<typename T>
	class Vector2
	{
	public:
		Vector2() : x((T)0), y((T)0) {}
		Vector2(T _x, T _y) : x(_x), y(_y) {}
		template<typename U> 
		Vector2(const Vector2<U>& other) : x(other.x), y(other.y) {}
	public:
		T x, y;
	};

	template<typename T>
	Vector2<T> operator +(const Vector2<T>& self, const Vector2<T>& other)
	{
		return Vector2<T>(self.x + other.x, self.y + other.y);
	}

	template<typename T>
	Vector2<T> operator -(const Vector2<T>& self, const Vector2<T>& other)
	{
		return Vector2<T>(self.x - other.x, self.y - other.y);
	}

	template<typename T>
	Vector2<T> operator *(const Vector2<T>& self, T num)
	{
		return Vector2<T>(self.x * num, self.y * num);
	}

	template<typename T>
	Vector2<T> operator /(const Vector2<T>& self, T num)
	{
		return Vector2<T>(self.x / num, self.y / num);
	}

	template<typename T>
	Vector2<T>& operator +=(Vector2<T>& self, const Vector2<T>& other)
	{
		self.x += other.x;
		self.y += other.y;
		return self;
	}

	template<typename T>
	Vector2<T>& operator -=(Vector2<T>& self, const Vector2<T>& other)
	{
		self.x -= other.x;
		self.y -= other.y;
		return self;
	}

	template<typename T>
	Vector2<T>& operator *=(Vector2<T>& self, T other)
	{
		self.x *= other;
		self.y *= other;
		return self;
	}

	template<typename T>
	Vector2<T>& operator /=(Vector2<T>& self, T other)
	{
		self.x /= other;
		self.y /= other;
		return self;
	}

	template<typename T>
	bool operator ==(const Vector2<T>& self, const Vector2<T>& other)
	{
		return (self.x == other.x) && (self.y == other.y);
	}

	template<typename T>
	bool operator !=(const Vector2<T>& self, const Vector2<T>& other)
	{
		return (self.x != other.x) || (self.y != other.y);
	}

	typedef Vector2<char> Vector2b;
	typedef Vector2<unsigned char> Vector2ub;
	typedef Vector2<short> Vector2s;
	typedef Vector2<int> Vector2i;
	typedef Vector2<unsigned int> Vector2u;
	typedef Vector2<float> Vector2f;
}
