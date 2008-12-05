#pragma once

template<typename S, typename T>
class Serializer
{
private:
	Serializer() { }
	~Serializer() { }
public:
	static T *Read(const S& path);
	static void Write(const S& path, const T& feed);
};
