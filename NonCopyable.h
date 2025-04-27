#pragma once
class NonCopyable
{
protected:
	NonCopyable() {}
	~NonCopyable() {}
private:
	NonCopyable(const NonCopyable& obj) = delete;
	NonCopyable& operator=(NonCopyable& other) = delete;
};

