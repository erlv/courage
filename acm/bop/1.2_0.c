#include <stdint.h>
#include <stdio.h>

struct pos {
  uint8_t j:4;
  uint8_t s:4;
};

int main() {
  struct pos two_pos;
  two_pos.j=0;
  while (two_pos.j < 9) {
    two_pos.s=0;
    while (two_pos.s < 9) {
      if (two_pos.j%3 != two_pos.s%3) {
        printf("(%c%d, %c%d)\t", (two_pos.j%3+'d'), (10-two_pos.j/3), 
               (two_pos.s%3+'d'), (two_pos.s/3+1));
      }
      two_pos.s++;
    }
    two_pos.j++;
  }
  return 0;
}
