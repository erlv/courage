#include "FSLatency_distributed.h"

/* void print_usage() { */
/*   printf("Usage: test_server read_path  filesize(Byte) blocksize(Byte)\n"); */
/*   printf(" >>> path: a avaible path that can be read/write\n"); */
/*   printf(" >>> count: the number of files that each notification \n"); */
/*   printf(" >>> filesize: Each file size in Byte that will be write\n"); */
/*   printf(" >>> blocksize: Block Size per read/write\n"); */
/*   printf(" >>> close: \n"); */
/*   printf(" >>> \t 0: Use open-write/read-close steps for each block.\n"); */
/*   printf(" >>> \t 1: use  write-sync per block for each block.\n\n"); */
/*   printf(" >>> distributed mode: \n"); */
/*   printf(" >>> \t 0: Doing File System latency test locally without network.\n"); */
/*   printf(" >>> \t 1: Doing FS latency test distributedly, please enter the ip/port later.\n\n"); */
/*   printf(" Example: test  data/ 500 524288 524288 1 0\n"); */

/* } */

void print_usage() {
  printf("Usage: test path testcount filesize(Byte) blocksize(Byte) close(1|0) distribued(0|1)\n");
  printf(" >>> path: a avaible path that can be read/write\n");
  printf(" >>> count: the number of files that need to be read\n");
  printf(" >>> filesize: Each file size in Byte that will be read\n");
  printf(" >>> blocksize: Block Size per read\n");
  printf(">>> When run this daemon, the FSLatency test should be in distributed mode.\n");
  printf(">>> When compare with local mode, be sure that these parameters are the same for both modes.\n");

  printf(" Example: test  data/ 500 524288 524288\n");

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
  int fileCount=500;
  int fileSize=524288;
  int blockSize=524288;

  int read_fileCount = READS_PER_WRITE * fileCount;  
  prepare_env(read_fileCount, fileSize);


  
  int port=0;
  if(argc < 2) {
    port = 6789;
  } else {
    port = atoi(argv[1]);
  }


  print_start_information(fileCount, fileSize, blockSize, port);

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
  int servaddr_len = sizeof(servaddr);
  int clisock = accept(sock, (struct sockaddr *)NULL, NULL);
  if (clisock >= 0) {
    while(1) {
      int messageLength = 16;
      char message[messageLength+1];
      int in, index = 0, limit = messageLength;

      recv(clisock, message, messageLength,0);

      if(message[0]='r'){
	printf(">>>\t >start read file");
	op_file_read(fileCount, blockSize);
      } else {
	printf("Error Message. 'read' command is needed here.\n");
      }

      char* done_msg = "Done!";
      send(clisock, done_msg,  strlen(done_msg), 0);
      printf(">>>\t >Read files done! Notify the client.\n");
    }
    close(clisock);
  }
}
