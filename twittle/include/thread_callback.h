#pragma once

#include <wx/wx.h>

#include "callback.h"

template<typename T>
class ThreadCallback : public wxThread
{
	const Callback<T>& callback;
public:
	ThreadCallback(const Callback<T>& callback) : callback(callback) { }
	
	// @override wxThread
	void* Entry() { callback.Call(); }
};
