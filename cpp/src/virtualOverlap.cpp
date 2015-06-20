/*
 * =====================================================================================
 *
 *       Filename:  virtualOverlap.cpp
 *
 *    Description:  This testcase is used to check whether derived class has virtual 
 *                  function with the same name as base, but has different prototype.
 *                  
 *
 *        Version:  1.0
 *        Created:  05/29/2015 07:13:36 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kun Ling (), kunling@lingcc.com
 *   Organization:  Lingcc.com
 *
 * =====================================================================================
 */

#include <iostream>

using namespace std;

class b {
  public:
    int _f_a;
    int _f_b;
    b () {
      _f_a = 1;
      _f_b = 2;
    }
    virtual void v_foo(int a, int b) {
      _f_a = _f_a + a;
      _f_b = _f_b + b;
      cout << "call v_foo(int a, int b)" << endl;
    }
};

class b_d: b {
  public:
    virtual void  v_foo(int a) {
      _f_a = _f_a + a;
      cout << "call v_foo(int a)" << endl;
    }
};

class b_c : b {
  public:
    virtual void v_foo(int a, int b) {
      cout << " call v_foo( int a, int b) of b_c " << endl;
    }
};

int main() {
  b obj_b;
  b_d obj_bd;
  b_c obj_bc;

  obj_b.v_foo(1);
  obj_b.v_foo(1,2);
  obj_bd.v_foo(1);
  obj_bd.v_foo(1,2);
  obj_bc.v_foo(1);
  obj_bc.v_foo(1,2);
}
