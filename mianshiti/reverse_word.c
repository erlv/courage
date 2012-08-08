#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char str[100];
char new_str[100];

void  ReverseWord( ) {
  int str_len = strlen(str);
  int i,j;
  i = 0;
  j = 0;

  new_str[str_len-1] = '\0';
  
  for(; i <(str_len -1); i++) {
    if( str[i] != ' ')
      continue;

    int k;
    for (k = i -1 ; k >= j; k--) {
      new_str[str_len -i + k -j] = str[k];
    }
    j = i;
    new_str[str_len -1 -j ] = ' ';
  }
}

int main() {

  printf("Please input the string:");
  gets(str);
  ReverseWord();
  printf("Result:%s.\n", new_str);
}
