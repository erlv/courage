/*
 * =====================================================================================
 *
 *       Filename:  localArrayReturnWarning.cpp
 *
 *    Description:  This is an example code about how to prevent local array warning
 *
 *        Version:  1.0
 *        Created:  05/11/2015 08:41:22 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kun Ling (), kunling@lingcc.com
 *   Organization:  Lingcc.com
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

char*  returnLocalArray() {
  char localArr[100] = {'a', 'b', 'c', 'd'};
  return &localArr[0];
}

char* returnHeapArray() {
  char* heapArr = (char*) malloc(sizeof(char) * 100);
  heapArr[0] = '1';
  heapArr[1] = '2';
  heapArr[2] = '3';
  heapArr[3] = '4';
  heapArr[4] = '5';
  heapArr[5] = '6';
  return heapArr;
}

int main() {
  char* localret = returnLocalArray();
  char* heapret = returnHeapArray();
  printf("localret:0x%x, heapret:0x%x\n", localret, heapret);
  printf("localarray:%s.\n", localret);
  printf("heaparray:%s.\n", heapret);
  return 0;
}
