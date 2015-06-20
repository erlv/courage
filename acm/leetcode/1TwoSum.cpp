/*
 * =====================================================================================
 *
 *       Filename:  1TwoSum.cpp
 *
 *    Description:  https://leetcode.com/problems/two-sum/
 *
 *        Version:  1.0
 *        Created:  05/26/2015 07:12:53 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kun Ling (), kunling@lingcc.com
 *   Organization:  Lingcc.com
 *
 * =====================================================================================
 */
#include <vector>
#include <iostream>
#include <algorithm>

// C++:
//  1. declared without init vector will be empty.
//  2. 
using namespace std;

void print( vector<int> a) {
  if ( a.empty()) {
    cout << "vector is empty" << endl;
    return;
  }
  int i=0;
  for (; i < a.size(); i++) {
    cout << a[i] << ", ";
  }
  cout << endl;
}

#if 0
class Solution {
  // Time Limit Exceeded Solution
  public:
    vector<int> twoSum(vector<int>& nums, int target) {
      int i,j;
      vector<int> res;
      for (i=0; i < nums.size()-1; i++) {
        for(j=i+1; j < nums.size(); j++) {
          if ((nums[i] + nums[j]) == target) {
            res.push_back(i+1);
            res.push_back(j+1);
            return res;
          }
        }
      }
      return res; // return empty res
    }
};
#endif

class node {
public:
   int value;
   int pos;
   // does return value necessary for constructor ?
   node(int a, int b) {
      value = a;
      pos = b;
   }
};



bool node_cmp (node a, node b) {
    return a.value > b.value;
}



class Solution {
  public:
    vector<int> twoSum(vector<int>& nums, int target) {
       vector<node> node_nums;
       int i=0;
       for (; i < nums.size(); i++) {
          class node tmp = node (nums[i], i);
          node_nums.push_back(tmp);
        }
        sort(node_nums.begin(), node_nums.end(), node_cmp);  // How to sort with customized compare function?
        vector<int> res;
        int l, r;
        l = 0;
        r = node_nums.size()-1;
        while ( l < r) {
           int sum = node_nums[l].value + node_nums[r].value;
           if (sum == target) {
              res.push_back(node_nums[l].pos);
              res.push_back(node_nums[r].pos);
              break;
           } else if (sum < target) {
             l++;
           } else {
             r--;
           }
        }
        sort(res.begin(), res.end());
        return res;
    }
};

int main() {
  Solution sol; // Can we init a class this way on stack
  vector<int> in {591,955,829,805,312,83,764,841,12,744,104,773,627,306,731,539,349,811,662,341,465,300,491,423,569,405,508,802,500,747,689,506,129,325,918,606,918,370,623,905,321,670,879,607,140,543,997,530,356,446,444,184,787,199,614,685,778,929,819,612,737,344,471,645,726};
  int target = 789;
  vector<int> out = sol.twoSum(in, target);
  print(out);
  return 0;
}
