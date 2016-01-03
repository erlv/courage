#include <stdint.h>
#include <stdio.h>

int main() {
  unsigned char pos=0;
  while ((pos &0x0f) < 9) {
    pos &=0x0f;
    while ( ((pos & 0xf0) >> 4) < 9) {
      if ((pos &0x0f %3) != ((pos &0xf0) >>4)%3) {
        printf("(%c%d, %c%d)\t", ((pos&0x0f)%3+'d'), (10-(pos&0x0f)/3), 
            (((pos&0xf0)>>4)%3+'d'), (((pos&0xf0)>>4)/3+1));
      }
      pos = (pos &0x0f) | ((((pos &0xf0)>>4)+1 &0x0f)<<4);
    }
    pos=pos+1;
  }
  return 0;
}
