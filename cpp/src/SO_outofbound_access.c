/*
 * =====================================================================================
 *
 *       Filename:  SO_outofbound_access.c
 *
 *    Description:  Check the compiler memory access bound checking framework of 
 *                  different tools.
 *
 *                  valgrind, gcc -fsanitize=address, clang -fsanitize=address
 *                  could help to detect such kind of issues.
 *
 *                  However, simple static check like -Warray-bound
 *                  -Warray-bounds-pointer-arithmetic does not help.
 *
 *        Version:  1.0
 *        Created:  05/11/2015 09:16:43 PM
 *       Revision:  none
 *       Compiler:  gcc/clang
 *
 *         Author:  Kun Ling (), kunling@lingcc.com
 *   Organization:  Lingcc.com
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *p = malloc(10);
    int i;
    for(i=0;i<15;i++)
    {
        p[i]='c';
        printf("INDEX:%d  %c\n",i,p[i]);
    }

    return 0;
}
