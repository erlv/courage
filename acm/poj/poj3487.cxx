#include <iostream>
#include <stdlib.h>

#define MAX_SIZE 26

using namespace std;

int male[MAX_SIZE];
int female[MAX_SIZE];
int male_pref_mtx[MAX_SIZE][MAX_SIZE];
int female_pref_mtx[MAX_SIZE][MAX_SIZE];
int male_eng_arr[MAX_SIZE];

void init_all_data( int tt_pairs) {
    
  int j;
  for( j = 0; j < tt_pairs; j++) {
    cin >> male[j];
  }
  for( j = 0; j < tt_pairs; j++) {
    cin >> female[j];
  }

  for( j = 0; j < tt_pairs; j++) {
    // male prefer matrix
    int male_name;
    cin >> male_name;
    
    int k;
    for( k = 0; k < tt_pairs; k++) {
      if( male_name == male[k] )
	break;
    }
    int male_index = k;    
    for( k = 0; k < (tt_pairs+1); k++) {
      int prefer_female_name;
      cin >> prefer_female_name;
      if(k == 0) {
	if( prefer_female_name != ':' ) {
	  cout << "Error: input error";
	  exit(2);
	} else 
	  // ignore the ':'
	  continue;
      }
      cin >> male_pref_mtx[male_index][k-1];
    }
  }

  for( j = 0; j < tt_pairs; j++) {
    // male prefer matrix
    int female_name;
    cin >> female_name;
    
    int k;
    for( k = 0; k < tt_pairs; k++) {
      if( female_name == female[k] )
	break;
    }
    int female_index = k;
    
    for( k = 0; k < (tt_pairs+1); k++) {
      int prefer_male_name;
      cin >> prefer_male_name;
      if(k == 0) {
	if( prefer_male_name != ':' ) {
	  cout << "Error: input error";
	  exit(2);
	} else 
	  // ignore the ':'
	  continue;
      }
      cin >> female_pref_mtx[female_index][k-1];
    }
  }
}

int is_free( int is_male, int index, int tt_pairs) {

  if( is_male == 1 ) {
    return male_eng_arr[index];
  } else {
    int female_name = female[index];
    int i;
    for( i =0; i < tt_pairs; i++) {
      if( female_name == male_eng_arr[i] )
	return false;
    }
    if( i == tt_pairs ) return true;
  }
    
}


void compute_engage( int tt_pair) {

  // start from the current free male
  int i=0;
  while( !is_free( true, i, tt_pair) )
    i++;

  //loop: get female from its pref_mtx
  int j = 0;
  for(; j < tt_pair; i++) {
    // is_female free?
    int female_candidiate = male_pref_mtx[i][j];
    if(is_free( false, female_candidiate, tt_pair)) {
      // yes, get engage
      male_eng_arr[i] = female_candidiate;
    } else {
      // no, does the female prefer this guy?
      
    }

  
    
    // yes, make the previous male free, engage the current male and female
    // no, get the next female from the prefer list, and continue the loop

    // until no free guys
  
  }


  void print_result ( int tt_pair) {
    int i;
    for( i = 0; i < tt_pair; i++) {
      cout << male[i] << ' ' << male_eng_arr[i] << endl;
    }
  }

  int main () {

    int total_test;
    cin >> total_test;

    int i;
    for( i=0; i < total_test; i++) {

      int all_pairs;
      cin >> all_pairs;

      // Get one single test case data, and store in global data
      init_all_data( all_pairs); 

      compute_engage( all_pairs);
    
      print_result( all_pairs);
      cout << endl;
    }

    return 0;

  }
