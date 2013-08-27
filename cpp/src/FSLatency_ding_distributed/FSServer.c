/*
 * FS Latency Distributed Test Tool
 *
 * Author: Ling Kun  <lingkun@loongstore.com.cn>
 * Loongstore Inc.
 *
 */

#include "FSLatency_distributed.h"

void print_usage() {
  printf("Usage: FSServer count filesize(KB) MultiReader(0|1) distribued(1) port(int)\n");
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

void print_start_information(const int read_fcnt, const int read_filesize,
			     const int read_blocksize, const int port) {
  printf("Starting FSLatency distributed server daemon...\n");
  printf(">>> Listen port: %d.\n", port);
  printf(">>> Total read file count: %d.\n", read_fcnt);
  printf(">>> Read from Path:%s.\n", G_path);
  printf(">>> Each read file size: %dKB.\n", read_filesize);
  printf(">>> Each read block size: %dB.\n", read_blocksize);
  if( G_is_multithread_read) {
    printf(">>> Use Multithread to read files.\n");
  } else {
    printf(">>> Use single thread to read files.\n");
  }
}


int main(int argc, char **argv) {
  
  //Create 1000 files frist for read support
  int fileCount, fileSize, blockSize, read_fileCount, port;
  bool is_distributed;

  if(argc != 8) {
    printf("Wrong command line argument number!\n");
    print_usage();
    exit(0);
  }

  is_distributed = atoi(argv[5]);
  G_is_distributed = is_distributed;

  if( !is_distributed) {
    printf("Server daemon could only run in distributed mode.\n");
    print_usage();
    exit(0);
  }
  strcpy(G_path, argv[1]);
  fileCount = atoi(argv[2]);
  G_fileCount = fileCount;
  int KfileSize = atoi(argv[3]);
  fileSize = KfileSize*1024;
  G_fileSize= fileSize;
  G_is_multithread_read = atoi(argv[4]);
  strcpy(G_ipaddr, argv[6]);
  G_port = atoi(argv[7]);
  blockSize=G_blockSize;

  read_fileCount = READS_PER_WRITE * fileCount;  
  if(read_fileCount > MAX_READ_FILE_CNT) {
    read_fileCount = MAX_READ_FILE_CNT;
  }
  
  port=G_port;
  print_start_information(read_fileCount, KfileSize, blockSize, port);
  prepare_env(read_fileCount, fileSize);

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
  servaddr.sin_port = htons(port);

  if(bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("Sockect Bind");
    exit(-1);
  }

  printf(">>> Network listen port binding done.\n");
  /* try to specify maximum of 5 pending connections for the master socket */
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
	//printf(">>>\t >start read file\n");
	op_file_read(read_fileCount, blockSize);
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
