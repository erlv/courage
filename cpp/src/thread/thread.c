/*
 * =====================================================================================
 *
 *       Filename:  thread.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/21/2015 06:42:12 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kun Ling (), kunling@lingcc.com
 *   Organization:  Lingcc.com
 *
 * =====================================================================================
 */

#include <pthread.h>
#include <stdio.h>

void* sayHello (void *x){
    printf ("Hello, this is %d\n", (int)pthread_self());
    return NULL;
}

int main (){
    pthread_t thread;
    pthread_create (&thread, NULL, &sayHello, NULL);
    printf("HERE\n");
    return 0;
}
