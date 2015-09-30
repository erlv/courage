// KWexplicit.cpp
// Kun Ling <kunling@lingcc.com>
// This testcase is used to show how the explicit keyword works
// How to check:
//   $ clang++ CPPRTTI.cpp && ./a.out > CPPRTTI.cpp.out
//   $ diff CPPRTTI.cpp.out CPPRTTI.cpp.expected
//     expect: run success


#include <iostream>
#include <typeinfo>
#include <exception>
using namespace std;

class Base {
public:
    virtual void f() {}
    Base() {
      cout << "call Base Constructor" << endl;
    }

};

class Derived : public Base {
public:
  Derived() {
    cout << "call Derived Constructor" << endl;
  }
};

typedef int my_int;
int main () {
  try {
    Base *a = new Base;
    Base *b = new Derived;
    my_int c = 0;
    int d = 0;
    cout << "a is " << typeid(a).name() << endl;
    cout << "b is " << typeid(b).name() << endl;
    cout << "*a is " << typeid(*a).name() << endl;
    cout << "*b is " << typeid(*b).name() << endl;
    cout << "c is " << typeid(c).name() << endl;
    cout << "typeid(int) is " << typeid(int).name() << endl;
    cout << "typeid(Derived) is " << typeid(Derived).name() << endl;
    if (typeid(a) == typeid(b)) {
      cout << "a VS b: equal" << endl;
    } else {
      cout << "a VS b: unequal" << endl;
    }
    if (typeid(c) == typeid(d)) {
      cout << "c VS d: equal" << endl;
    } else {
      cout << "c VS d: unequal" << endl;
    }
    if (typeid(c) == typeid(int)) {
      cout << "c type is int" << endl;
    } else {
      cout << "c type is not int" << endl;
    }
    if (typeid(c) == typeid(my_int)) {
      cout << "c type is my_int" << endl;
    } else {
      cout << "c type is not my_int" << endl;
    }

  } catch (exception &e) {
    cout << "Exception: " << e.what() << endl;
  }
  return 0;
}
