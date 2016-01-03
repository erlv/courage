#include <vector>
#include <iostream>

/*
 * INCORRECT SOLUTION:
 *   The problem is not about get the mini price when we know how many we buy for each
 *   book.
 *   The problem is about the mini price when we know the total number of books we buy,
 *   we could have any number of books for each volumn.
 */

using namespace std;

float sales[5]={1, 0.95, 0.90, 0.80, 0.75};

float get_min_price(vector<int>& vec) {
  float res=0;
  while (!(vec[0] == 0 && vec[1] == 0 && vec[2] == 0
	   && vec[3] == 0 && vec[4] == 0)) {
    int count = 0;
    for(int i=0; i < 5; i++) {
      if (vec[i] != 0) {
	count++;
	vec[i]--;
      }
    }
    float cur_sale = sales[count-1];
    float cur_price = 8*count*cur_sale;
    res += cur_price;
  }
  return res;
}

int main() {
  vector<int> book_cnt;
  int i=0;
  for (; i < 5; i++) {
    int temp=0;
    cin >> temp;
    book_cnt.push_back(temp);
  }
  cout << "Lowest Price:" << get_min_price(book_cnt) << endl;
  return 0;
}
