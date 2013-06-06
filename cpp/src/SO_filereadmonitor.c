#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * This file is used to answer stackoverflow question
 *   http://stackoverflow.com/questions/16725586/read-from-a-file-that-is-continuously-being-updated/16726035#16726035
 *
 * Thu Jun  6 16:21:19 CST 2013
 * Kun Ling <lkun.erlv@gmail.com>
 */
   

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
    
  
  

  
