#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "common.h"

int sock;

void getLine(char *message, char *buf, int maxLen)
{
  printf("%s", message);
  fgets(buf, maxLen, stdin);
  buf[strlen(buf) - 1] = 0;
}

void excCommand(char *command)
{
  if ( send(sock, command, strlen(command), 0) < 0)
    ErrorWithSystemMessage("send() failed");

  char replyMessage[BUFSIZE];
  ssize_t numBytesRecv = 0;
  do
    {
      numBytesRecv = recv(sock, replyMessage, BUFSIZE, 0);
      if ( numBytesRecv < 0)
	ErrorWithSystemMessage("recv() failed");
      printf("%s\n", replyMessage);
      memset(&replyMessage, 0, sizeof(replyMessage));

    }
  while (numBytesRecv > 0);
}

void PrintFile(const char *filename)
{
  FILE *fp;
  fp = fopen(filename, "r");
  if (fp)
    {
      char line[128];
      while (fgets(line, sizeof(line), fp) != NULL)
	fputs(line, stdout);
      fputs("\n", stdout);
      fclose(fp);
    }
}

int main(int argc, char *argv[])
{
  int quit = 1;
  char command[10];

  if (argc < 2 || argc > 3)
    {
      ErrorWithUserMessage("Parameter(s)", "<Server Address> <Server Port>");
    }

  char *server = argv[1];
  char *service = argv[2];

  sock = SetupTCPClientSocket(server, service);
  if (sock < 0)
    ErrorWithUserMessage("SetupTCPClientSocket() failed: ", "unable to connect");

  printf("Connection established!\n\n");

  PrintFile("menu.txt");
  excCommand("help");

  while (quit)
    {
      getLine("client> ", command, 10);
      if (strcmp(command, "quit") == 0)
	quit = 0;
      else
	excCommand(command);
    }

  fputs("\n", stdout);
  close(sock);
  exit(EXIT_SUCCESS);
}
