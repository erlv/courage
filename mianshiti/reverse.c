#include "string.h"
main() {
  char* src="hello.world";
  char* dest = NULL;
  int len=strlen(src);
  dest = (char*) malloc(len);
  char* d=dest;
  char* s=src+len;
  while(len-- != 0)
    *(d++)=*((s--)-1);

  printf("%s",dest);
  return 0;
}
