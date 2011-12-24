#include<stdio.h>
#include<limits.h>

int main() {
	long long i=0;
	for(;;) {
		i ++;
		if( (i % LONG_MAX) == 0)
			printf("overflowed.\n");
	}
}
