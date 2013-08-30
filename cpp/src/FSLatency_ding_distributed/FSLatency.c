/*
 * FS Latency Test Tool
 *
 * Author: Ling Kun  <lkun.erlv@gmail.com>
 * 
 * 2013-8-28
 *
 */


// TODO: Add asyncio support

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


typedef int FD;
typedef int bool;

enum T_testRole {
  ROLE_SERVER,
  ROLE_CLIENT,
  ROLE_LOCAL,
};

enum T_readMode {
  MODE_SINGLETHREAD,
  MODE_MULTITHREAD,
  MODE_ASYNCIO,
};

char G_path[MAX_PATH_LEN] = {"data"};
char* G_filename_w_prefix = "1MB_W_";
char* G_filename_r_prefix = "1MB_R_";
long long G_write_fileCount=0;
long long G_read_fileCount=0;
char G_ipaddr[MAX_IP_LEN]={"127.0.0.1"};
int G_port = 5678;
int G_fileSize=514*1024;
int G_kfileSize=514;
int G_blockSize=514*1024;
enum T_readMode G_ReadMode=MODE_SINGLETHREAD;
long long G_0_20_ms=0;
long long G_20_40_ms=0;
long long G_40_60_ms=0;
long long G_60_80_ms=0;
long long G_80_100_ms=0;
long long G_100_150_ms=0;
long long G_150_300_ms=0;
long long G_300_ms=0;
long long G_total=0;
enum T_testRole G_testRole=ROLE_LOCAL;

pthread_t G_t[READS_PER_WRITE]={0};
int G_thread_idx[READS_PER_WRITE]={0};
sem_t sem_read_start[READS_PER_WRITE];
sem_t sem_read_end[READS_PER_WRITE];

ssize_t read(int fd, void *buf, size_t count);
int close(int fd);
ssize_t write(int fd, const void *buf, size_t count);
int fsync(int fd);
in_addr_t inet_addr(const char *cp);

void print_whole_usage() {
  printf("Usage: FSLatency l|s|c \n");
  printf(">>> Do FileSystem latency test in local or distributed mode, using single-thread,multi-thread, async-io read \n");
  printf(" FSLatency l : Do latency test in local mode, all read/write is done locally.\n");
  printf(" FSLatency s : Do latency test in distributed mode-server  read side.\n");
  printf(" FSLatency c : Do latency test in distributed mode-client  write side.\n");
  printf(">>> Please run the above command for more detailed usage information.\n");
}

void print_server_usage() {
  printf("Usage: FSLatency s path count filesize(KB) ReaderMode(0|1|2) port(int)\n");
  printf(" >>> path: a avaible path where read perform\n");
  printf(" >>> count: the number of tests\n");
  printf(" >>> filesize: Each file size in KB that will be generated to read\n");
  printf(" >>> MultiReader:\n");
  printf(" >>> \t 0: Use single thread to do %d file read.\n", READS_PER_WRITE);
  printf(" >>> \t 1: Use multi-thread to do %d file read.\n", READS_PER_WRITE);
  printf(" >>> \t 2: Use Async-io interface to do %d file read.\n", READS_PER_WRITE);
  printf(" >>> port: the port of server for distrituted test.\n");
  printf(" \n The Following config could be changed in FSLatency_distributed.h\n");
  printf(" >>> blocksize: Block Size per read/write\n\n");
  printf("Example: FSLatency s /datapool  500 514 1 5678 \n");
}

void print_client_usage() {
  printf("Usage: FSLatency c  path count filesize(KB) ip port(int)\n");
  printf(" >>> path: a avaible path that can be write\n");
  printf(" >>> count: the number of test that need to be done\n");
  printf(" >>> filesize: Each file size in KB that will be write\n");
  printf(" >>> ip: the IP address of server for distrituted test.\n");
  printf(" >>> port: the port of server for distrituted test.\n");
  printf(" \n The Following config could be changed in FSLatency_distributed.h\n");
  printf(" >>> blocksize: Block Size per read/write\n");
  printf("\nExample: FSLatency c /datapool/ 500 514 127.0.0.1 5678 \n");
}

void print_local_usage() {
  printf("Usage: FSLatency l  path count filesize(KB) ReaderMode(0|1|2)\n");
  printf(" >>> path: a avaible path that can be read/write\n");
  printf(" >>> count: the number of test that need to be done\n");
  printf(" >>> filesize: Each file size in KB that will be write\n");
  printf(" >>> MultiReader:\n");
  printf(" >>> \t 0: Use single thread to do %d file read.\n", READS_PER_WRITE);
  printf(" >>> \t 1: Use multi-thread to do %d file read.\n", READS_PER_WRITE);
  printf(" >>> \t 2: Use Async-io interface to do %d file read.\n", READS_PER_WRITE);
  printf(" \n The Following config could be changed in FSLatency_distributed.h\n");
  printf(" >>> blocksize: Block Size per read/write\n\n");
  printf("Example: FSLatency l /datapool/ 500 514 1 \n");
}


void print_usage() {
  if(G_testRole == ROLE_SERVER) {
    print_server_usage();
  } else if(G_testRole == ROLE_LOCAL) {
    print_local_usage();
  } else if(G_testRole == ROLE_CLIENT) {
    print_client_usage();
  } else {
    print_whole_usage();
  }
}

void print_server_start_information() {
  printf("Starting FSLatency distributed test: Server...\n");
  printf(">>> Listen port: %d.\n", G_port);
  printf(">>> Total read file count: %lld.\n", G_read_fileCount);
  printf(">>> Read from Path:%s.\n", G_path);
  printf(">>> Each read file size: %d KB.\n", G_kfileSize);
  printf(">>> Each read block size: %d B.\n", G_blockSize);
  if( G_ReadMode == MODE_MULTITHREAD) {
    printf(">>> Use Multithread to read files.\n");
  } else if(G_ReadMode == MODE_SINGLETHREAD) {
    printf(">>> Use single thread to read files.\n");
  } else if( G_ReadMode == MODE_ASYNCIO) {
    printf(">>> Use single thread Async io to read files.\n");
  }
}

void print_client_start_information() {
  printf("Starting FSLatency distributed test: Client...\n");
  printf(">>> Total File to write: %lld\n", G_write_fileCount);
  printf(">>> File Write to: %s\n", G_path);
  printf(">>> Each Writen-file Size: %d KB\n", G_kfileSize);
  printf(">>> Block Size per write: %d B\n", G_blockSize);
  printf(">>> FSLatency test server: %s:%d\n", G_ipaddr, G_port);
}

void print_local_start_information() {
  printf("Starting FSLatency test: Local mode...\n");
  printf(">>> File read/write Path: %s.\n", G_path);
  printf(">>> Total test or file-write count: %lld\n", G_write_fileCount);
  printf(">>> Total file-read count: %lld\n", G_read_fileCount);
  printf(">>> Each File Read/Write Size: %d KB\n", G_kfileSize);
  printf(">>> Block Size per write: %d B\n", G_blockSize);
  if( G_ReadMode == MODE_MULTITHREAD) {
    printf(">>> Use Multithread to read files.\n");
  } else if(G_ReadMode == MODE_SINGLETHREAD) {
    printf(">>> Use single thread to read files.\n");
  } else if( G_ReadMode == MODE_ASYNCIO) {
    printf(">>> Use single thread Async io to read files.\n");
  }

}

void print_start_information() {
  printf("------ Running Parameter ------\n");

  // Output the start time
  time_t now = time(0);
  char buff[100];
  strftime(buff, 100, "%Y-%m-%d %H:%M:%S.000\n", localtime(&now));
  printf(">>> Start Time: %s\n", buff);
 
  if(G_testRole == ROLE_SERVER) {
    print_server_start_information();
  } else if( G_testRole == ROLE_CLIENT) {
    print_client_start_information();
  } else if( G_testRole == ROLE_LOCAL) {
    print_local_start_information();
  }
  printf("-------------------------------\n\n");

  printf("\nStart test......\n");
}

/**
 * Record all the latency into several gloal var 
 *for distribution calculation
 */
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

/**
 * Analyse the recorded latency of the whole test
 */
void Analysis_distribution() {
  float f_0_20_ms = (float)G_0_20_ms/(float)G_total*100;
  float f_20_40_ms = (float)G_20_40_ms/(float)G_total*100;
  float f_40_60_ms = (float)G_40_60_ms/(float)G_total*100;
  float f_60_80_ms = (float)G_60_80_ms/(float)G_total*100;
  float f_80_100_ms = (float)G_80_100_ms/(float)G_total*100;
  float f_100_150_ms = (float)G_100_150_ms/(float)G_total*100;
  float f_150_300_ms = (float)G_150_300_ms/(float)G_total*100;
  float f_300_ms = (float)G_300_ms/(float)G_total*100;
  printf("Latency Distribution for %lld test:\n", G_total);
  printf("Latency(ms)\t0~20\t20~40\t40~60\t60~80\t80~100\t100~150\t150~300\t>300\n");
  printf("Percentage\t%5.2f%%\t%5.2f%%\t%5.2f%%\t%5.2f%%\t%5.2f%%\t%5.2f%%\t%5.2f%%\t%5.2f%%\n",
	 f_0_20_ms, f_20_40_ms, f_40_60_ms,f_60_80_ms,
	 f_80_100_ms, f_100_150_ms,f_150_300_ms, f_300_ms);
}

/**
 * Perform the single file read operation
 * The filename is generated randomly
 */
void single_file_read(long long file_id,  char* filename, 
		      char* buf) {
  int blockSize = G_blockSize;
  snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, G_path, 
	   G_filename_r_prefix, file_id);
  int read_fd = open(filename, O_RDONLY, 0);
  if( read_fd <= 0 ) {
    printf("Open file for read error: %s\n", filename);
  }
  read(read_fd, buf, blockSize);
  close(read_fd);
}

/**
 * An thread which will perform one single file read operation 
 * under the main thread's control using sem_read_start[thread_idx] and 
 * sem_read_end[thread_idx]
 */
void single_file_read_thread(void* args) {
  int thread_idx = *(int*) args;
  long long rand_step = 
    (G_read_fileCount/READS_PER_WRITE)*thread_idx;
  printf(">>> Threads: start Thread %d: 0x%x and wait\n", 
	 thread_idx,
	 (unsigned int)pthread_self());
  char buf[MAX_BLOCK_SIZE] = {0};
  char filename[MAX_FILENAME_LEN]={0};

  while(1) {
    // Wait for main thread to post sem_read_start[thread_idx]

    // Sometimes sem_wait will return -1 left semaphore 
    // unchanged on error, We need to take care of this
    int ret_val;
    while( ret_val = sem_wait(&sem_read_start[thread_idx])) {
      if(ret_val == -1 )
	continue;
    }

    // Add rand_step to rand() return value, so that each thread
    // could use a more random file id.
    long long rand_i = (rand()+ rand_step) % (G_read_fileCount);

    single_file_read(rand_i, buf, filename);

    // Tell main thread that I have done file read!
    sem_post(&sem_read_end[thread_idx]);
  }
}

/**
 * Read multiple files in async io mode using a single thread 
 */
void op_file_read_async_io() {
  // TODO: Not implement yet.
  int efd, fd, epfd;
  io_context_t ctx;
  struct timespec tms;
  struct io_event events[NUM_EVENTS];
  struct custom_iocb iocbs[NUM_EVENTS];
  struct iocb *iocbps[NUM_EVENTS];
  struct custom_iocb* iocbp;
  struct epoll_event epevent;
  char* buf;
  if(posix_memalign(&buf, ALIGN_SIZE, MAX_BLOCKSIZE)) {
    perror("posix_memalign");
    exit(0);
  }

  char filename[MAX_FILENAME_LEN] = {0};

  efd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC );
  if( efd == -1) {
    perror("eventfd");
    exit(0);
  }
  if(io_setup(8192, &ctx)) {
    perror("io_setup");
    exit(0);
  }

  long long rand_i = (rand()) % (G_read_fileCount);
  snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, G_path, 
	   G_filename_r_prefix, rand_i);
  int read_fd = open(filename, O_RDONLY, 0);
  if( read_fd == -1) {
    perror("open");
    exit(0);
  }

}


/**
 * Read multiple  files in a single thread.
 *
 */
void op_file_read_single_thread() {
  char buf[MAX_BLOCK_SIZE] = {0};
  char filename[MAX_FILENAME_LEN]={0};

  int j=0;
  for(; j < READS_PER_WRITE; j++) {
    long long rand_i = rand() % (G_read_fileCount);
    single_file_read(rand_i, buf, filename);
  }
}

/**
 * Read Multiple files in multithread.
 * Each thread will read only one file.
 */
void op_file_read_multi_thread() {
  // Tell all the thread to start read
  int i=0;
  for(;  i < READS_PER_WRITE; i++) {
    sem_post(&sem_read_start[i]);
  }

  // Wait all thread to end
  i=0;
  for(;  i < READS_PER_WRITE; i++) {
    // Sometimes sem_wait will return -1 left semaphore unchanged on error,
    // We need to take care of this
    int ret_val;
    while( ret_val = sem_wait(&sem_read_end[i])) {
      if( ret_val == -1)
	continue;
    }
  }
}

/**
 * Perform the file read operation
 */
void op_file_read() { 
  if(G_ReadMode == MODE_MULTITHREAD) {
    op_file_read_multi_thread();
  } else if(G_ReadMode == MODE_SINGLETHREAD) {
    op_file_read_single_thread();
  } else if (G_ReadMode == MODE_ASYNCIO) {
    op_file_read_async_io();
  }
}

/**
 * Perpare for the test dir
 * This function is called before any file read/write start
 */
void create_test_dir() {
  char mkdircmd[MAX_STR_LEN] = {0};
  sprintf( mkdircmd, "mkdir -p %s", G_path);
  system(mkdircmd);
}

/**
 * Perform the file create and write operation
 */
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


/**
 * Do the FS Latency in local mode
 */
void do_test_local() {
  // Each file only read/write a single blocksize of data
  long long i=0;

  for(i=0; i < G_write_fileCount; i++) {
    if( i % 200 == 0 ) {
      printf("\rWrite Test process:%3.1f%%.", (float)i/(float)G_write_fileCount*100);
      fflush(0);
    }

    create_test_dir();

    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);
    op_file_create_write(i);
    op_file_read();
    gettimeofday(&tv_end, NULL);


    long long elapsed = ( ( tv_end.tv_sec * 1000000 + tv_end.tv_usec)
			  - (tv_begin.tv_sec * 1000000 + tv_begin.tv_usec));
    record_latency(elapsed);
    if( i%5000 == 0 ) {
      Analysis_distribution();
    }
  }
  printf("\rWrite Test process:%3.1f%%.\n\n\n", (float)100);
  Analysis_distribution();
}

/**
 * Do the FS Latency in distributed mode--the client side
 *
 * The Write of client side is done locally, and
 * it will ask the server to read, and wait untile the read is done.
 */
void do_test_client( ) {

  // STEP1: Create a client socket
  struct sockaddr_in servaddr;
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(G_port);
  servaddr.sin_addr.s_addr = inet_addr(G_ipaddr);

  // STEP2: Connect to the Server side
  if(connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    printf("FSServer connect error. Please Check:\n\t1.Is FSServer started?\n");
    printf("\t2.Have it done the Read file preparation phase?\n");
    exit(0);
  }

  create_test_dir();
  // STEP3: Start the stest 
  long long i=0;
  for(i=0; i < G_write_fileCount; i++) {
    // STEP3.1: report the whole progress of the test.
    if( i % 200 == 0 ) {
      printf("\rWrite Test process:%3.1f%%.", (float)i/(float)G_write_fileCount*100);
      fflush(0);
    }
    
    // STEP3.2: Start timing
    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);
    
    // STEP3.3: Perform local write operation
    op_file_create_write(i);


    // STEP3.4: Ask remote server to perform read operation
    char* message="read";
    send(sock, message, strlen(message), 0); 

    // STEP3.5: Waiting for the remote server operation done message
    char done_msg[6]={0};
    recv(sock, done_msg, 6, 0);
    
    if(done_msg[0] != 'D' ) {
      printf("Remote File Read error.\n");
      exit(-1);
    }

    // STEP3.6: Stop timing
    gettimeofday(&tv_end, NULL);

    long long elapsed = ( ( tv_end.tv_sec * 1000000 + tv_end.tv_usec)
			  - (tv_begin.tv_sec * 1000000 + tv_begin.tv_usec));

    // STEP 3.7: Record the latecny
    record_latency( elapsed);

    if( i%5000 == 0 ) {
      Analysis_distribution();
    }
  }
  // Just used to make the output works smothly
  printf("\rWrite Test process:%3.1f%%.\n\n", (float)100);

  // STEP 4: Analyse all the recorded latency
  Analysis_distribution();
}

/**
 * Do the FS Latency in distributed mode--the server side
 *
 * It will:
 *  1. wait for the client to connect
 *  2. start single/multiple thread file reading when the client tell it to do.
 *  3. Tell the client that read have done
 *  4. wait for the client's next command
 */
void do_test_server() {

  // STEP1: Create and init the socket, and listen to the port specified by user
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

  // STEP2: wait for the socket connection from client
  int clisock = accept(sock, (struct sockaddr *)NULL, NULL);
  printf(">>> Receive connection from client, under client's direction now.\n");


  // STEP3: Wait for client command, and do the read files operation
  if (clisock >= 0) {
    while(1) {
      int messageLength = 16;
      char message[messageLength+1];

      // STEP3.1 wait for client command
      recv(clisock, message, messageLength,0);

      // STEP3.2 Start files read operation
      if(message[0]=='r'){
	op_file_read();
      } else {
	printf("Error Message. 'read' command is needed here.\n");
      }
      
      // STEP3.3 Tell client that read operation is done.
      char* done_msg = "Done!";
      send(clisock, done_msg,  strlen(done_msg), 0);
    }
    close(clisock);
  }
}

/**
 * Create G_read_fileCount files with the size: file_size to prepare for file read
 */
void prepare_env() {

  printf(">>> Prepare %lld files for read....\n", G_read_fileCount);
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
    write(cur_fd, buf, G_blockSize);
    close(cur_fd);    
  }
  printf("\r>>>> Current process:%3.1f%%.\n", (float)100);
}


/**
 * Parse all the user specified options 
 */
void parse_options(int argc, char** argv) {
  
  if(argc < 2 ) {
    print_whole_usage();
    exit(0);
  }

  if( strlen(argv[1]) == 1) {
    if(argv[1][0] == 's') {
      G_testRole = ROLE_SERVER;
    } else if (argv[1][0] == 'c') {
      G_testRole = ROLE_CLIENT;
    } else if (argv[1][0] == 'l') {
      G_testRole = ROLE_LOCAL;
    } else {
      printf("Error: Not an available row, only Server(s)/Client(c) is acceptable.\n");
      print_whole_usage();
      exit(0);
    }
  } else {
    printf("Error: please pick up a role first for the test");
    print_whole_usage();
    exit(0);
  }

  if(G_testRole == ROLE_CLIENT && argc != 7 ) {
    printf("Error: Please check the parameter for Distributed mode client side.\n");
    print_usage();
    exit(0);
  } else if(G_testRole == ROLE_SERVER && argc != 7 ) {
    printf("Error: Please check the parameter for Distributed mode Server side.\n");
    print_usage();
    exit(0);
  } else if( G_testRole== ROLE_LOCAL && (argc != 6)) {
    printf("Error: Please check the parameter for local mode.\n");
    print_usage();
    exit(0);
  }

  if(G_testRole == ROLE_SERVER || G_testRole == ROLE_LOCAL) {
    switch(atoi(argv[5])) {
    case 0:
      G_ReadMode = MODE_SINGLETHREAD;
      break;
    case 1:
      G_ReadMode = MODE_MULTITHREAD;
      break;
    case 2:
      G_ReadMode = MODE_ASYNCIO;
      break;
    default:
      printf("Error: Please check ReaderMode parameter.\n");
      print_usage();
    }
  }

  strcpy(G_path, argv[2]);
  G_write_fileCount = atoi(argv[3]);
  G_kfileSize = atoi(argv[4]);
  G_fileSize = G_kfileSize*1024;

  switch(G_testRole) {
    
  case ROLE_SERVER:
    G_port = atoi(argv[6]);
    break;
  case ROLE_LOCAL:
    break;  
  case ROLE_CLIENT:
    strcpy(G_ipaddr, argv[5]);
    G_port = atoi(argv[6]);
    break;
  }
  
  G_read_fileCount = READS_PER_WRITE * G_write_fileCount;
  if(G_read_fileCount > MAX_READ_FILE_CNT) {
    G_read_fileCount= MAX_READ_FILE_CNT;
  }
  G_blockSize = G_fileSize;
}

/**
 * The main function of the program
 */
int main(int argc, char **argv) {

  // STEP1: Parse all options
  parse_options(argc, argv);


  // STEP2: if using multithread read, create semaphores used for 
  //        file read control.
  if(G_ReadMode == MODE_MULTITHREAD) {
    printf(">>> Init %d threads for multiple thread read.\n", READS_PER_WRITE);
    int i=0;
    for(;  i < READS_PER_WRITE; i++) {
      sem_init(&sem_read_start[i],0,0);
      sem_init(&sem_read_end[i],0,0);
      G_thread_idx[i]=i;
      pthread_create(&G_t[i],NULL, (void*)single_file_read_thread, &G_thread_idx[i]);
    }
  }

  print_start_information();

  // STEP3: Perform the test according to its role
  if(G_testRole == ROLE_LOCAL) {
    prepare_env();
    do_test_local();
  } else if (G_testRole == ROLE_SERVER ) {
    prepare_env();
    do_test_server();
  } else if ( G_testRole == ROLE_CLIENT ) {
    do_test_client();
  }
  return 0;
}
  
