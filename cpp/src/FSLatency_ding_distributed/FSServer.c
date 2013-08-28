/*
 * FS Latency Distributed Test Tool
 *
 * Author: Ling Kun  <lingkun@loongstore.com.cn>
 * Loongstore Inc.
 *
 */

//#include "FSCommon.h"
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
char G_ipaddr[MAX_IP_LEN]={"127.0.0.1"};
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


void print_server_usage() {
  printf("Usage: FSLatency-s count filesize(KB) MultiReader(0|1) port(int)\n");
  printf(" >>> path: a avaible path that can be read/write\n");
  printf(" >>> count: the number of files that need to be read/written\n");
  printf(" >>> filesize: Each file size in KB that will be write\n");
  printf(" >>> MultiReader:\n");
  printf(" >>> \t 0: Use single thread to do %d file read.\n", READS_PER_WRITE);
  printf(" >>> \t 1: Use multi-thread to do %d file read.\n", READS_PER_WRITE);
  printf(" >>> distributed mode: \n");
  printf(" >>> \t 1: Doing FS latency test distributedly, please enter the ip/port later.\n");
  printf(" >>> port: the port of server for distrituted test.\n");
  printf(" \n The Following config could be changed in FSLatency_distributed.h\n");
  printf(" >>> blocksize: Block Size per read/write\n\n");
  printf(" Example: FSServer /datapool  500 514 1 1 127.0.0.1 5678 \n");
}

void print_client_usage() {
  printf("Usage: FSLatency-c  path count filesize(KB) ip port(int)\n");
  printf(" >>> path: a avaible path that can be read/write\n");
  printf(" >>> count: the number of files that need to be read/written\n");
  printf(" >>> filesize: Each file size in KB that will be write\n");
  printf(" >>> MultiReader:\n");
  printf(" >>> \t 0: Use single thread to do %d file read.\n", READS_PER_WRITE);
  printf(" >>> \t 1: Use multi-thread to do %d file read.\n", READS_PER_WRITE);
  printf(" >>> distributed mode: \n");
  printf(" >>> \t 0: Doing File System latency test locally without network.\n");
  printf(" >>> \t 1: Doing FS latency test distributedly, please enter the ip/port later.\n\n");
  printf(" >>> ip: the IP address of server for distrituted test.\n");
  printf(" >>> port: the port of server for distrituted test.\n");
  printf(" \n The Following config could be changed in FSLatency_distributed.h\n");
  printf(" >>> blocksize: Block Size per read/write\n");
  printf(" Example: FSClient /datapool/ 500 514 1 1 127.0.0.1 5678 \n");
  printf(" Example: FSClient /datapool/ 500 514 0 0 \n");
}

void print_usage() {
  if(G_testRole == ROLE_SERVER) {
    print_server_usage();
  } else {
    print_client_usage();
  }
}

void print_client_start_information() {
  printf("Starting FSLatency distributed Server...\n");
  printf(">>> Listen port: %d.\n", G_port);
  printf(">>> Total read file count: %lld.\n", G_read_fileCount);
  printf(">>> Read from Path:%s.\n", G_path);
  printf(">>> Each read file size: %dKB.\n", G_kfileSize);
  printf(">>> Each read block size: %dB.\n", G_blockSize);
  if( G_is_multithread_read) {
    printf(">>> Use Multithread to read files.\n");
  } else {
    printf(">>> Use single thread to read files.\n");
  }
}

void print_client_start_information() {
  printf(">>> Total File to write: %lld\n", G_write_fileCount);
  printf(">>> File Write to: %s\n", G_path);
  printf(">>> Total File to read: %lld\n", G_read_fileCount);
  printf(">>> Each File Read/Write Size: %d KB\n", G_kfileSize);
  printf(">>> Block Size per write: %d B\n", G_blockSize);
  if( G_is_multithread_read) {
    printf(">>> Use Multithread to read files.\n");
  } else {
    printf(">>> Use single thread to read files.\n");
  }

  if( G_is_distributed) {
    printf(">>> Read File will be  done by sever: %s:%d\n", G_ipaddr, G_port);
  } else {
    printf(">>> Read File is done locally.\n");
  }
  printf("Start test......\n");
}

void print_start_information() {
  if(G_testRole == ROLE_SERVER) {
    print_server_start_information();
  } else {
    print_client_start_information();
  }
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
    printf("Long Latency 100~150ms: %lld us.\n", elapsed);
    G_100_150_ms++;
  } else if ( elapsed < 300000) {
    printf("Long Latency 150~300ms: %lld us.\n", elapsed);
    G_150_300_ms++;
  } else if ( elapsed > 300000) {
    printf("Long Latency >300ms: %lld us.\n", elapsed);
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

void single_file_read() {
  long long r_fileCount = G_read_fileCount;
  long long rand_i = rand() % (r_fileCount);
  int blockSize = G_blockSize;

  char buf[MAX_BLOCK_SIZE] = {0};
  char filename[MAX_FILENAME_LEN];
  snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, G_path, G_filename_r_prefix, rand_i);
  int read_fd = open(filename, O_RDONLY, 0);
  if( read_fd <= 0 ) {
    printf("Open file for read error: %s\n", filename);
  }
  read(read_fd, buf, blockSize);
  close(read_fd);
}

void single_file_read_thread(void* args) {
  int thread_idx = *(int*) args;
  printf(">>> Threads: start Thread %d: 0x%x and wait\n", thread_idx,
	 (unsigned int)pthread_self());

  while(1) {
    sem_wait(&sem_read_start[thread_idx]);
    single_file_read();
    sem_post(&sem_read_end[thread_idx]);
  }
}

void op_file_read_single_thread() {
  // Read READS_PER_WRITE-file
  int j=0;
  for(; j < READS_PER_WRITE; j++) {
    single_file_read();
  }
}

void op_file_read_multi_thread() {
  // Tell all the thread to start read
  int i=0;
  for(;  i < READS_PER_WRITE; i++) {
    sem_post(&sem_read_start[i]);
  }

  // Wait all thread to end
  i=0;
  for(;  i < READS_PER_WRITE; i++) {
    sem_wait(&sem_read_end[i]);
  }
}

void op_file_read() { 
  if(G_is_multithread_read) {
    op_file_read_multi_thread();
  } else {
    op_file_read_single_thread();
  }
}

// Create the testdir
void create_test_dir() {
  char mkdircmd[MAX_STR_LEN] = {0};
  sprintf( mkdircmd, "mkdir -p %s", G_path);
  system(mkdircmd);
}

void single_step_create( const long long index_start, const long long index_end) {

  long long i;
  char buf[MAX_BLOCK_SIZE] = {0};
  int blockSize=G_blockSize;

  for( i= index_start; i < index_end; i++) {
    char filename[MAX_FILENAME_LEN];
    snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, G_path, G_filename_r_prefix, i);
    FD cur_fd = ( FD )open(filename, (O_CREAT|O_APPEND|O_RDWR), 0666);
    if(cur_fd <= 0 ) {
      printf("Open file error: %s\n", filename);
    }
    int cur_size = 0;
    while(cur_size < G_fileSize ) {
      if( (G_fileSize - cur_size) > blockSize) {
	write(cur_fd, buf, blockSize);
	cur_size += blockSize;
      } else if( (G_fileSize - cur_size) > 0) {
	write(cur_fd, buf, (G_fileSize - cur_size ));
	cur_size = G_fileSize;
      }
    }
    close(cur_fd);    
  }
}


void op_file_create_write(const long long  fileName_idx) {

  // Write 1-file
  char filename[MAX_FILENAME_LEN];
  char buf[MAX_BLOCK_SIZE] = {0};


  snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, G_path,
	   G_filename_w_prefix, fileName_idx);
  int cur_fd;
  cur_fd = open(filename, ( O_CREAT|O_APPEND|O_RDWR ), 0666);
  if(cur_fd <= 0 ) {
    printf("Open file error: %s\n", filename);
  }
  write(cur_fd, buf, G_blockSize);
  close(cur_fd);
}


void do_test_local() {
  // Each file only read/write a single blocksize of data
  long long i=0;

  for(i=0; i < G_write_fileCount; i++) {
    if( i % 200 == 0 ) {
      printf("\rWrite Test process:%3.1f%%.", (float)i/(float)G_write_fileCount*100);
      fflush(0);
    }

    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);
    create_test_dir();
    op_file_create_write(i);
    op_file_read();

    gettimeofday(&tv_end, NULL);

    long long elapsed = ( ( tv_end.tv_sec * 1000000 + tv_end.tv_usec)
			  - (tv_begin.tv_sec * 1000000 + tv_begin.tv_usec));

    record_latency(elapsed);

  }
  printf("\rWrite Test process:%3.1f%%.\n\n\n", (float)100);
  
  Analysis_distribution();
  
}

// Write was done locally, while read was done remotely
void do_test_remote( ) {

  // Create a client socket
  struct sockaddr_in servaddr;
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(G_port);
  servaddr.sin_addr.s_addr = inet_addr(G_ipaddr);

  if(connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    printf("FSServer connect error. Please Check:\n\t1.Is FSServer started?\n");
    printf("\t2.Have it done the Read file preparation phase?\n");
    exit(0);
  }

  // Each file only read/write a single blocksize of data
  long long i=0;
  for(i=0; i < G_write_fileCount; i++) {
    if( i % 200 == 0 ) {
      printf("\rWrite Test process:%3.1f%%.", (float)i/(float)G_write_fileCount*100);
      fflush(0);
    }
    
    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);
    
    // 1. Local write
    op_file_create_write(i);


    // 2. Remote read
    char* message="read";
    send(sock, message, strlen(message), 0); 
    char done_msg[6]={0};
    recv(sock, done_msg, 6,0);
    if(done_msg[0] != 'D' ) {
      printf("Remote File Read error.\n");
      exit(-1);
    }
    gettimeofday(&tv_end, NULL);

    long long elapsed = ( ( tv_end.tv_sec * 1000000 + tv_end.tv_usec)
			  - (tv_begin.tv_sec * 1000000 + tv_begin.tv_usec));

    record_latency( elapsed);
  }
  printf("\rWrite Test process:%3.1f%%.\n\n", (float)100);

  Analysis_distribution();
}


void do_test() {
  if(G_is_distributed) {
    // Distributed mode
    // 1. Read file preparation will be done by server
    // Nothing need to do;

    // 2. Do write-remote-reads test
    do_test_remote();

  } else {
    // Local mode
    // 1. prepare files for read support
    prepare_env();
   
    // 2. Do reads-write test locally
    do_test_local();
  }
}

/**
 * Create read_fcnt files with the size: file_size to prepare for file read
 */
void prepare_env() {

  printf(">>> Prepare files for read, Total Count:%lld .\n", G_read_fileCount);
  create_test_dir();
  char buf[MAX_BLOCK_SIZE] = {0};  
  long long i = 0;
  for( ; i < G_read_fileCount; i++) {
    if( i % 300 == 0 ) {
      printf("\r>>>> Current process:%3.1f%%.", (float)i/(float)G_read_fileCount*100);
      fflush(0);
    }
    char filename[MAX_FILENAME_LEN];
    snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, G_path, G_filename_r_prefix, i);
    FD cur_fd = ( FD )open(filename, ( O_CREAT|O_APPEND|O_RDWR ), 0666);
    if(cur_fd <= 0 ) {
      printf("Open file error: %s\n", filename);
    }
    int cur_size = 0;
    while(cur_size < G_fileSize ) {
      if( (G_fileSize - cur_size) > G_blockSize) {
	write(cur_fd, buf, G_blockSize);
	cur_size += G_blockSize;
      } else if( (G_fileSize - cur_size) > 0) {
	write(cur_fd, buf, (G_fileSize - cur_size ));
	cur_size = G_fileSize;
      }
    }
    close(cur_fd);    
  }
  printf("\r>>>> Current process:%3.1f%%.\n", (float)100);
}



enum T_testRole {
  ROLE_SERVER,
  ROLE_CLIENT,
};

void parse_options() {

  if(!(argc == 7  || argc ==9)) {
    printf("Error: does not have correct number of parameter.\n");
    print_usage();
    exit(0);
  }

  if(argv[1][0] == 's' ) {
    G_testRole = ROLE_SERVER;
  } else if (argv[1][0] == 'c') {
    G_testRole = ROLE_CLIENT;
  } else {
    printf("Error: Not an available row, only Server(s)/Client(c) is acceptable.\n");
    print_usage();
    exit(0);
  }

  G_is_distributed = atoi(argv[6]);
  if(G_is_distributed && argc != 9 ) {
    printf("Error: Please check the parameter for Distributed mode.\n");
    print_usage();
    exit(0);
  } else if( !G_is_distributed && (argc != 7)) {
    printf("Error: Please check the parameter for local mode.\n");
    print_usage();
    exit(0);
  }


  strcpy(G_path, argv[2]);
  G_write_fileCount = atoi(argv[3]);
  G_kfileSize = atoi(argv[4]);
  G_fileSize = G_kfileSize*1024;
  G_is_multithread_read = atoi(argv[5]);
  if(G_is_distributed) {
    strcpy(G_ipaddr, argv[7]);
    G_port = atoi(argv[8]);
  }
  G_read_fileCount = READS_PER_WRITE * G_write_fileCount;
  if(G_read_fileCount > MAX_READ_FILE_CNT) {
    G_read_fileCount= MAX_READ_FILE_CNT;
  }
}


int main(int argc, char* argv[]) {
  parse_options();
  print_start_information();

  if (G_is_multithread_read) {
    printf(">>> Init %d threads for multiple thread read.\n", READS_PER_WRITE);
    int i=0;
    for(;  i < READS_PER_WRITE; i++) {
      sem_init(&sem_read_start[i],0,0);
      sem_init(&sem_read_end[i],0,0);
      G_thread_idx[i]=i;
      pthread_create(&G_t[i],NULL, (void*)single_file_read_thread, &G_thread_idx[i]);
    }
  }
  do_test();
  return 0;
}

int main(int argc, char **argv) {
  
  prepare_env();

  if(G_is_multithread_read) {
    printf(">>> Init %d threads for multiple thread read.\n", READS_PER_WRITE);
    int i=0;
    for(;  i < READS_PER_WRITE; i++) {
      sem_init(&sem_read_start[i],0,0);
      sem_init(&sem_read_end[i],0,0);
      G_thread_idx[i]=i;
      pthread_create(&G_t[i],NULL, (void*)single_file_read_thread, &G_thread_idx[i]);
    }
  }

  struct sockaddr_in servaddr;  
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(G_port);

  if(bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("Sockect Bind");
    exit(-1);
  }

  printf(">>> Network listen port binding done.\n");
  if (listen(sock, 5)<0) {
    /* if listen failed then display error and exit */
    perror("listen");
    exit(-1);
  }
  printf(">>> start listening done. \n");

  // Accept socket connection
  int clisock = accept(sock, (struct sockaddr *)NULL, NULL);
  printf(">>> Receive connection from client, under client's direction now.\n");
  if (clisock >= 0) {
    while(1) {
      int messageLength = 16;
      char message[messageLength+1];

      recv(clisock, message, messageLength,0);

      if(message[0]=='r'){
	op_file_read();
      } else {
	printf("Error Message. 'read' command is needed here.\n");
      }

      char* done_msg = "Done!";
      send(clisock, done_msg,  strlen(done_msg), 0);
    }
    close(clisock);
  }
  return 0;
}
