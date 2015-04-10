/*
 * =====================================================================================
 *
 *       Filename:  qra-10-b.cpp
 *
 *    Description:  Qualification Round Africa 2010 Problem B
 *
 *        Version:  1.0
 *        Created:  03/26/2015 06:11:12 PM
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
#include <sstream>
#include <iterator>
#include <algorithm>

using namespace std;

void output_vector_string(string prefix, vector<string> tokens) {
  cout << prefix;
  
  vector<string>::reverse_iterator token_iter = tokens.rbegin();
  for (; token_iter < tokens.rend(); token_iter++) {
    cout << " " << *token_iter;
  }
  cout << endl;
}

int main() {
  int tt_cnt=0;
  cin >> tt_cnt;

  int i=0;
  for (; i < tt_cnt; i++) {
    string sen;
    getline(cin, sen);
    istringstream iss(sen);
    vector<string> tokens;
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(tokens));
  
    ostringstream stringStream;
    stringStream << "Case #" << i+1 << ":";
    string prefix = stringStream.str(); // How to use string stream to construct a string.
    output_vector_string(prefix, tokens);
  }
  return 0;
}
