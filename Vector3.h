#pragma once
#include <cmath>

namespace cl
{
	template<typename T>
	class Vector3
	{
	public:
		Vector3() : x((T)0), y((T)0), z((T)0) {}
		Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
		template<typename U>
		Vector3(const Vector3<U>& other) : x(other.x), y(other.y), z(other.z) {}
	public:
		T x, y, z;
	};

	template<typename T>
	Vector3<T> operator +(const Vector3<T>& self, const Vector3<T>& other)
	{
		return Vector3<T>(self.x + other.x, self.y + other.y, self.z + other.z);
	}

	template<typename T>
	Vector3<T> operator -(const Vector3<T>& self, const Vector3<T>& other)
	{
		return Vector3<T>(self.x - other.x, self.y - other.y, self.z - other.z);
	}

	template<typename T>
	Vector3<T> operator *(const Vector3<T>& self, T num)
	{
		return Vector3<T>(self.x * num, self.y * num, self.z * num);
	}

	template<typename T>
	Vector3<T> operator /(const Vector3<T>& self, T num)
	{
		return Vector3<T>(self.x / num, self.y / num, self.z / num);
	}

	template<typename T>
	Vector3<T>& operator +=(Vector3<T>& self, const Vector3<T>& other)
	{
		self.x += other.x;
		self.y += other.y;
		self.z += other.z;
		return self;
	}

	template<typename T>
	Vector3<T>& operator -=(Vector3<T>& self, const Vector3<T>& other)
	{
		self.x -= other.x;
		self.y -= other.y;
		self.z -= other.z;
		return self;
	}

	template<typename T>
	Vector3<T>& operator *=(Vector3<T>& self, T other)
	{
		self.x *= other;
		self.y *= other;
		self.z *= other;
		return self;
	}

	template<typename T>
	Vector3<T>& operator /=(Vector3<T>& self, T other)
	{
		self.x /= other;
		self.y /= other;
		self.z /= other;
		return self;
	}

	template<typename T>
	bool operator ==(const Vector3<T>& self, const Vector3<T>& other)
	{
		return (self.x == other.x) && (self.y == other.y) && (self.z == other.z);
	}

	template<typename T>
	bool operator !=(const Vector3<T>& self, const Vector3<T>& other)
	{
		return (self.x != other.x) || (self.y != other.y) || (self.z != other.z);
	}

	template<typename T>
	T dotProduct(const Vector3<T>& first, const Vector3<T>& second)
	{
		return first.x * second.x + first.y * second.y + first.z * second.z;
	}

	template<typename T>
	Vector3<T> crossProduct(const Vector3<T>& first, const Vector3<T>& second)
	{
		return Vector3<T>(first.y * second.z - first.z * second.y, first.z * second.x - first.x * second.z, first.x * second.y - first.y * second.x);
	}

	template<typename T>
	float length(const Vector3<T>& vec)
	{
		return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}

	template<typename T>
	Vector3<T> normalize(const Vector3<T>& vec)
	{
		float l = length(vec);
		return Vector3<T>(vec.x / l, vec.y / l, vec.z / l);
	}

	typedef Vector3<char> Vector3b;
	typedef Vector3<unsigned char> Vector3ub;
	typedef Vector3<short> Vector3s;
	typedef Vector3<int> Vector3i;
	typedef Vector3<unsigned int> Vector3u;
	typedef Vector3<float> Vector3f;
}