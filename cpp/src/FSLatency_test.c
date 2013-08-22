/*
 * FS Latency Test Case that support:
 *   1. Write Latency:
 *     1. Append write(aw): Each write will append at the end of the file
 *     2. head write(hw): Each write will write to the head of the file.
 *          There will be no block allocation operation in FS.
 *   2. Read Latency:
 *     1. Append Read(ar): maintain an offset in the program so that the read
 *           can continue to read the full data of the file.
 *     2. head Read(hr): Each read will read from only the head of the file, 
 *           so that it is only read a single block in the cluster.
 *
 *
 * Author: Ling Kun  <lkun.erlv@gmail.com>
 *
 */

/** 
 * TODO:
 *   1. Implement the hw write latency test
 *   2. Implement the ar read latency test
 *   3. Implement the hr read latency test
 *   4. Make the program automatically generate a log file whitch record all latency
 */


#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MAX_COUNT 1024
#define MAX_BLOCK_SIZE 1024*1024
#define FILENAME_FORMAT "%s/%s%04d"
#define MAX_PATH_LEN  90
#define MAX_STR_LEN 128
#define MAX_FILENAME_LEN MAX_STR_LEN

#define AW_FILE_MODE ( O_CREAT|O_APPEND|O_RDWR )
#define HW_FILE_MODE ( O_CREAT|O_RDWR )
#define AR_FILE_MODE ( O_CREAT|O_RDWR )
#define HR_FILE_MODE ( O_CREAT|O_RDWR )

typedef int bool;

enum  test_mode {
  MODE_AW,
  MODE_HW,
  MODE_AR,
  MODE_HR
};

char path[MAX_PATH_LEN] = {0};
char* filename_prefix = "1MB_";

void print_usage() {
  printf("Usage: test path count filesize(Byte) blocksize(Byte) close(1|0) mode(aw|hw|ar|hr) \n");
  printf(" >>> path: a avaible path that can be read/write\n");
  printf(" >>> count: the number of files that need to be read/written\n");
  printf(" >>> filesize: Each file size in Byte that will be write\n");
  printf(" >>> blocksize: Block Size per read/write\n");
  printf(" >>> close: \n");
  printf(" >>> \t 0: Use open-write/read-close steps for each block.\n");
  printf(" >>> \t 1: use  write-sync per block for each block.\n\n");
  printf(" >>> mode: \n");
  printf(" >>> \t aw: write file using append \n");
  printf(" >>> \t hw: write file always to the head of the file \n");
  printf(" >>> \t ar: read to the end of the file \n");
  printf(" >>> \t hr: always read from the head of the file \n");
  printf(" Example: test  data/ 1024 2048576 1048576 1 aw \n");
}

void print_start_information( const int fileCount, const int fileSize, 
    const int blockSize, const bool needclose, const enum test_mode mode) {
  printf("Start test......\n");
  printf(">>> Total File to write: %d\n", fileCount);
  printf(">>> Each File Write Size: %d B\n", fileSize);
  printf(">>> Block Size per write: %d B\n", blockSize);
  printf(">>> File descriptor maintainance mode:");
  if(needclose) {
    printf(" open-write-close per block\n");
  } else {
    printf(" write-sync per block\n");
  }

  char* mode_str;
  switch(mode) {
    case MODE_AW:
      mode_str = "aw";
      break;
    case MODE_HW:
      mode_str = "hw";
      break;
    case MODE_AR:
      mode_str = "ar";
      break;
    case MODE_HR:
      mode_str = "hr";
      break;
  }
  printf(">>> Test Mode is: %s.\n", mode_str);

  printf("\n\n");
}

void init_fds( int* fd, int totalfiles, char* filename_prefix, char* path, int mode ) {
  int i = 0;
  for( ; i < totalfiles; i++) {
    char filename[MAX_FILENAME_LEN];
    snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, path, filename_prefix, i);
    if( ( fd[i] = open(filename, mode, 0666) ) <= 0 )
      printf("Open file error: %s\n", filename);	  
  }
}


void close_fds( int*fd, int totalfiles) {
  int i = 0;
  for( ; i < totalfiles; i++) {
    close(fd[i]);
  }
}

void do_append_write_test(int* fd, const int fileCount, const int fileSize, 
    const int blockSize, const bool needclose) {

  char buf[MAX_BLOCK_SIZE] = {0};
  if(! needclose) {
    // Do not need close:
    // Open file before the test start, mode O_CREAT|O_APPEND|O_SYNC
    init_fds(fd, fileCount, filename_prefix, path, AW_FILE_MODE);
  }

  char mkdircmd[MAX_STR_LEN] = {0};
  sprintf( mkdircmd, "mkdir -p %s", path);
  system(mkdircmd);

  int i=0;
  int j=0;
  for(; (blockSize * j) < fileSize; j++) {

    for(i=0; i < fileCount; i++) {
      char filename[MAX_FILENAME_LEN];
      snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, path, filename_prefix, i);
      struct timeval tv_begin, tv_end;    
      gettimeofday(&tv_begin, NULL);
      int cur_fd;
      if(needclose) {
        cur_fd = open(filename, AW_FILE_MODE, 0666);
        if(cur_fd <= 0 ) {
          printf("Open file error: %s\n", filename);
        }
        write(cur_fd, buf, blockSize);
        close(cur_fd);
      } else {
        cur_fd = fd[i];
        write(cur_fd, buf, blockSize);
        fsync(cur_fd);
      }
      gettimeofday(&tv_end, NULL);

      long long elapsed = ( ( tv_end.tv_sec * 1000000 + tv_end.tv_usec)
          - (tv_begin.tv_sec * 1000000 + tv_begin.tv_usec));
      if(needclose) {
        printf("Need Close: ");
      } else {
        printf("Do not need Close: ");
      }
      printf("%ld us\n", elapsed);
    }
  }  
  if(!needclose) {
    close_fds(fd, fileCount);
  }
}


void do_head_write_test(int* fd, const int fileCount, const int fileSize, 
    const int blockSize, const bool needclose) {

}

void do_append_read_test(int* fd, const int fileCount, const int fileSize, 
    const int blockSize, const bool needclose) {

}

void do_head_read_test(int* fd, const int fileCount, const int fileSize, 
    const int blockSize, const bool needclose) {

}




int main(int argc, char* argv[]) {

  int fd[MAX_COUNT] = {0};

  // variables for the test
  if(argc != 7 ) {
    print_usage();
    exit(0);
  }

  // init path
  strcpy(path, argv[1]);
  // init total file count
  int fileCount = atoi(argv[2]);
  int fileSize = atoi(argv[3]);
  int blockSize = atoi(argv[4]);
  bool needclose = atoi(argv[5]);
  enum test_mode mode;
  if(strcmp(argv[6], "aw")) {
    mode = MODE_AW;
  } else if (strcmp(argv[6], "hw")) {
    mode = MODE_HW;
  } else if (strcmp(argv[6], "ar")) {
    mode = MODE_AR;
  } else if (strcmp(argv[6], "hr")) {
    mode = MODE_HR;
  } else {
    printf("Unknow mode \n");
    print_usage();
    exit(0);
  }

  print_start_information(fileCount, fileSize, blockSize, needclose, mode);

  switch(mode) {
    case MODE_AW:
      do_append_write_test(fd, fileCount, fileSize, blockSize, needclose);
      break;

    case MODE_HW:
      do_head_write_test(fd, fileCount, fileSize, blockSize, needclose);
      break;

    case MODE_AR:
      do_append_read_test(fd, fileCount, fileSize, blockSize, needclose);
      break;

    case MODE_HR:
      do_head_read_test(fd, fileCount, fileSize, blockSize, needclose);
      break;
  }


  return 0;
}
