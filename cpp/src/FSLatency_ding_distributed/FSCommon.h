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

// Used for multithread read file
#define MAX_THREAD_NUM 256
#define MIN_THREAD_NUM 5

typedef int FD;
typedef int bool;

char G_path[MAX_PATH_LEN] = {"data"};
char* G_filename_w_prefix = "1MB_W_";
char* G_filename_r_prefix = "1MB_R_";
long long G_write_fileCount=0;
long long G_read_fileCount=0;
int G_fileSize=524288;
int G_kfileSize=514;
int G_blockSize=514*1024;
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

pthread_t G_t[READS_PER_WRITE]={0};
int G_thread_idx[READS_PER_WRITE]={0};
sem_t sem_read_start[READS_PER_WRITE];
sem_t sem_read_end[READS_PER_WRITE];


ssize_t read(int fd, void *buf, size_t count);
int close(int fd);
ssize_t write(int fd, const void *buf, size_t count);
int fsync(int fd);
in_addr_t inet_addr(const char *cp);
void single_file_read();
void single_file_read_thread(void* args);
void op_file_read_single_thread();
void op_file_read_multi_thread();
void op_file_read();
// Create the testdir
void create_test_dir();

// Create read_fcnt files with the size: file_size to prepare for file read
void prepare_env();

// Record all the latency into several gloal var for distribution calculation
void record_latency( const long long elapsed );
void Analysis_distribution();

#endif /* _FSLATENCY_DISTRIBUTED_H */
