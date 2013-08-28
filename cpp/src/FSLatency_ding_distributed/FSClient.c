/*
 * FS Latency Distributed Test Tool
 *
 * Author: Ling Kun  <lingkun@loongstore.com.cn>
 * Loongstore Inc.
 *
 */

#include "FSCommon.h"

enum T_testRole {
  ROLE_SERVER,
  ROLE_CLIENT,
};

void parse_options() {

  if(!(argc == 7  || argc ==9)) {
    printf("Error: does not have correct number of parameter.\n");
    print_usage();
    exit(0);
  }

  G_is_distributed = atoi(argv[6]);
  if(G_is_distributed && argc != 9 ) {
    printf("Error: Please check the parameter for Distributed mode.\n");
    print_usage();
    exit(0);
  } else if( !G_is_distributed && (argc != 7)) {
    printf("Error: Please check the parameter for local mode.\n");
    print_usage();
    exit(0);
  }

  if(argv[1][0] == 's' ) {
    G_testRole = ROLE_SERVER;
  } else if (argv[1][0] == 'c') {
    G_testRole = ROLE_CLIENT;
  } else {
    printf("Error: Not an available row, only Server(s)/Client(c) is acceptable.\n");
    print_usage();
    exit(0);
  }

  strcpy(G_path, argv[2]);
  G_write_fileCount = atoi(argv[3]);
  G_kfileSize = atoi(argv[4]);
  G_fileSize = G_kfileSize*1024;
  G_is_multithread_read = atoi(argv[5]);
  if(G_is_distributed) {
    strcpy(G_ipaddr, argv[7]);
    G_port = atoi(argv[8]);
  }
  G_read_fileCount = READS_PER_WRITE * G_write_fileCount;
  if(G_read_fileCount > MAX_READ_FILE_CNT) {
    G_read_fileCount= MAX_READ_FILE_CNT;
  }
}


int main(int argc, char* argv[]) {
  parse_options();
  print_start_information();

  if (G_is_multithread_read) {
    printf(">>> Init %d threads for multiple thread read.\n", READS_PER_WRITE);
    int i=0;
    for(;  i < READS_PER_WRITE; i++) {
      sem_init(&sem_read_start[i],0,0);
      sem_init(&sem_read_end[i],0,0);
      G_thread_idx[i]=i;
      pthread_create(&G_t[i],NULL, (void*)single_file_read_thread, &G_thread_idx[i]);
    }
  }
  do_test();
  return 0;
}
