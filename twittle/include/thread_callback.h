#pragma once

#include <wx/wx.h>

/**
 * A version of Callback but encapsulated into a thread which automatically
 * performs the callback on its Entry function.
 *
 * @see Callback
 */
template<typename T>
class ThreadCallback : public wxThread
{
	T& obj;
	void (T::*func)();
public:
	/** 
	 * Creates a new callback for object obj calling function func when
	 * the thread starts.
	 *
	 * @param obj the object to perform the callback on
	 * @param func the function name to call when the thread starts
	 */
	ThreadCallback(T& obj, void (T::*func)()) : 
	  wxThread(wxTHREAD_JOINABLE), obj(obj), func(func) { Create(); Run(); }
	
	/**
	 * The thread entry function. This calls the function callback immediately.
	 */
	void* Entry() { (obj.*func)(); return 0; }
};

/**
 * A version of Callback1 but encapsulated into a thread which automatically
 * performs the callback on its Entry function.
 *
 * @see Callback1
 */
template<typename T, typename P1>
class ThreadCallback1 : public wxThread
{
	P1& p1;
	T& obj;
	void (T::*func)(P1&);

public:
	/** 
	 * Creates a new callback for object obj calling function func with data p1 when
	 * the thread starts.
	 *
	 * @param obj the object to perform the callback on
	 * @param func the function name to call when the thread starts
	 * @param p1 extra data to pass to the callback
	 */
	ThreadCallback1(T& obj, void (T::*func)(P1&), P1 & p1) : 
		wxThread(wxTHREAD_JOINABLE), p1(p1), obj(obj), func(func) { Create(); Run();  }
	
	/**
	 * The thread entry function. This calls the function callback immediately.
	 */
	void* Entry() { (obj.*func)(p1); return 0; }
};
