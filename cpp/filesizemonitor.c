/*This is the sample program to notify us for the file creation and file deletion takes place in “/tmp/test_inotify” file*/
// reference: http://www.thegeekstuff.com/2010/04/inotify-c-program-example/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/inotify.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int main( )
{
  int length, i = 0;
  int fd;
  int wd;
  char buffer[EVENT_BUF_LEN];

  /*creating the INOTIFY instance*/
  fd = inotify_init();
  /*checking for error*/
  if ( fd < 0 ) {
    perror( "inotify_init error" );
  }

  /* adding the “/tmp/test_inotify” test into watch list. Here, 
   * the suggestion is to validate the existence of the 
   * directory before adding into monitoring list.
   */
  wd = inotify_add_watch( fd, "/tmp/test_inotify", IN_CREATE | IN_DELETE | IN_ACCESS | IN_MODIFY | IN_OPEN );

  /* read to determine the event change happens on “/tmp/test_inotify” file. 
   * Actually this read blocks until the change event occurs
   */ 
  length = read( fd, buffer, EVENT_BUF_LEN ); 
  /* checking for error */
  if ( length < 0 ) {
    perror( "read" );
  }  

  /* actually read return the list of change events happens. 
   *  Here, read the change event one by one and process it accordingly.
   */
  while ( i < length ) {
    struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
    if( event->len == 0) {
      // For a single file watching, the event->name is empty, and event->len = 0
      printf(" Single file watching event happened\n");
    } else if ( event->len ) {
      if ( event->mask & IN_CREATE ) {
        if ( event->mask & IN_ISDIR ) {
          printf( "New directory %s created.\n", event->name );
        } else {
          printf( "New file %s created.\n", event->name );
        }
      } else if ( event->mask & IN_DELETE ) {
        if ( event->mask & IN_ISDIR ) {
          printf( "Directory %s deleted.\n", event->name );
        } else {
          printf( "File %s deleted.\n", event->name );
        }
      } else if( event->mask & IN_ACCESS ) {
        if ( event->mask & IN_ISDIR ) {
          printf( "Directory %s accessed.\n", event->name );
        } else {
	  printf(" File %s accessed. \n", event->name );
        }
      } else if( event->mask & IN_MODIFY ) {
        if ( event->mask & IN_ISDIR ) {
          printf( "Directory %s modified.\n", event->name );
        } else {
	  printf(" File %s modified. \n", event->name );
        }
      } else if( event->mask & IN_OPEN ) {
        if ( event->mask & IN_ISDIR ) {
          printf( "Directory %s opened.\n", event->name );
        } else {
	  printf(" File %s opened. \n", event->name );
        }
      } else {
	printf( "Directory or File is accessed by other mode\n");
      }
    }
    i += EVENT_SIZE + event->len;
  }

  /* removing the “/tmp/test_inotify” directory from the watch list. */
  inotify_rm_watch( fd, wd );

  /* closing the INOTIFY instance */
  close( fd );

}
