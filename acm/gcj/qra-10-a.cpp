#include <iostream>
#include <vector>


using namespace std;
struct GOOD {
  int _price;
  int _pos;
public:
  GOOD(int price, int pos) {
    _price =  price;
    _pos = pos;
  }
};

// Solution 1: the most simple O(n^2) solution.
vector<int> process( vector<GOOD> vec, int cl) {
  int tt_items  = vec.size();
  int i=0; 
  int j=0;
  
  for (; i < tt_items -1; i++) {
    if ( vec[i]._price >= cl) {
      continue;
    }
    for (j=i+1; j < tt_items; j++) {
      if (vec[j]._price + vec[i]._price == cl) {
	vector<int> res;
	res.push_back(i+1);
	res.push_back(j+1);
	return res;
      }
    }
  }
  //  return NULL;
}

int main() {
  int tt_cnt = 0;
  cin >> tt_cnt;

  int i = 0;
  for (; i < tt_cnt; i++) {
    int cl = 0;
    cin >> cl;
    
    int tt_items = 0;
    cin >> tt_items;

    vector<GOOD> good_vec;
    vector<int> res_pos;
    int j=0;
    for (; j < tt_items; j++) {
      int  pos = j;
      int price = 0; 
      cin >> price;
      GOOD new_good(price, pos);
      good_vec.push_back(new_good);
    }

    res_pos = process( good_vec, cl);

    cout <<  "Case #" << i+1 << ": " << res_pos[0] 
	 << " " <<  res_pos[1] << endl;
  }
  return 0;
}
