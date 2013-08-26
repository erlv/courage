#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define READS_PER_WRITE 5


#define MAX_COUNT 1024
#define MAX_BLOCK_SIZE 1024*1024
#define FILENAME_FORMAT "%s/%s%04d"
#define MAX_PATH_LEN  90
#define MAX_STR_LEN 128
#define MAX_FILENAME_LEN MAX_STR_LEN
#define MAX_IP_LEN 15



#define AW_FILE_MODE ( O_CREAT|O_APPEND|O_RDWR )
#define HW_FILE_MODE ( O_CREAT|O_RDWR )
#define AR_FILE_MODE ( O_CREAT|O_RDWR )
#define HR_FILE_MODE ( O_CREAT|O_RDWR )

enum  test_mode {
  MODE_AW,
  MODE_HW,
  MODE_AR,
  MODE_HR
};

typedef int FD;
typedef int bool;



int G_port=0;
char G_ipaddr[MAX_IP_LEN]={"127.0.0.1"};
char G_path[MAX_PATH_LEN] = {"data"};
char* G_filename_r_prefix = "1MB_R_";
char* G_filename_w_prefix = "1MB_W_";
int G_fileCount=10;
int G_fileSize=524288;
int G_blockSize=52428;
int G_needclose=1;
int G_0_20_us=0;
int G_20_40_us=0;
int G_40_60_us=0;
int G_60_80_us=0;
int G_80_100_us=0;
int G_100_150_us=0;
int G_150_300_us=0;
int G_300_us=0;

ssize_t read(int fd, void *buf, size_t count);
int close(int fd);
ssize_t write(int fd, const void *buf, size_t count);
int fsync(int fd);
in_addr_t inet_addr(const char *cp);


void op_file_read( const int r_fileCount, const int blockSize) {
  // Read READS_PER_WRITE-file
  int j=0;
  char buf[MAX_BLOCK_SIZE] = {0};

  for(; j < READS_PER_WRITE; j++) {
    char filename[MAX_FILENAME_LEN];

    //Generate a random int between [0:READS_PER_WRITE*fileCount]
    int rand_i = rand() % (r_fileCount);
    snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, G_path, G_filename_r_prefix, rand_i);
    printf("Read %s.\n", filename);
    int read_fd = open(filename, O_RDONLY, 0);
    if( read_fd <= 0 ) {
      printf("Open file for read error: %s\n", filename);
    }
    read(read_fd, buf, blockSize);
    close(read_fd);
  }
}

// Create the testdir
void create_test_dir() {
  char mkdircmd[MAX_STR_LEN] = {0};
  sprintf( mkdircmd, "mkdir -p %s", G_path);
  system(mkdircmd);
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
    printf("prepare file:%s.\n", filename);
    FD cur_fd = ( FD )open(filename, AW_FILE_MODE, 0666);
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
