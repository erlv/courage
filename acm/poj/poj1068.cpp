//============================================================================
// Name        : poj1068.cpp
// Author      : lingkun
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
using namespace std;

vector<int> compute_q( vector<char> origin, int tt) {
	vector<int> dest;

	vector<char>::iterator ori_iter= origin.begin();
	for(; ori_iter < origin.end(); ori_iter++) {
		if( *ori_iter != ')') continue;

		vector<char>::iterator ori_iter_inner = ori_iter;
		int match_count = 0;
		for(; ori_iter_inner >= origin.begin(); ori_iter_inner--) {
			if(*ori_iter_inner == ')') match_count++;
			else if(*ori_iter_inner == '(') match_count--;

			if(match_count == 0) {
				dest.push_back((ori_iter - ori_iter_inner+1)/2);
				break;
			}
	
		}
	}
	return dest;
}

vector<char> roll_back( vector<int> src, int tt) {
	vector<char> result;
	vector<int>::iterator src_iter;
	int prev,curr;
	prev=0;
	for( src_iter=src.begin(); src_iter < src.end(); src_iter++){
		curr = *src_iter;

		int j;
		for(j=0; j  < (curr-prev); j++) {result.push_back('(');}
		result.push_back(')');
		prev = curr;
	}

	return result;
}

vector<int> convert( vector<int> src, int tt ) {
	vector<int> dest;
	vector<char> origin_par = roll_back( src, tt);
	dest = compute_q(origin_par, tt);
	return dest;
}


int main () {

	int tt_case;
	cin >> tt_case;

	int i;
	for( i=0; i < tt_case; i++) {
		int p_pare;
		vector<int> pare_vec_src, pare_vec_dest;

		cin >> p_pare;

		int j;
		for(j=0; j < p_pare; j++) {
			int pare;
			cin >> pare;
			pare_vec_src.push_back(pare);
		}
		pare_vec_dest = convert( pare_vec_src, p_pare);


		vector<int>::iterator pare_iter;
		for( pare_iter=pare_vec_dest.begin(); pare_iter < (pare_vec_dest.end()-1);
				pare_iter++) {
			cout << *pare_iter << " ";

		}
		cout << *(pare_vec_dest.end()-1) << endl;
	}
	return 0;
}
