/*
 * FS Latency Distributed Test Tool
 *
 * Author: Ling Kun  <lingkun@loongstore.com.cn>
 * Loongstore Inc.
 *
 */

#include "FSLatency_distributed.h"

void print_usage() {
  printf("Usage: FSClient path count filesize(KB) distribued(0|1) ip port(int)\n");
  printf(" >>> path: a avaible path that can be read/write\n");
  printf(" >>> count: the number of files that need to be read/written\n");
  printf(" >>> filesize: Each file size in KB that will be write\n");
  printf(" >>> distributed mode: \n");
  printf(" >>> \t 0: Doing File System latency test locally without network.\n");
  printf(" >>> \t 1: Doing FS latency test distributedly, please enter the ip/port later.\n\n");
  printf(" >>> ip: the IP address of server for distrituted test.\n");
  printf(" >>> port: the port of server for distrituted test.\n");
  printf(" \n The Following config could be changed in FSLatency_distributed.h\n");
  printf(" >>> blocksize: Block Size per read/write\n");
  printf(" Example: FSClient /datapool/ 500 514 1 127.0.0.1 5678 \n");
  printf(" Example: FSClient /datapool/ 500 514 0 \n");
}


void print_start_information( const int fileCount, const int fileSize, 
			      const int blockSize, const bool needclose,
			      const enum test_mode mode, const bool is_distributed) {
  printf(">>> Total File to write: %d\n", fileCount);
  printf(">>> File Write to: %s\n", G_path);
  printf(">>> Total File to read: %dx%d=%d\n", fileCount, READS_PER_WRITE, READS_PER_WRITE * fileCount);
  printf(">>> Each File Read/Write Size: %d KB\n", fileSize);
  printf(">>> Block Size per write: %d B\n", blockSize);

  if( is_distributed) {
    printf(">>> Read File will be  done by sever: %s:%d\n", G_ipaddr, G_port);
  } else {
    printf(">>> Read File is done locally.\n");
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
  printf(">>> Test Mode is: %s.\n\n", mode_str);
  printf("Start test......\n");
}

void op_file_create_write(FD* fd, const long long fileCount, const int blockSize, 
			  const bool needclose, const long long  fileName_idx) {

  // Write 1-file
  char filename[MAX_FILENAME_LEN];
  char buf[MAX_BLOCK_SIZE] = {0};
  create_test_dir();

  snprintf(filename, MAX_FILENAME_LEN, FILENAME_FORMAT, G_path,
	   G_filename_w_prefix, fileName_idx);
  int cur_fd;
  cur_fd = open(filename, AW_FILE_MODE, 0666);
  if(cur_fd <= 0 ) {
    printf("Open file error: %s\n", filename);
  }
  write(cur_fd, buf, blockSize);
  close(cur_fd);
}


void do_append_write_test_local(FD* fd, const long long  write_fileCount, 
				const long long read_fileCount, const int fileSize,
				const int blockSize, const bool needclose) {
  // Each file only read/write a single blocksize of data
  long long i=0;

  for(i=0; i < write_fileCount; i++) {
    printf("\rAppend Write Test process:%3.1f%%.", (float)i/(float)write_fileCount*100);
    fflush(0);

    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);

    op_file_create_write(fd, write_fileCount, blockSize, needclose, i);
    op_file_read(read_fileCount, blockSize);

    gettimeofday(&tv_end, NULL);

    long long elapsed = ( ( tv_end.tv_sec * 1000000 + tv_end.tv_usec)
			  - (tv_begin.tv_sec * 1000000 + tv_begin.tv_usec));

    record_latency(elapsed);

    /* if(needclose) { */
    /*   printf("Write with Close: "); */
    /* } else { */
    /*   printf("Write without Close: "); */
    /* } */
    // printf("Latency:%lld us\n", elapsed);
  }
  printf("\rAppend Write Test process:%3.1f%%.\n\n\n", (float)100);
  
  Analysis_distribution();
  
}

// Write was done locally, while read was done remotely
void do_append_write_test_remote( FD* fd, const long long  write_fileCount, const int fileSize,
				  const int blockSize, const bool needclose) {

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
  for(i=0; i < write_fileCount; i++) {
    printf("\rAppend Write Test process:%3.1f%%.", (float)i/(float)write_fileCount*100);
    fflush(0);
    
    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);
    
    // 1. Local write
    op_file_create_write(fd, write_fileCount, blockSize, needclose, i);


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
    //printf("Latency:%lld us\n", elapsed);
  }
  printf("\rAppend Write Test process:%3.1f%%.\n\n", (float)100);

  Analysis_distribution();
}


void do_append_write_test(FD* fd, const long long write_fileCount, const int fileSize, 
			  const int blockSize, const bool needclose, const bool distributed_mode) {
  if(distributed_mode) {
    // Distributed mode
    // 1. Read file preparation will be done by server
    // Nothing need to do;

    // 2. Do write-remote-reads test
    do_append_write_test_remote(fd, write_fileCount, fileSize, blockSize, needclose);

  } else {
    // Local mode
    // 1. prepare files for read support
    long long  read_fileCount = READS_PER_WRITE * write_fileCount;
    if(read_fileCount > 100000) {
      read_fileCount=100000;
    }
    prepare_env(read_fileCount, fileSize);
   
    // 2. Do reads-write test locally
    do_append_write_test_local(fd, write_fileCount, read_fileCount, fileSize, blockSize, needclose);
  }
}


int main(int argc, char* argv[]) {

  FD fd[MAX_COUNT] = {0};
  long long  fileCount;
  int fileSize, blockSize;
  bool needclose, is_distributed;

  if(!(argc ==5  || argc ==7)) {
    printf("Error: does not have correct number of parameter.\n");
    print_usage();
    exit(0);
  }

  is_distributed = atoi(argv[4]);
  if(is_distributed && argc != 7 ) {
    printf("Error: Please check the parameter for Distributed mode.\n");
    print_usage();
    exit(0);
  } else if( !is_distributed && (argc !=5)) {
    printf("Error: Please check the parameter for local mode.\n");
    print_usage();
    exit(0);
  }

  strcpy(G_path, argv[1]);
  fileCount = atoi(argv[2]);
  int KfileSize = atoi(argv[3]);
  fileSize = KfileSize*1024;
  if(is_distributed) {
    strcpy(G_ipaddr, argv[5]);
    G_port = atoi(argv[6]);
  }
  blockSize = G_blockSize;
  needclose = G_needclose;

  enum test_mode mode;
  mode = MODE_AW;
  print_start_information(fileCount, KfileSize, blockSize, needclose, mode, is_distributed);

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
