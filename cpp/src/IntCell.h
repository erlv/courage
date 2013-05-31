#ifndef INT_CELL_H
#define INT_CELL_H

/**
 * A class for simluateing integer memory cell;
 *
 /

class IntCell {
public:
  explicit IntCell { int initialValue = 0; }
  int read() const;
  void write( int x );
  

private:
  int storedValue;
}
#endif
