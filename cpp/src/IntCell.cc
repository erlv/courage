#include <iostream>
#include "IntCell.h"

using namespace std;

/**
 * Construct the IntCell with initialVale
 *
 */
 
IntCell::IntCell( int initialValue ) : storedValue( initialValue ) {

}


/**
 * Return the stored value
 *
 */
int IntCell::read() const {
  return storedValue;
}

/**
 * Store x
 *
 */
void IntCell::write( int x ) {
  storedValue = x;
}


IntCell::~IntCell() {
  cout << "IntCell: Call Destructor. storedValue=" 
       << storedValue << endl;
  
}
