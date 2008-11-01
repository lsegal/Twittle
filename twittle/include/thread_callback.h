#pragma once

#include <wx/wx.h>

#include "callback.h"

template<typename T>
class ThreadCallback : public wxThread
{
	Callback<T>& callback;
public:
	ThreadCallback(Callback<T>& callback) : 
	  callback(callback) { Create(); Run(); }
	
	// @override wxThread
	void* Entry() { callback.Call(); return 0; }
};

template<typename T, typename P1>
class ThreadCallback1 : public wxThread
{
	P1& p1;
	Callback1<T, P1>& callback;
public:
	ThreadCallback1(Callback1<T, P1>& callback, P1 & p1) : 
		p1(p1), callback(callback) { Create(); Run(); }

	
	// @override wxThread
	void* Entry() { callback.Call(p1); return 0; }
};
