#include <iostream>

int main(int argc, char const *argv[]);

class Base {
protected:
	using Func = void (Base::*)();
	inline void f() {}
public:
	Func y = &Base::f;
};

class Derived : public Base {
public:
	Func x = (Func)&Derived::f;
};

int main(int argc, char const *argv[])
{
	/* code */
	std::cout << (Derived().x == Base().y) << std::endl;
	return 0;
}