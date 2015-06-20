/*
 * =====================================================================================
 *
 *       Filename:  15-qrb.cpp
 *
 *    Description:  For Google code Jam 2015 Qualification Round Problem B
 *                  Named Infinite House of Pancakes.
 *
 *        Version:  1.0
 *        Created:  04/10/2015 08:24:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kun Ling (), kunling@lingcc.com
 *   Organization:  Lingcc.com
 *
 *          Notes:  Not sure how to solve this problem.
 *                  1. Sort the array so that the number could be maintained well.
 *                  2. The minimum value could take lgVAL or VAL min to be zero.
 *                  3. 
 * =====================================================================================
 */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int least_count( int din_pan) {
  int res_tmp = 0;
  while (din_pan >> 1) {
    res_tmp += 1;
    

  }

}

int process(vector<int> din_vec, int num_din) {
 
  int res = 0;
  if (num_din == 0) {
    return 0;
  } else if (num_din == 1) {
    

  }

  vector<int>::iterator din_iter;
  for(din_iter = din_vec.begin(); din_iter < din_vec.end(); din_iter++) {
    
  }
  return res;
}


int main() {
  int tt_cnt = 0;
  cin >> tt_cnt;

  int i = 0;
  for (; i < tt_cnt; i++) {
    int num_din = 0;
    cin >> num_din;

    vector<int> din_vec;
    int j=0;
    for (; j <= num_din; j++) {
      int  num_p =0 ;
      cin >> num_p;
      din_vec.push_back(num_p);
    }
    sort(din_vec.begin(), din_vec.end());
    int res = process(din_vec, num_din);

    cout <<  "Case #" << i+1 << ": " << res << endl;
  }
  return 0;
}
