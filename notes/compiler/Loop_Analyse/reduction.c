#include <stdio.h>
short a[10][20];
short b[20];
int foo( int n) {
  int result=0;
  int i,j,k;
  for(i=0; i < 10; i++) {
    result = 0;
    for (j=0; j< 10; j++) {
      for (k=0; k < 10; k++) {
	result += a[j][k];
      }
    }
    b[i] = result;
  }
  return b[3];
}

int main() {

  int res= foo(2);
  printf("get %d\n", res);
  return 0;
}
