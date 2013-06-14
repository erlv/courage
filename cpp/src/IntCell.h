#ifndef INTCELL_H

#define INT_CELL_H

/**
 * A class for simluateing integer memory cell;
 *
 */

class IntCell {
public:
  explicit IntCell (int initialValue = 0 );
  int read() const;
  void write( int x );
  ~IntCell() ;
  IntCell( const IntCell &rhs);
  const IntCell& :operator=( const IntCell& rhs);

private:
  int storedValue;
};

#endif
