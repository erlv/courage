#include <iostream>
#include <chrono>

/**
 * This file is used to answer stackoverflow question:
 *   http://stackoverflow.com/questions/16708715/how-to-measure-execution-time-in-c11/16708979#16708979
 * Thu Jun  6 16:24:32 CST 2013
 * Kun Ling <lkun.erlv@gmail.com>
 */


using namespace std;

int main(){
    cout << "system_clock" << endl;
    cout << chrono::system_clock::period::num << endl;
    cout << chrono::system_clock::period::den << endl;
    cout << "steady = " << boolalpha << chrono::system_clock::is_steady << endl << endl;

    cout << "high_resolution_clock" << endl;
    cout << chrono::high_resolution_clock::period::num << endl;
    cout << chrono::high_resolution_clock::period::den << endl;
    cout << "steady = " << boolalpha << chrono::high_resolution_clock::is_steady << endl << endl;

    cout << "steady_clock" << endl;
    cout << chrono::steady_clock::period::num << endl;
    cout << chrono::steady_clock::period::den << endl;
    cout << "steady = " << boolalpha << chrono::steady_clock::is_steady << endl << endl;

    return 0;    
}
