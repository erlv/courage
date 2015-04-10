/*
 * =====================================================================================
 *
 *       Filename:  08-r1a.cpp
 *
 *    Description:  For GCJ problem https://code.google.com/codejam/contest/32016/dashboard#s=p0 
 *
 *        Version:  1.0
 *        Created:  04/08/2015 08:31:41 PM
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
#include <algorithm>
using namespace std;
// For quick problem solver, using long long will help to handle overflow problems.
// So try to use big data types.
// strtok(): a cstring function. Used to find the next token in a string. 
//            we could also specify the seperator.

long long min_scalar_product( vector<long long> a, vector<long long> b) {
  long long res_1;
    
  vector<long long>::iterator iter_a, iter_b;
  vector<long long>::reverse_iterator riter_a, riter_b;

  res_1 = 0;
  iter_a = a.begin(); riter_b  = b.rbegin();
  for (; iter_a < a.end(); iter_a++) {
    res_1 += (*iter_a * *riter_b);
    riter_b++;
  }

  return res_1;
}


int main() {
  int tt_cnt =0 ;
  cin >> tt_cnt;
  
  int i;
  for (i=0; i < tt_cnt; i++) {
    int v_len = 0;
    cin >> v_len;
    vector<long long> v_1;
    vector<long long> v_2;
    
    int j=0;
    for(; j < v_len; j++) {

      int tmp =0;
      cin >> tmp;
      v_1.push_back(tmp);
    }
    for(j=0; j < v_len; j++) {
      int tmp=0;
      cin >> tmp;
      v_2.push_back(tmp);
    }
    sort(v_1.begin(), v_1.end());
    sort(v_2.begin(), v_2.end());
  //  print_vec(v_1);
   // print_vec(v_2);

    long long res = min_scalar_product(v_1, v_2);
    cout << "Case #" << i+1 << ": " << res << endl; 
  }
  return 0;
}

