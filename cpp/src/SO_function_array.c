#include <stdio.h>
#include <stdlib.h>

/**
 * This file is used to answer stackoverflow question: 
 *
 *   http://stackoverflow.com/questions/16728602/c-function-returning-an-array
 *
 */

char string1[10] = "123456789";


char x(void)[10];

int main(void) {
    printf("string returned by x() is %s",x());
    return EXIT_SUCCESS;
}

char x(void)[10] {
    return x;
}
