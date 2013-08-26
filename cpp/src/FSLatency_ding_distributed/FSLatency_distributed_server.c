/*
 * FS Latency Distributed Test Tool
 *
 * Author: Ling Kun  <lingkun@loongstore.com.cn>
 * Loongstore Inc.
 *
 */

#include "FSLatency_distributed.h"

void print_usage() {
  printf("Usage: FSServer count filesize distribued(1) port(int)\n");
  printf(" >>> count: the number of files that need to be read/written\n");
  printf(" >>> filesize: Each file size in Byte that will be write\n");
  printf(" >>> distributed mode: \n");
  printf(" >>> \t 1: Doing FS latency test distributedly, please enter the ip/port later.\n");
  printf(" >>> port: the port of server for distrituted test.\n");
  printf(" \n The Following config could be changed in FSLatency_distributed.h\n");
  printf(" >>> path: a avaible path that can be read/write\n");
  printf(" >>> blocksize: Block Size per read/write\n\n");
  printf(" Example: FSServer  500 524288 1 5678 \n");
}

void print_start_information(const int read_fcnt, const int read_filesize,
			     const int read_blocksize, const int port) {
  printf("Starting FSLatency distributed server daemon...\n");
  printf(">>> Listen port: %d.\n", port);
  printf(">>> Total read file count: %d.\n", read_fcnt);
  printf(">>> Each read file size: %d.\n", read_filesize);
  printf(">>> Each read block size: %d.\n", read_blocksize);
}



int main(int argc, char **argv) {
  
  //Create 1000 files frist for read support
  int fileCount, fileSize, blockSize, read_fileCount, port;
  bool is_distributed;

  if(argc != 5) {
    printf("Wrong command line argument number!\n");
    print_usage();
    exit(0);
  }

  is_distributed = atoi(argv[3]);
  if( !is_distributed) {
    printf("Server daemon could only run in distributed mode.\n");
    print_usage();
    exit(0);
  }

  fileCount = atoi(argv[1]);
  fileSize = atoi(argv[2]);
  G_port = atoi(argv[4]);
  blockSize=G_blockSize;
  read_fileCount = READS_PER_WRITE * fileCount;  


  
  port=G_port;

  print_start_information(fileCount, fileSize, blockSize, port);

  prepare_env(read_fileCount, fileSize);

  struct sockaddr_in servaddr;  
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  if(bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind");
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
	op_file_read(fileCount, blockSize);
      } else {
	printf("Error Message. 'read' command is needed here.\n");
      }

      char* done_msg = "Done!";
      send(clisock, done_msg,  strlen(done_msg), 0);
      // printf(">>>\t >Read files done! Notify the client.\n");
    }
    close(clisock);
  }
  return 0;
}
