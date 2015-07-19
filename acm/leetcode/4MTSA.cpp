/*
 * =====================================================================================
 *
 *       Filename:  4MTSA.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/23/2015 06:56:49 AM
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

using namespace std;

class Solution {
  public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
      vector<double> combine;
      int m_1=0; int m_2 = 0;
      while ( m_1 < nums1.size() && m_2 < nums2.size()) {
        if (nums1[m_1] < nums2[m_2]) {
          combine.push_back(nums1[m_1]);
          m_1++;
        } else {
          combine.push_back(nums2[m_2]);
          m_2++;
        }
      }
      if (m_1 == nums1.size()) {
        while (m_2 < nums2.size()) {
          combine.push_back(nums2[m_2]);
          m_2++;
        }
      } else {
        while (m_1 < nums1.size()) {
          combine.push_back(nums1[m_1]);
          m_1++;
        }
      }
      int is_even = !(combine.size()%2);
      int mid_pos =  (combine.size()-1)/2;
      cout << combine.size() << " " <<  is_even << " " << mid_pos << endl;
      double res = combine.size() > 0? 
        (is_even? (combine[mid_pos] + combine[mid_pos+1])/2:combine[mid_pos]):
        0;
      return res;
    }
};


int main() {
  vector<int> v_1, v_2;
  v_2.push_back(2);
  v_2.push_back(3);
  Solution sol;
  double res = sol.findMedianSortedArrays(v_1, v_2);
  cout << res << endl;
  return 0;
}
