#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef int FD ;

int main() {
  FD filed = open("/tmp/test_inotify", O_RDWR );
  char buf[128];

  if( !filed ) {
    printf("Openfile error\n");
    exit(-1);
  }

  int nbytes;
  while(1) {
    nbytes = read(filed, buf, 16);
    printf("read %d bytes from file.\n", nbytes);
    if(nbytes > 0) {
      split_buffer_by_newline(buf); // split buffer by new line.
    }
    sleep(1);
  }
  return 0;
}
    
  
  

  
