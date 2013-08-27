#ifndef _FSLATENCY_DISTRIBUTED_H
#define _FSLATENCY_DISTRIBUTED_H

/*
 * FS Latency Distributed Test Tool
 *
 * Author: Ling Kun  <lingkun@loongstore.com.cn>
 * Loongstore Inc.
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
//#include "threadpool.h"
#include <pthread.h>
#include <semaphore.h>

#define READS_PER_WRITE 5


#define MAX_COUNT 1024
#define MAX_BLOCK_SIZE 1024*1024
#define FILENAME_FORMAT "%s/%s%09lld"
#define MAX_PATH_LEN  160
#define MAX_STR_LEN 128
#define MAX_FILENAME_LEN MAX_STR_LEN
#define MAX_IP_LEN 15
#define MAX_READ_FILE_CNT 50000


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
long long G_fileCount=10;
long long G_fileSize=524288;
long long G_blockSize=514*1024;
bool G_is_distributed=0;
bool G_is_multithread_read=1;
long long G_0_20_ms=0;
long long G_20_40_ms=0;
long long G_40_60_ms=0;
long long G_60_80_ms=0;
long long G_80_100_ms=0;
long long G_100_150_ms=0;
long long G_150_300_ms=0;
long long G_300_ms=0;
long long G_total=0;
ssize_t read(int fd, void *buf, size_t count);
int close(int fd);
ssize_t write(int fd, const void *buf, size_t count);
int fsync(int fd);
in_addr_t inet_addr(const char *cp);

// Used for multithread read file
#define MAX_THREAD_NUM 256
#define MIN_THREAD_NUM 5
pthread_mutex_t lock;
pthread_t G_t[READS_PER_WRITE]={0};
sem_t sem;
volatile int G_thread_read=READS_PER_WRITE;

void single_file_read() {
  long long r_fileCount = G_fileCount;// *(long long int*) arg;
  long long rand_i = rand() % (r_fileCount);
  int blockSize = G_blockSize;//*( int *)(arg+ sizeof(long long int));

  char buf[MAX_BLOCK_SIZE] = {0};
  char filename[MAX_FILENAME_LEN];
  snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, G_path, G_filename_r_prefix, rand_i);
  //printf("Read %s.\n", filename);
  int read_fd = open(filename, O_RDONLY, 0);
  if( read_fd <= 0 ) {
    printf("Open file for read error: %s\n", filename);
  }
  read(read_fd, buf, blockSize);
  close(read_fd);
}

void single_file_read_thread() {
  printf(">>> Threads: start Thread 0x%x and wait\n", (unsigned int)pthread_self());
  while(1) {
    sem_wait(&sem);
    printf(">>>> start read. 0x%x\n", (unsigned int)pthread_self());
    single_file_read();
    pthread_mutex_lock(&lock);
    G_thread_read--;
    pthread_mutex_unlock(&lock);
    
  }
}

void op_file_read_single_thread( const long long r_fileCount, const int blockSize) {
  // Read READS_PER_WRITE-file
  int j=0;
  for(; j < READS_PER_WRITE; j++) {
    single_file_read();
  }
}

void op_file_read_multi_thread(const long long r_fileCount, const int blockSize) {
  // Tell all the thread to start read
  int i=0;
  for(;  i < READS_PER_WRITE; i++) {
    sem_post(&sem);
  }
  //wait all the thread to finish
  int sem_value = -1;
  sem_getvalue(&sem, &sem_value);
  while(sem_value) {
    //printf(">>> Keep on waiting all thread to start read.%d\n", sem_value);
    sem_getvalue(&sem, &sem_value);
  }
  printf(">>> All thread have start read.\n");
  
  // Wait until G_thread_read to 1, that is all threads have done
  // their reads.
  while(G_thread_read > 1 ) {
    continue;
  }
}

void op_file_read(const long long r_fileCount, const int blockSize) {
  if(G_is_multithread_read) {
    op_file_read_multi_thread(r_fileCount, blockSize);
  } else {
    op_file_read_single_thread(r_fileCount, blockSize);
  }
}

// Create the testdir
void create_test_dir() {
  char mkdircmd[MAX_STR_LEN] = {0};
  sprintf( mkdircmd, "mkdir -p %s", G_path);
  system(mkdircmd);
}

/**
 *
 *
 */
void single_step_create( const long long index_start, const long long index_end,
			 int file_size) {

  long long i;
  char buf[MAX_BLOCK_SIZE] = {0};
  int blockSize=G_blockSize;

  for( i= index_start; i < index_end; i++) {
    char filename[MAX_FILENAME_LEN];
    snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, G_path, G_filename_r_prefix, i);
    FD cur_fd = ( FD )open(filename, AW_FILE_MODE, 0666);
    if(cur_fd <= 0 ) {
      printf("Open file error: %s\n", filename);
    }
    int cur_size = 0;
    while(cur_size < file_size ) {
      if( (file_size - cur_size) > blockSize) {
	write(cur_fd, buf, blockSize);
	cur_size += blockSize;
      } else if( (file_size - cur_size) > 0) {
	write(cur_fd, buf, (file_size - cur_size ));
	cur_size = file_size;
      }
    }
    close(cur_fd);    
  }
}

/**
 * Create read_fcnt files with the size: file_size to prepare for file read
 *
 */
void prepare_env(long long read_fcnt, int file_size) {

  printf(">>> Prepare files for read, Total Count:%lld .\n", read_fcnt);
  create_test_dir();

  int SINGLE_STEP=300;
  if( read_fcnt < SINGLE_STEP*400) {
    long long i = 0;
    for( ; i < read_fcnt; i+=SINGLE_STEP) {
      printf("\r>>>> Current process:%3.1f%%.", (float)i/(float)read_fcnt*100);
      fflush(0);
      single_step_create(i, i+SINGLE_STEP, file_size);
    } 
    printf("\r>>>> Current process:%3.1f%%.\n", (float)100);
  } else {
    printf("Too many file creation will take a long time.\n");
  }
  printf(" Prepare Read files Done!\n");
}

// Record all the latency into several gloal var for distribution calculation
void record_latency( const long long elapsed ) {
  G_total++;
  if( elapsed < 20000) {
    G_0_20_ms++;
  } else if ( elapsed < 40000) {
    G_20_40_ms++;
  } else if ( elapsed < 60000) {
    G_40_60_ms++;
  } else if ( elapsed < 80000) {
    G_60_80_ms++;
  } else if ( elapsed < 100000) {
    G_80_100_ms++;
  } else if ( elapsed < 150000) {
    printf("Long Latency 100~150ms: %lld.\n", elapsed);
    G_100_150_ms++;
  } else if ( elapsed < 300000) {
    printf("Long Latency 150~300ms: %lld.\n", elapsed);
    G_150_300_ms++;
  } else if ( elapsed > 300000) {
    printf("Long Latency >300ms: %lld.\n", elapsed);
    G_300_ms++;
  }
}

void Analysis_distribution() {
  float f_0_20_ms = (float)G_0_20_ms/(float)G_total*100;
  float f_20_40_ms = (float)G_20_40_ms/(float)G_total*100;
  float f_40_60_ms = (float)G_40_60_ms/(float)G_total*100;
  float f_60_80_ms = (float)G_60_80_ms/(float)G_total*100;
  float f_80_100_ms = (float)G_80_100_ms/(float)G_total*100;
  float f_100_150_ms = (float)G_100_150_ms/(float)G_total*100;
  float f_150_300_ms = (float)G_150_300_ms/(float)G_total*100;
  float f_300_ms = (float)G_300_ms/(float)G_total*100;
  printf("Latency Distribution:\n");
  printf("Latency(ms)\t0~20\t20~40\t40~60\t60~80\t80~100\t100~150\t150~300\t>300\n");
  printf("Percentage\t%5.2f%%\t%5.2f%%\t%5.2f%%\t%5.2f%%\t%5.2f%%\t%5.2f%%\t%5.2f%%\t%5.2f%%\n",
	 f_0_20_ms, f_20_40_ms, f_40_60_ms,f_60_80_ms, f_80_100_ms, f_100_150_ms,f_150_300_ms,
	 f_300_ms);
}

// End Thread pool related code


#endif /* _FSLATENCY_DISTRIBUTED_H */
