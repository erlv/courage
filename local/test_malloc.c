#include <stdio.h>
#include <stdlib.h>

void * __wrap_malloc( size_t size) {
  void* result;
  //result= __real_malloc( size);
  result = memalign(32, size);
  printf("Malloc: allocate %d byte mem, start at %p", size, result);

  return result;
}


int main ()
{
  int i,n;
  char * buffer;

  printf ("How long do you want the string? ");
  //scanf ("%d", &i);
  i = 200;

  buffer = (char*) malloc (i+1);
  if (buffer==NULL) exit (1);

  for (n=0; n<i; n++)
    buffer[n]=rand()%26+'a';
  buffer[i]='\0';

  printf ("Random string: %s\n",buffer);
  free (buffer);

  return 0;
}

