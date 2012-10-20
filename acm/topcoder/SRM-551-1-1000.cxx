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
#include <lingkun/debug.h>
using namespace std;


class SweetFruits {
public:
    int countTrees(vector <int> sweetness, int maxSweetness) {
        
    }
};

bool KawigiEdit_RunTest(int testNum, vector <int> p0, int p1, bool hasAnswer, int p2) {
    cout << "Test " << testNum << ": [" << "{";
    for (int i = 0; int(p0.size()) > i; ++i) {
        if (i > 0) {
            cout << ",";
        }
        cout << p0[i];
    }
    cout << "}" << "," << p1;
    cout << "]" << endl;
    SweetFruits *obj;
    int answer;
    obj = new SweetFruits();
    clock_t startTime = clock();
    answer = obj->countTrees(p0, p1);
    clock_t endTime = clock();
    delete obj;
    bool res;
    res = true;
    cout << "Time: " << double(endTime - startTime) / CLOCKS_PER_SEC << " seconds" << endl;
    if (hasAnswer) {
        cout << "Desired answer:" << endl;
        cout << "\t" << p2 << endl;
    }
    cout << "Your answer:" << endl;
    cout << "\t" << answer << endl;
    if (hasAnswer) {
        res = answer == p2;
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
    
    vector <int> p0;
    int p1;
    int p2;
    
    {
    // ----- test 0 -----
    int t0[] = {1,2,-1,3};
            p0.assign(t0, t0 + sizeof(t0) / sizeof(t0[0]));
    p1 = 3;
    p2 = 3;
    all_right = KawigiEdit_RunTest(0, p0, p1, true, p2) && all_right;
    // ------------------
    }
    
    {
    // ----- test 1 -----
    int t0[] = {1,2,-1,3};
            p0.assign(t0, t0 + sizeof(t0) / sizeof(t0[0]));
    p1 = 5;
    p2 = 7;
    all_right = KawigiEdit_RunTest(1, p0, p1, true, p2) && all_right;
    // ------------------
    }
    
    {
    // ----- test 2 -----
    int t0[] = {-1,-1,2,5,5};
            p0.assign(t0, t0 + sizeof(t0) / sizeof(t0[0]));
    p1 = 6;
    p2 = 20;
    all_right = KawigiEdit_RunTest(2, p0, p1, true, p2) && all_right;
    // ------------------
    }
    
    {
    // ----- test 3 -----
    int t0[] = {2,6,8,4,1,10,-1,-1,-1,-1};
            p0.assign(t0, t0 + sizeof(t0) / sizeof(t0[0]));
    p1 = 15;
    p2 = 17024000;
    all_right = KawigiEdit_RunTest(3, p0, p1, true, p2) && all_right;
    // ------------------
    }
    
    {
    // ----- test 4 -----
    int t0[] = {1078451,-1,21580110,8284711,-1,4202301,3427559,8261270,-1,16176713,22915672,24495540,19236,5477666,12280316,3305896,17917887,564911,22190488,21843923,23389728,14641920,9590140,12909561,20405638,100184,23336457,12780498,18859535,23180993,10278898,5753075,21250919,17563422,10934412,22557980,24895749,7593671,10834579,5606562};
            p0.assign(t0, t0 + sizeof(t0) / sizeof(t0[0]));
    p1 = 245243285;
    p2 = 47225123;
    all_right = KawigiEdit_RunTest(4, p0, p1, true, p2) && all_right;
    // ------------------
    }
    
    if (all_right) {
        cout << "You're a stud (at least on the example cases)!" << endl;
    } else {
        cout << "Some of the test cases had errors." << endl;
    }
    return 0;
}
