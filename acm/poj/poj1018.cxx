#include <iostream>
#include <vector>
using namespace std;

struct dev {
  int bandwith;
  int price;
};

// vector<dev> contains all the dev candidate for current dev
// vector<vector<dev>> contains the whole systems candidate
vector< vector < dev > > com_sys;

struct dev get_max( vector<dev> current_dev_vec, struct dev current_max ) {

  struct dev new_max_dev;// = new dev;
  vector<dev>::iterator manu_iter;
  float max_bp = 0;
  for( manu_iter = current_dev_vec.begin();
       manu_iter != current_dev_vec.end(); manu_iter++) {
    
    int new_bw;
    if( current_max.bandwith == 0 )
      new_bw = manu_iter->bandwith;
    else
      new_bw = min( current_max.bandwith, manu_iter->bandwith);
    
    int new_price = current_max.price + manu_iter->price;
    float new_bp = (float)new_bw/(float)new_price;
    if( max_bp < new_bp ) {
      max_bp = new_bp;
      new_max_dev.bandwith = new_bw;
      new_max_dev.price = new_price;
    }
  }
  return new_max_dev;
}

int main() {
  int tt;
  cin >> tt;
  
  int i;
  for( i=0; i < tt; i++) {
    int tt_dev;
    cin >> tt_dev;
    
    int j;
    for(j=0; j < tt_dev; j++) {
      int tt_manuf;
      cin >> tt_manuf;
      
      int k;
      vector<dev> dev_vec;
      for(k=0; k < tt_manuf; k++) {
	struct dev dev_can;
	cin >> dev_can.bandwith >> dev_can.price;
	dev_vec.push_back(dev_can);
      }
      com_sys.push_back(dev_vec);
    }
    // END: input for current tc done

    // Begin: start to compute, greedy + enumeration
    int current_bw = 0;
    int tt_price = 0;
    struct dev current_max;// = new dev;
    current_max.bandwith = 0;
    current_max.price = 0;
    for( j=0; j < tt_dev; j++ ) {
      vector<dev> dev_vec = com_sys[j];
      current_max = get_max( dev_vec, current_max);
    }
    cout.precision(3); // what is mean by cout << setprecision(3) ?
    cout <<  ((float)current_max.bandwith / (float)current_max.price) << endl;
  }
}
