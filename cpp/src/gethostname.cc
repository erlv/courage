#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_STR_LEN 1024

int main() {

  char HostName[MAX_STR_LEN];
  HostName[1023]='\0';
  gethostname(HostName,1023);

  struct timeval tv_curr;
  gettimeofday(&tv_curr, NULL);

  printf("%d: HostName: %s\n", tv_curr.tv_sec, HostName);

  return 0;
}
