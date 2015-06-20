/*
 * =====================================================================================
 *
 *       Filename:  3LSWRC.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/28/2015 07:19:11 AM
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

class Solution {
public:
  int lengthOfLongestSubstring (string s) {
    vector<int> posLongest;
    int i,j;
    for( i=0; i < (int)s.length()-1; i++) {
      int tmp =0;
      for(j=i+1; j < (int)s.length(); j++) {
        if (s[i] != s[j]) {
          tmp++;
        } else {
          posLongest.push_back(tmp);
          break;
        }
      }
    }
    if (s.length() > 0) {
      posLongest.push_back(1);
    }
    int res= 0;
    for(i=0; i < posLongest.size(); i++) {
      if (posLongest[i] > res) {
        res = posLongest[i];
      }
    }
    return res;
  }
};

int main() {
        string s("");
        Solution* sol = new Solution();
        int res = sol->lengthOfLongestSubstring(s);
        cout << res << endl;
        delete sol;
        return 0;
}
