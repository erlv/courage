/**************************************************
 * It is a DP problem from POJ 2342 and CLRS 15-4
 * Hard point:
 * 1. How to find the root of the tree
 * 2. Hard to think about how to record the subproblem's solution
 * My week point:
 * 1. Can not figure why the dfs strategy works
 * 2. Can not use 2-D array to record the subproblem solutions
 *
 * Ling Kun (lingcc@lingcc.com)
 **************************************************/

#include <iostream>
#include <vector>
#include <stdio.h>
using namespace std;

#define MAX_PEOPLE_COUNT 6000
int conviviality[MAX_PEOPLE_COUNT]={0};

vector<int> relations[MAX_PEOPLE_COUNT]; // An array of vectors
char f[MAX_PEOPLE_COUNT];
int d[MAX_PEOPLE_COUNT][2];

void dfs(int x) {
  int i, j;
  int m =  relations[x].size(); // How many employees x has?

  for(i=0; i < m; i++)
    dfs(relations[x][i]); // Recursivly visit all its employees, deep first visit

  d[x][1] = conviviality[x]; // If the root will be selected
  d[x][0] = 0; // If the root can not be selected

  if(!m) // If current guy do not have any employee
    return;

  for( i=0; i < m; i++) {
    j = relations[x][i]; // The index of current employee
    d[x][1] += d[j][0]; // If the suppervisor is selected, all employees will not be selected

    // If suppervisor is not selected, all employees can be selected or not.
    // However, we only choose the max ones
    d[x][0] += max(d[j][0], d[j][1]); 
    
  }
}

int main() {
  int tt_employees;
  cin >> tt_employees;
  
  int i;
  for( i=0; i < tt_employees; i ++) {
    cin >> conviviality[i]; // Get the conviviality
  }
  
  int employee, supervisor;
  while(scanf("%d %d", &employee, &supervisor ), supervisor+employee) {
    employee--;
    supervisor--; // In the input, all index start from "1",not "0"
    relations[supervisor].push_back(employee); // Get the relation ship of employment
    f[employee] = supervisor; // Set the supervior not be a leaf of the tree
  }
    
  for(i=0; f[i] !=0;)
	 i = f[i];  // Find the root of the tree

  dfs(i); 
  cout << max(d[i][0], d[i][1]) << endl; 

}
