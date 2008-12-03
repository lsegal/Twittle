#pragma once

#include <wx/wx.h>

template<typename T>
class ThreadCallback : public wxThread
{
	T& obj;
	void (T::*func)();
public:
	ThreadCallback(T& obj, void (T::*func)()) : 
	  wxThread(wxTHREAD_JOINABLE), obj(obj), func(func) { Create(); Run(); }
	
	// @override wxThread
	void* Entry() { (obj.*func)(); return 0; }
};

template<typename T, typename P1>
class ThreadCallback1 : public wxThread
{
	P1& p1;
	T& obj;
	void (T::*func)(P1&);

public:
	ThreadCallback1(T& obj, void (T::*func)(P1&), P1 & p1) : 
		wxThread(wxTHREAD_JOINABLE), p1(p1), obj(obj), func(func) { Create(); Run();  }
	
	// @override wxThread
	void* Entry() { (obj.*func)(p1); return 0; }
};
