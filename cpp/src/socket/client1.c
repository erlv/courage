#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>


int main(int argc, char **argv) {

  struct sockaddr_in servaddr;

  // Creat socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);


  int port=0;
  if(argc < 2) {
    port = 6789;
  } else {
    port = atoi(argv[1]);
  }


  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");


  while(1) {

    // char message[161];
    // fgets(message, 161, stdin);
    char* message = "helloworld.\n";

    /* Replacing '\n' with '\0' */
    // char *tmp = strchr(message, '\n');
    //if (tmp) *tmp = '\0';
  
    connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Send:%s\n", message);

    send(sock, message, strlen(message), 0); 

    char done_msg[16]={0};

    // Should wait here
    int in_len = recv(sock, done_msg, 16,0);
    printf("Recv:%s", done_msg);
  }
  sleep(2);
  close(sock);

}
