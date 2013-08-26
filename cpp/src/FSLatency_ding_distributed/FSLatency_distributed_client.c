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
 *      b. client tell the server to read READS_PER_WRITE files.
 *      c. server read READS_PER_WRITE files, and tell client that I have done
 *      d. client report the whole latency
 *
 *
 */
#include "FSLatency_distributed.h"

void print_usage() {
  printf("Usage: test path testcount filesize(Byte) blocksize(Byte) close(1|0) distribued(0|1)\n");
  printf(" >>> path: a avaible path that can be read/write\n");
  printf(" >>> count: the number of files that need to be read/written\n");
  printf(" >>> filesize: Each file size in Byte that will be write\n");
  printf(" >>> blocksize: Block Size per read/write\n");
  printf(" >>> close: \n");
  printf(" >>> \t 0: Use open-write/read-close steps for each block.\n");
  printf(" >>> \t 1: use  write-sync per block for each block.\n\n");
  printf(" >>> distributed mode: \n");
  printf(" >>> \t 0: Doing File System latency test locally without network.\n");
  printf(" >>> \t 1: Doing FS latency test distributedly, please enter the ip/port later.\n\n");
  printf(" >>> port: the port of server for distrituted test.\n");
  printf(" Example: test  data/ 500 524288 524288 1 0 5678 \n");
}


void print_usage_new() {
  printf("Usage: test distribued(0|1) port(int)\n");
  printf(" >>> distributed mode: \n");
  printf(" >>> \t 0: Doing File System latency test locally without network.\n");
  printf(" >>> \t 1: Doing FS latency test distributedly, please enter the ip/port later.\n\n");
  printf(" >>> port: the port of server for distrituted test.\n");
  printf(" \n The Following config could be changed in FSLatency_distributed.h\n");
  printf(" >>> path: a avaible path that can be read/write\n");
  printf(" >>> count: the number of files that need to be read/written\n");
  printf(" >>> filesize: Each file size in Byte that will be write\n");
  printf(" >>> blocksize: Block Size per read/write\n");
  printf(" >>> close: \n");
  printf(" >>> \t 0: Use open-write/read-close steps for each block.\n");
  printf(" >>> \t 1: use  write-sync per block for each block.\n\n");
  printf(" Example: test 1 5678 \n");
  printf(" Example: test 0 \n");
}


void print_start_information( const int fileCount, const int fileSize, 
			      const int blockSize, const bool needclose,
			      const enum test_mode mode) {
  printf("Start test......\n");
  printf(">>> Total File to write: %d\n", fileCount);
  printf(">>> Total File to read: %dx%d=%d\n", fileCount, READS_PER_WRITE, READS_PER_WRITE * fileCount);
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

  case MODE_HW:
  case MODE_AR:
  case MODE_HR:
    printf("Mode not implemented yet.\n");
    exit(-1);

  default:
    printf("Unsupported Mode. \n");
    exit(-1);

  }
  printf(">>> Test Mode is: %s.\n", mode_str);
  printf("\n\n");
}

void init_fds( FD* fd, int totalfiles, char* filename_prefix, int mode ) {
  int i = 0;
  for( ; i < totalfiles; i++) {
    char filename[MAX_FILENAME_LEN];
    snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, 
	     G_path, filename_prefix, i);
    if( ( fd[i] = open(filename, mode, 0666) ) <= 0 )
      printf("Open file error: %s\n", filename);
  }
}


void close_fds( FD* fd, int totalfiles) {
  int i = 0;
  for( ; i < totalfiles; i++) {
    close(fd[i]);
  }
}


void op_file_create_write(FD* fd, const int fileCount, const int blockSize, 
			  const bool needclose, const int fileName_idx) {

  // Write 1-file
  char filename[MAX_FILENAME_LEN];
  char buf[MAX_BLOCK_SIZE] = {0};

  snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, G_path,
	   G_filename_w_prefix, fileName_idx);
  int cur_fd;
  if(needclose) {
    cur_fd = open(filename, AW_FILE_MODE, 0666);
    if(cur_fd <= 0 ) {
      printf("Open file error: %s\n", filename);
    }
    write(cur_fd, buf, blockSize);
    close(cur_fd);
  } else {
    cur_fd = fd[fileName_idx];
    write(cur_fd, buf, blockSize);
    fsync(cur_fd);
  }
}


void do_append_write_test_local(FD* fd, const int fileCount, const int fileSize,
				const int blockSize, const bool needclose) {
  if(! needclose) {
    // Do not need close:
    // Open file before the test start, mode O_CREAT|O_APPEND|O_SYNC
    init_fds(fd, fileCount, G_filename_w_prefix, AW_FILE_MODE);
  }
  create_test_dir();

  int i=0;

  // Each file only read/write a single blocksize of data
  for(i=0; i < fileCount; i++) {

    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);

    op_file_create_write(fd, fileCount, blockSize, needclose, i);
    op_file_read(READS_PER_WRITE * fileCount, blockSize);

    gettimeofday(&tv_end, NULL);

    long long elapsed = ( ( tv_end.tv_sec * 1000000 + tv_end.tv_usec)
			  - (tv_begin.tv_sec * 1000000 + tv_begin.tv_usec));
    if(needclose) {
      printf("Write with Close: ");
    } else {
      printf("Write without Close: ");
    }
    printf("%lld us\n", elapsed);
  }

  if(!needclose) {
    close_fds(fd, fileCount);
  }
}

// Write was done locally, while read was done remotely
void do_append_write_test_remote( FD* fd, const int fileCount, const int fileSize,
				  const int blockSize, const bool needclose) {
  if(! needclose) {
    // Do not need close:
    // Open file before the test start, mode O_CREAT|O_APPEND|O_SYNC
    init_fds(fd, fileCount, G_filename_w_prefix, AW_FILE_MODE);
  }
  create_test_dir();

  // Create a client socket
  struct sockaddr_in servaddr;
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(G_port);
  servaddr.sin_addr.s_addr = inet_addr(G_ipaddr);

  connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));

  // Each file only read/write a single blocksize of data
  int i=0;
  for(i=0; i < fileCount; i++) {


    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);
    
    // 1. Local write
    op_file_create_write(fd, fileCount, blockSize, needclose, i);


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
    if(needclose) {
      printf("Write with Close: ");
    } else {
      printf("Write without Close: ");
    }
    printf("%lld us\n", elapsed);
  }

  if(!needclose) {
    close_fds(fd, fileCount);
  }
}


void do_append_write_test(FD* fd, const int fileCount, const int fileSize, 
			  const int blockSize, const bool needclose, const bool distributed_mode) {

  if(distributed_mode) {
    // Distributed mode
    // 1. Read file preparation will be done by server

    // 2. Do write-remote-reads test
    do_append_write_test_remote(fd, fileCount, fileSize, blockSize, needclose);
  } else {
    // Local mode

    // 1. prepare files for read support
    int read_fileCount = READS_PER_WRITE * fileCount;
    prepare_env(read_fileCount, fileSize);
    
    // 2. Do reads-write test locally
    do_append_write_test_local(fd, fileCount, fileSize, blockSize, needclose);
  }
}



int main(int argc, char* argv[]) {

  FD fd[MAX_COUNT] = {0};

  // variables for the test
  //  if(argc != 8 ) {

  if(argc == 1 || argc > 3) {
    print_usage_new();
    exit(0);
  }

  bool is_distributed = atoi(argv[1]);
  if(is_distributed && (argc != 3) ) {
    print_usage_new();
    exit(0);
  } else if( !is_distributed && (argc !=2)) {
    print_usage_new();
    exit(0);
  }

  // init path
  //strcpy(G_path, argv[1]);
  //int fileCount = 
  // init total file count
  /* int fileCount = atoi(argv[2]); */
  /* int fileSize = atoi(argv[3]); */
  /* int blockSize = atoi(argv[4]); */
  /* bool needclose = atoi(argv[5]); */
  /* bool is_distributed = atoi(argv[6]); */
  /* G_port = atoi(argv[7]); */

  if(is_distributed) {
    G_port = atoi(argv[2]);
  }

  int fileCount = G_fileCount;
  int fileSize = G_fileSize;
  int blockSize = G_blockSize;
  int needclose = G_needclose;

  enum test_mode mode;

  mode = MODE_AW;
  print_start_information(fileCount, fileSize, blockSize, needclose, mode);

  switch(mode) {
  case MODE_AW:
    do_append_write_test(fd, fileCount, fileSize, blockSize, needclose, is_distributed);
    break;

  case MODE_HW:
  case MODE_AR:
  case MODE_HR:
    printf("Mode not implemented yet.\n");
    exit(-1);

  default:
    printf("Unsupported Mode. \n");
    exit(-1);

  }

  return 0;
}