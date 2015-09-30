// KWexplicit.cpp
// Kun Ling <kunling@lingcc.com>
// This testcase is used to show how the explicit keyword works
// How to check:
//   clang++ -DWITH_EXPLICIT KWexplicit.cpp && ./a.out
//     expect: error no matching function for call to 'DoBar'
//   clang++ -UWITH_EXPLICIT KWexplicit.cpp && ./a.out
//     expect: PASS


#include <iostream>

using namespace std;

#ifdef WITH_EXPLICIT
#define EXPLICIT_TEST explicit
#else
#define EXPLICIT_TEST
#endif

#define CHECK_VALUE 42
class Foo {
public:
  EXPLICIT_TEST Foo (int foo)
  : m_foo(foo)
  {}
  int GetFoo() {
    return m_foo;
  }
private:
  int m_foo;
};

void DoBar( Foo  foo) {
  int i = foo.GetFoo();
  if (CHECK_VALUE = i) {
    cout << "PASS" << endl;
  } else {
    cout << "FAIL" << endl;
  }
}

int main() {
  DoBar (CHECK_VALUE);
}
