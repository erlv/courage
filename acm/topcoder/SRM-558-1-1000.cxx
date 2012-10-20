/*
 * At each step, each animal will do their optimal steps.
 * So a framework is needed to find out the optimal steps. 
 * However, I do not know how to find it. What a pity.
 * How to Optimal:
    1. Try to convert all white cells in a single step.
    2. Try to leave odd number of individual white cells.

 * DP may be a solution
*/
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;


class CatAndRabbit {
public:
    string getWinner(string tiles) {
        
    }
};

bool KawigiEdit_RunTest(int testNum, string p0, bool hasAnswer, string p1) {
    cout << "Test " << testNum << ": [" << "\"" << p0 << "\"";
    cout << "]" << endl;
    CatAndRabbit *obj;
    string answer;
    obj = new CatAndRabbit();
    clock_t startTime = clock();
    answer = obj->getWinner(p0);
    clock_t endTime = clock();
    delete obj;
    bool res;
    res = true;
    cout << "Time: " << double(endTime - startTime) / CLOCKS_PER_SEC << " seconds" << endl;
    if (hasAnswer) {
        cout << "Desired answer:" << endl;
        cout << "\t" << "\"" << p1 << "\"" << endl;
    }
    cout << "Your answer:" << endl;
    cout << "\t" << "\"" << answer << "\"" << endl;
    if (hasAnswer) {
        res = answer == p1;
    }
    if (!res) {
        cout << "DOESN'T MATCH!!!!" << endl;
    } else if (double(endTime - startTime) / CLOCKS_PER_SEC >= 2) {
        cout << "FAIL the timeout" << endl;
        res = false;
    } else if (hasAnswer) {
        cout << "Match :-)" << endl;
    } else {
        cout << "OK, but is it right?" << endl;
    }
    cout << "" << endl;
    return res;
}
int main() {
    bool all_right;
    all_right = true;
    
    string p0;
    string p1;
    
    {
    // ----- test 0 -----
    p0 = "#..";
    p1 = "Cat";
    all_right = KawigiEdit_RunTest(0, p0, true, p1) && all_right;
    // ------------------
    }
    
    {
    // ----- test 1 -----
    p0 = ".#.";
    p1 = "Rabbit";
    all_right = KawigiEdit_RunTest(1, p0, true, p1) && all_right;
    // ------------------
    }
    
    {
    // ----- test 2 -----
    p0 = "###";
    p1 = "Rabbit";
    all_right = KawigiEdit_RunTest(2, p0, true, p1) && all_right;
    // ------------------
    }
    
    {
    // ----- test 3 -----
    p0 = "#..##.#";
    p1 = "Cat";
    all_right = KawigiEdit_RunTest(3, p0, true, p1) && all_right;
    // ------------------
    }
    
    {
    // ----- test 4 -----
    p0 = "...";
    p1 = "Rabbit";
    all_right = KawigiEdit_RunTest(4, p0, true, p1) && all_right;
    // ------------------
    }
    
    {
    // ----- test 5 -----
    p0 = "###...####....###...####....";
    p1 = "Rabbit";
    all_right = KawigiEdit_RunTest(5, p0, true, p1) && all_right;
    // ------------------
    }
    
    if (all_right) {
        cout << "You're a stud (at least on the example cases)!" << endl;
    } else {
        cout << "Some of the test cases had errors." << endl;
    }
    return 0;
}
