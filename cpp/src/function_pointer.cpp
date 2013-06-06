#include <iostream>

/**
 * This file is used to answer stackoverflow question:
 *   http://stackoverflow.com/questions/16956006/pass-function-to-another-function-by-pointer-and-by-name
 *
 *  Thu Jun  6 16:14:27 CST 2013
 *  Kun Ling <lkun.erlv@gmail.com>
 */   

using namespace std;

int add(int first, int second)
{
    return first + second;
}

int subtract(int first, int second)
{
    return first - second;
}

int operation(int first, int second, int (*functocall)(int, int))
{
    return (*functocall)(first, second);
}

int main()
{
    int  a, b;
    int  (*plus)(int, int) = add;
    a = operation(7, 5, plus);
    b = operation(20, a, subtract);
    cout << "a = " << a << " and b = " << b << endl;
    return 0;
}
