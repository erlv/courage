#include <iostream>
#include <vector>

using namespace std;

int count = 0;
vector<int> pos_vec;

bool can_put( int exist_cnt, int pos, int size, int total_cnt) {
  int pos_x = pos / size;
  int pos_y = pos % size;

  if(exist_cnt == total_cnt)
      return true;

  int i=0;
  for(; i < pos_vec.size(); i++) {
    int exist_pos = pos_vec[i];
    int exist_pos_x = exist_pos/size;
    int exist_pos_y = exist_pos % size;
    if( (pos_x - exist_pos_x) == (pos_y - exist_pos_y)
          || (pos_x - exist_pos_x) == (0-(pos_y - exist_pos_y))
          || (pos <= exist_pos))
          return false;
  }

  if( i == exist_cnt) {
      return true;
  }
}


int put_bis( int exist_cnt, int size, int total_cnt) {
    if( (exist_cnt +1) ==  0) {
        count++;
        
        int i = 0;
        cout<<"{" ;
        for(; i<pos_vec.size(); i++) {
            cout<<pos_vec[i] << ",";
        }
        cout << "}" << endl;

        return 0;
    }
    if(exist_cnt < 0 )
        return 0;

    int i=0;
    for(; i < size*size; i++) {
        // Check if the (total_cnt - exist_cnt)th can be put in postion i.
        if(can_put(exist_cnt, i, size, total_cnt)) {
            // If yes, store the position and start to find a position for
            // (total_cnt - exist_cnt +1)th bishop.
            pos_vec.push_back(i);
            exist_cnt--;
            put_bis(exist_cnt, size, total_cnt);
            pos_vec.pop_back();
            exist_cnt++;
        }
    }
}

int main() {
    int n, k;
    cin >> n >> k;
    while( n != 0 && k != 0 ) {
        put_bis(k, n, k);
        cout << count;
        count = 0;
        cin >> n >> k;
    }
}
