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
 *   1. Distributed mode for read/write file test
 *   2. Write is client, read is sever
 *   3. The whole work flow:
 *      a. Client write 1 file
 *      b. client tell the server to read 5 files.
 *      c. server read 5 files, and tell client that I have done
 *      d. client report the whole latency
 *
 *
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

char G_path[MAX_PATH_LEN] = {0};
char* G_filename_r_prefix = "1MB_R_";
char* G_filename_w_prefix = "1MB_W_";

void print_usage() {
  printf("Usage: test path testcount filesize(Byte) blocksize(Byte) close(1|0) \n");
  printf(" >>> path: a avaible path that can be read/write\n");
  printf(" >>> count: the number of files that need to be read/written\n");
  printf(" >>> filesize: Each file size in Byte that will be write\n");
  printf(" >>> blocksize: Block Size per read/write\n");
  printf(" >>> close: \n");
  printf(" >>> \t 0: Use open-write/read-close steps for each block.\n");
  printf(" >>> \t 1: use  write-sync per block for each block.\n\n");
  printf(" Example: test  data/ 500 524288 524288 1\n");
}

void print_start_information( const int fileCount, const int fileSize, 
    const int blockSize, const bool needclose, const enum test_mode mode) {
  printf("Start test......\n");
  printf(">>> Total File to write: %d\n", fileCount);
  printf(">>> Total File to read: %dx5=%d\n", fileCount, 5*fileCount);
  printf(">>> Each File Read/Write Size: %d B\n", fileSize);
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
  }
  printf(">>> Test Mode is: %s.\n", mode_str);
  printf("\n\n");
}

void init_fds( int* fd, int totalfiles, char* filename_prefix, int mode ) {
  int i = 0;
  for( ; i < totalfiles; i++) {
    char filename[MAX_FILENAME_LEN];
    snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, G_path, filename_prefix, i);
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

void do_file_read_test(const int fileCount, const int blockSize, const bool needclose) {
  // Read 5-file
  int j=0;
  for(; j < 5; j++) {
    char filename[MAX_FILENAME_LEN];

    //Generate a random int between [0:5*fileCount]
    int rand_i = rand() % (5*fileCount);
    snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, G_path, G_filename_r_prefix, rand_i);

    int read_fd = open(filename, O_RDONLY, 0);
    if( read_fd <= 0 ) {
      printf("Open file for read error: %s\n", filename);
    }
    read(read_fd, buf, blockSize);
    close(read_fd);
  }

}

void do_file_write_test(const int fileCount, const int blockSize, const bool needclose) {

  // Write 1-file
  char filename[MAX_FILENAME_LEN];
  snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, G_path, G_filename_w_prefix, i);
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


}

void do_append_write_test(int* fd, const int fileCount, const int fileSize, 
    const int blockSize, const bool needclose) {

  char buf[MAX_BLOCK_SIZE] = {0};
  if(! needclose) {
    // Do not need close:
    // Open file before the test start, mode O_CREAT|O_APPEND|O_SYNC
    init_fds(fd, fileCount, G_filename_w_prefix, AW_FILE_MODE);
  }

  char mkdircmd[MAX_STR_LEN] = {0};
  sprintf( mkdircmd, "mkdir -p %s", G_path);
  system(mkdircmd);

  int i=0;
  int j=0;
  for(; (blockSize * j) < fileSize; j++) {

    // Each file only read/write a single blocksize of data
    for(i=0; i < fileCount; i++) {

      struct timeval tv_begin, tv_end;    
      gettimeofday(&tv_begin, NULL);

      do_file_read_test(fileCount, blockSize, needclose);
      do_file_write_test(fileCount, blockSize, needclose);

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

/**
 * Create read_fcnt files with the size: file_size to prepare for file read
 *
 */
void prepare_env(int read_fcnt, int file_size) {

  printf("prepare env, read_fcnt:%d.\n", read_fcnt);

  char buf[MAX_BLOCK_SIZE] = {0};
  int blockSize=512*1024;

  int i = 0;
  for( ; i < read_fcnt; i++) {
    char filename[MAX_FILENAME_LEN];
    snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, G_path, G_filename_r_prefix, i);
    //printf("prepare file:%s.\n", filename);
    int* cur_fd = open(filename, AW_FILE_MODE, 0666);
    if(cur_fd <= 0 ) {
      printf("Open file error: %s\n", filename);
    }
    int cur_size = 0;
    while(cur_size < file_size ) {
      if( file_size - cur_size > blockSize) {
        write(cur_fd, buf, blockSize);
        cur_size += blockSize;
      } else {
        write(cur_fd, buf, (file_size - cur_size ));
        cur_size = file_size;
      }
    }
    close(cur_fd);
  }
}


int main(int argc, char* argv[]) {

  int fd[MAX_COUNT] = {0};

  // variables for the test
  if(argc != 6 ) {
    print_usage();
    exit(0);
  }

  // init path
  strcpy(G_path, argv[1]);
  // init total file count
  int fileCount = atoi(argv[2]);
  int fileSize = atoi(argv[3]);
  int blockSize = atoi(argv[4]);
  bool needclose = atoi(argv[5]);
  enum test_mode mode;

  mode = MODE_AW;
  print_start_information(fileCount, fileSize, blockSize, needclose, mode);

  //Create 1000 files frist for read support
  int read_fileCount = 5* fileCount;
  prepare_env(read_fileCount, fileSize);

  switch(mode) {
    case MODE_AW:
      do_append_write_test(fd, fileCount, fileSize, blockSize, needclose);
      break;
  }

  return 0;
}
