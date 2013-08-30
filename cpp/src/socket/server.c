#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

void HandleTCPClient(int clntSock)
{
  char buffer[BUFSIZE];
  ssize_t numBytesRcvd = recv(clntSock, buffer, BUFSIZE, 0);
  buffer[numBytesRcvd] = '\0';
  if (numBytesRcvd < 0)
    ErrorWithSystemMessage("recv() failed");
  if (strcmp(buffer, "help") == 0)
    {
      FILE *fp = fopen("help.txt", "r");
      if (fp)
        {
	  char line[128];
	  while (fgets(line, sizeof(line), fp) != NULL)
            {
	      if (send(clntSock, line, sizeof(line), 0) < 0)
		ErrorWithSystemMessage("send() failed");
            }
	  fclose(fp);
        }
    }

  close(clntSock);
}


int main(int argc, char *argv[])
{
  if (argc != 2)
    ErrorWithUserMessage("Parameter(s), <Server Port>");

  char *service = argv[1];

  int servSock = SetupTCPServerSocket(service);
  if (servSock < 0)
    ErrorWithUserMessage("SetupTCPServerSocket() failed: unable to establish");

  unsigned int childProcessCount = 0;
  while (1)
    {
      int clntSock = AcceptTCPConnection(servSock);

      pid_t processID = fork();
      if (processID < 0)
	ErrorWithSystemMessage("fork() failed");
      else if (processID == 0)
        {
	  close(servSock);
	  HandleTCPClient(clntSock);
	  exit(EXIT_SUCCESS);
        }

      printf("with child process: %d\n", processID);
      close(clntSock);
      childProcessCount++;

      //clean up zombies
      while (childProcessCount)
        {
	  processID = waitpid((pid_t) - 1, NULL, WNOHANG);
	  if (processID < 0)
	    ErrorWithSystemMessage("waitpid() failed");
	  else if (processID == 0)
	    break;
	  else
	    childProcessCount--;
        }

    }

}
