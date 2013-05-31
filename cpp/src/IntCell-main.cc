#include <iostream>
#include "IntCell.h"

using namespace std;

int main() {
  IntCell m(0);

  m.write( 5 );
  cout << "Stack allocation Cell contents: " << m.read() << endl;

  // Do not use "new IntCell()", which is like a function declaration
  IntCell* n = new IntCell; 
  n->write( 12 );
  cout << "Dynamic allocated IntCell: " << n->read() << endl;
  delete n;

  return 0;
}
