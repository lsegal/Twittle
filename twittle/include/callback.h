#pragma once

/**
 * Using this Callback class you can seamlessly bridge
 * C style callback functions with C++ method calls.
 *
 * Example:
 *   void MyClass::method1() {
 *     Callback<MyClass> c(*this, &MyClass::callbackMethod);
 *     OtherClass other;
 *     other.send(c);
 *   }
 *
 *   void MyClass::callbackMethod() {
 *     std::cout << "Callback method" << std::endl;
 *   }
 *
 *   template<T>
 *   void OtherClass::send(Callback<T>& callback) {
 *     // do something here
 *     callback.Call(); // call callback function
 *   }
 *
 * The above example creates a callback object pointing
 * to the method defined as a member of the original class
 * and passes this object to OtherClass to be called.
 * The result is the output "Callback method" called to
 * the screen. Note that the method is called as if you called
 * `obj->callbackMethod()`, so `this` will hold the original
 * object 
 *
 * If you want to pass around parameters consider using ::Callback1
 * 
 * @see Callback1
 */
template <typename T>
class Callback
{
private:
	T& obj;
	void (T::*func)();
public:
	/** 
	 * Creates a callback to object obj on the member method func
	 * 
	 * @param obj the object to call the callback on (obj->callback())
	 * @param func the function call to callback when #Call is called
	 */
	Callback(T& obj, void (T::*func)()) : obj(obj), func(func) { }

	/** 
	 * Calls the callback function on the original object
	 */
	void Call() { (obj.*func)(); }
};

/**
 * This callback class represents the same form of callback functionality
 * as ::Callback, but with an added parameter that can be passed around.
 * This might be used as:
 *
 *   ...
 *   Callback1<MyClass, std::string> cb(*this, &MyClass::callback);
 *   ...
 *   cb.Call(std::string("hello world"));
 *   ...
 *
 * Which would call the callback method with the string "hello world" as
 * the parameter. This is useful if you pass the object to another class
 * that needs to share more information than just the call itself when 
 * performing the callback.
 */
template <typename T, typename P1>
class Callback1
{
private:
	T& obj;
	void (T::*func)(P1& p1);
public:
	/** 
	 * Creates a callback to object obj on the member method func
	 * 
	 * @param obj the object to call the callback on (obj->callback())
	 * @param func the function call to callback when #Call is called
	 */
	Callback1(T& obj, void (T::*func)(P1&)) : obj(obj), func(func) { }

	/** 
	 * Calls the callback function on the original object with a parameter
	 *
	 * @param p1 extra data to be passed to the callback method
	 */
	void Call(P1& p1) { (obj.*func)(p1); }
};
