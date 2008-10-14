template <typename T>
class Callback
{
public:
	Callback(T& obj, void (T::*func)()) : obj(obj), func(func) { }
	void Call() { (obj.*func)(); }
private:
	T& obj;
	void (T::*func)();
};