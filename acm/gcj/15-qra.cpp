/*
 * =====================================================================================
 *
 *       Filename:  15-qra.cpp
 *
 *    Description:  For Google code Jam 2015 Qualification Round Problem A 
 *                  Named Standing Ovation
 *
 *        Version:  1.0
 *        Created:  04/10/2015 08:24:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kun Ling (), kunling@lingcc.com
 *   Organization:  Lingcc.com
 *
 * =====================================================================================
 */
#include <iostream>
#include <vector>
#include <string>

using namespace std;
int process(vector<int> s_vec, int s_max) {
  int cur_sum = 0;
  int res = 0;
  int cur_pos = 0;

  vector<int>::iterator s_iter;
  for(s_iter = s_vec.begin(); s_iter < s_vec.end(); s_iter++) {
    if (cur_pos == 0 ) {
      cur_sum += *s_iter;
      cur_pos++;
      continue;
    } else {
      if (cur_sum >= cur_pos) {
	cur_sum += *s_iter;
	cur_pos++;
      } else {
	int delta = (cur_pos - cur_sum);
	cur_pos++;
	cur_sum += (*s_iter + delta);
	res += delta;
      }
    }
  }
  return res;
}


int main() {
  int tt_cnt = 0;
  cin >> tt_cnt;

  int i = 0;
  for (; i < tt_cnt; i++) {
    int s_max = 0;
    cin >> s_max;

    string s_vec_str;
    cin >> s_vec_str;
    const char* s_vec_str_ptr = s_vec_str.c_str();

    vector<int> s_vec;
    int j=0;
    for (; j <= s_max; j++) {
      int s_j = s_vec_str_ptr[j] - '0' ;
      s_vec.push_back(s_j);
    }

    int res = process( s_vec, s_max);

    cout <<  "Case #" << i+1 << ": " << res << endl;
  }
  return 0;
}

