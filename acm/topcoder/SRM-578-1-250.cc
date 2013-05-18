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


class GooseInZooDivOne {
  public:
    int count(vector <string> field, int dist) {
      return 0;
    }
};

bool KawigiEdit_RunTest(int testNum, vector <string> p0, int p1, bool hasAnswer, int p2) {
  cout << "Test " << testNum << ": [" << "{";
  for (int i = 0; int(p0.size()) > i; ++i) {
    if (i > 0) {
      cout << ",";
    }
    cout << "\"" << p0[i] << "\"";
  }
  cout << "}" << "," << p1;
  cout << "]" << endl;
  GooseInZooDivOne *obj;
  int answer;
  obj = new GooseInZooDivOne();
  clock_t startTime = clock();
  answer = obj->count(p0, p1);
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

  vector <string> p0;
  int p1;
  int p2;

  {
    // ----- test 0 -----
    string t0[] = {"vvv"};
    p0.assign(t0, t0 + sizeof(t0) / sizeof(t0[0]));
    p1 = 0;
    p2 = 3;
    all_right = KawigiEdit_RunTest(0, p0, p1, true, p2) && all_right;
    // ------------------
  }

  {
    // ----- test 1 -----
    string t0[] = {"."};
    p0.assign(t0, t0 + sizeof(t0) / sizeof(t0[0]));
    p1 = 100;
    p2 = 0;
    all_right = KawigiEdit_RunTest(1, p0, p1, true, p2) && all_right;
    // ------------------
  }

  {
    // ----- test 2 -----
    string t0[] = {"vvv"};
    p0.assign(t0, t0 + sizeof(t0) / sizeof(t0[0]));
    p1 = 1;
    p2 = 0;
    all_right = KawigiEdit_RunTest(2, p0, p1, true, p2) && all_right;
    // ------------------
  }

  {
    // ----- test 3 -----
    string t0[] = {"v.v..................v............................",".v......v..................v.....................v","..v.....v....v.........v...............v......v...",".........vvv...vv.v.........v.v..................v",".....v..........v......v..v...v.......v...........","...................vv...............v.v..v.v..v...",".v.vv.................v..............v............","..vv.......v...vv.v............vv.....v.....v.....","....v..........v....v........v.......v.v.v........",".v.......v.............v.v..........vv......v.....","....v.v.......v........v.....v.................v..","....v..v..v.v..............v.v.v....v..........v..","..........v...v...................v..............v","..v........v..........................v....v..v...","....................v..v.........vv........v......","..v......v...............................v.v......","..v.v..............v........v...............vv.vv.","...vv......v...............v.v..............v.....","............................v..v.............."
      "...v",".v.............v.......v..........................","......v...v........................v..............",".........v.....v..............vv..................","................v..v..v.........v....v.......v....","........v.....v.............v......v.v............","...........v....................v.v....v.v.v...v..","...........v......................v...v...........","..........vv...........v.v.....................v..",".....................v......v............v...v....",".....vv..........................vv.v.....v.v.....",".vv.......v...............v.......v..v.....v......","............v................v..........v....v....","................vv...v............................","................v...........v........v...v....v...","..v...v...v.............v...v........v....v..v....","......v..v.......v........v..v....vv..............","...........v..........v........v.v................","v.v......v................v....................v..",".v........v............................."
        "...v......","............................v...v.......v.........","........................vv.v..............v...vv..",".......................vv........v.............v..","...v.............v.........................v......","....v......vv...........................v.........","....vv....v................v...vv..............v..","..................................................","vv........v...v..v.....v..v..................v....",".........v..............v.vv.v.............v......",".......v.....v......v...............v.............","..v..................v................v....v......",".....v.....v.....................v.v......v......."};
    p0.assign(t0, t0 + sizeof(t0) / sizeof(t0[0]));
    p1 = 3;
    p2 = 898961330;
    all_right = KawigiEdit_RunTest(3, p0, p1, true, p2) && all_right;
    // ------------------
  }

  if (all_right) {
    cout << "You're a stud (at least on the example cases)!" << endl;
  } else {
    cout << "Some of the test cases had errors." << endl;
  }
  return 0;
}
