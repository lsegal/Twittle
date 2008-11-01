#pragma once

template <typename T>
class Callback
{
private:
	T& obj;
	void (T::*func)();
public:
	Callback(T& obj, void (T::*func)()) : obj(obj), func(func) { }
	void Call() { (obj.*func)(); }
};

template <typename T, typename P1>
class Callback1
{
private:
	T& obj;
	void (T::*func)(P1& p1);
public:
	Callback1(T& obj, void (T::*func)(P1&)) : obj(obj), func(func) { }
	void Call(P1& p1) { (obj.*func)(p1); }
};