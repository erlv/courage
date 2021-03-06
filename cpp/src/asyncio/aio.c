#include <aio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>


#define SIZE_TO_READ 100

int main()
{
  // open the file
  int file = open("blah.txt", O_RDONLY, 0);
	
  if (file == -1)
    {
      printf("Unable to open file!\n");
      return 1;
    }
	
  // create the buffer
  char buffer[SIZE_TO_READ] = {0};
	
  // create the control block structure
  struct aiocb cb;
	
  memset(&cb, 0, sizeof(struct aiocb));
  cb.aio_nbytes = SIZE_TO_READ;
  cb.aio_fildes = file;
  cb.aio_offset = 0;
  cb.aio_buf = buffer;
	
  // read!
  if (aio_read(&cb) == -1)
    {
      printf("Unable to create request!\n");
      close(file);
    }
	
  printf("Request enqueued!\n");
	
  // wait until the request has finished
  while(aio_error(&cb) == EINPROGRESS)
    {
      printf( "Working...\n");
    }
	
  // success?
  int numBytes = aio_return(&cb);
	
  if (numBytes != -1)
    printf("Success!\n");
  else
    printf("Error!\n");
		
  // now clean up
  //  delete[] buffer;
  close(file);
	
  return 0;
}
