//      swopt.cxx
//      
//      Copyright 2012 lingkun <lingkun@lingkun-LifeBook-V1020>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.
//      
//      


#include <iostream>

#include <cstring>
#include <sstream>

using namespace std;

const int MAX_DIM = 1025;
const int INF = 1073741824;

#define max(a, b) ((a)>(b)?a:b)

class AffineSWOptimization {
public:
	int M[MAX_DIM][MAX_DIM];
	int H[MAX_DIM][MAX_DIM];
	int V[MAX_DIM][MAX_DIM];


	// input:
	//   b: the value of b, the target. Contains 1-1024 chars
	//   a: the value of a, the query. Contains 1-1024 chars
	//   qsc: the query_start_clip 0/1
	//   qec: the query_end_clip 0/1
	//   mm: the value of match source. int 1-10
	//   mi: the value of mismatch source. int -1 - -10
	//   o: the value of gap_open. int -10 - -2
	//   e: the value of gap extension. int  (o+1) - -1
	//   dir: direction 0/1
	// return: string "opt query_end target_end n_best"
	//    opt: the  value of the optimal alignment
	//    n_best: the number of different pairs (i,j). 1<= i <= m,
	//        1 <= j <= n.
	//    (query_end, target_end) is the location of the cell 
	//        corresponding to the optimal alignment.
	string process(string b, string a, int qsc, int qec,
		int mm, int mi, int o, int e, int dir) {
		int n = b.size(), m = a.size();

		int id = -1;
		if (qsc == 1 && qec == 1) id = 1;
		if (qsc == 0 && qec == 1) id = 2;
		if (qsc == 1 && qec == 0) id = 3;
		if (qsc == 0 && qec == 0) id = 4;

		if (id == 1 || id == 3) {
			for (int i=0; i <= m; i++) M[i][0] = 0;
			for (int j=0; j <= n; j++) M[0][j] = 0;
			for (int i=0; i <= m; i++) H[i][0] = V[i][0] = -INF;
			for (int j=0; j <= n; j++) H[0][j] = V[0][j] = -INF;

			for (int i=1; i <= m; i++)
				for (int j=1; j <= n; j++) {
					V[i][j] = max(M[i-1][j] + o + e, V[i-1][j] + e);
					H[i][j] = max(M[i][j-1] + o + e, H[i][j-1] + e);
					int mx = max(max(M[i-1][j-1], V[i-1][j-1]), H[i-1][j-1]);
					M[i][j] = max(0, mx + (a[i-1] == b[j-1] ? mm : mi));
				}
		} else {                                      
			for (int j=0; j <= n; j++) M[0][j] = 0;
			for (int i=1; i <= m; i++) M[i][0] = -INF;
			for (int j=0; j <= n; j++) H[0][j] = V[0][j] = -INF;
			for (int i=1; i <= m; i++) {
				V[i][0] = -INF;
				H[i][0] = o + e * i;
			}

			for (int i=1; i <= m; i++)
				for (int j=1; j <= n; j++) {
					V[i][j] = max(M[i-1][j] + o + e, V[i-1][j] + e);
					H[i][j] = max(M[i][j-1] + o + e, H[i][j-1] + e);
					int mx = max(max(M[i-1][j-1], V[i-1][j-1]), H[i-1][j-1]);
					M[i][j] = mx + (a[i-1] == b[j-1] ? mm : mi);
				}
		}

		int minI = 1, maxI = m, minJ = 1, maxJ = n;
		if (id == 3 || id == 4) minI = m;

		int opt = -INF, query_end = -1, target_end = -1, n_best = 0;
                              
		for (int i=minI; i <= maxI; i++)
			for (int j=minJ; j <= maxJ; j++) {
				opt = max(opt, M[i][j]);
				opt = max(opt, V[i][j]);
				opt = max(opt, H[i][j]);
			}

		n_best = 0;
		for (int i=minI; i <= maxI; i++)
			for (int j=minJ; j <= maxJ; j++)
				if (M[i][j] == opt || V[i][j] == opt || H[i][j] == opt)
					n_best++;

		if (dir == 0) {
			for (int i=minI; i <= maxI && query_end == -1; i++)
				for (int j=minJ; j <= maxJ && query_end == -1; j++)
					if (M[i][j] == opt || V[i][j] == opt || H[i][j] == opt) {
						query_end = i-1;
						target_end = j-1;
					}
		} else {
			for (int i=maxI; i >= minI && query_end == -1; i--)
				for (int j=maxJ; j >= minJ && query_end == -1; j--)
					if (M[i][j] == opt || V[i][j] == opt || H[i][j] == opt) {
						query_end = i-1;
						target_end = j-1;
					}
		}

		ostringstream oss;
		oss << opt << " " << query_end << " " << target_end << " " << n_best;

		return oss.str();
	}
};

int main(int argc, char **argv)
{
	
	return 0;
}

