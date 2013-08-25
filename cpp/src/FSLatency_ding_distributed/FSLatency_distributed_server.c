#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int main(int argc, char **argv) {

  struct sockaddr_in servaddr;  
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  
  int port=0;
  if(argc < 2) {
    port = 6789;
  } else {
    port = atoi(argv[1]);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  if(bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind");
    exit(-1);
  }

  printf("bind done.\n");
  /* try to specify maximum of 3 pending connections for the master socket */
  if (listen(sock, 5)<0) {
    /* if listen failed then display error and exit */
    perror("listen");
    exit(-1);
  }
  printf("listen done. \n");

  // Accept socket connection
  int servaddr_len = sizeof(servaddr);
  int clisock = accept(sock, (struct sockaddr *)NULL, NULL);
  printf("clisock=%d.\n",  clisock); 


  if (clisock >= 0) {
    while(1) {
      int messageLength = 160;
      char message[messageLength+1];
      int in, index = 0, limit = messageLength;

      int in_len = recv(clisock, message, messageLength,0);
      printf( "Recv:%s\n", message);

      char done_msg[5] = {'a','a','a','a',0};

      int send_ret = send(clisock, done_msg,  strlen(done_msg), 0);
      printf("Send:%s\n", done_msg);
    }
    close(clisock);

  }
}
