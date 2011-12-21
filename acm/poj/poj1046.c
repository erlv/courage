#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <values.h>

struct color {
	int red;
	int green;
	int blue;
};

struct to_map {
	int red;
	int green;
	int blue;
	struct to_map* next;
	int map_to_idx;
};

int main () {

	int i,j;

	struct color map_arr[16];
	for(i=0; i < 16; i++) {
		scanf("%d %d %d", &map_arr[i].red, &map_arr[i].green, &map_arr[i].blue);
	}

	struct to_map* head = malloc( sizeof(struct to_map) );
	struct to_map* p = head;
	scanf("%d %d %d", &p->red, &p->green, &p->blue);
	p->next = NULL;
	p->map_to_idx = -1;
	while( !( (p->red == -1) && ( p->green == -1) && (p->blue == -1)) ) {
	
		double min_dis = MAXFLOAT;
	        int min_idx = -1;

		
		int j;
		for( j = 0; j < 16; j++) {
			int sub_red = map_arr[j].red - p->red;
			int sub_green = map_arr[j].green - p->green;
			int sub_blue = map_arr[j].blue - p->blue;
			double orign=(double)(sub_red*sub_red + sub_green * sub_green + sub_blue * sub_blue);
			double dis = sqrt( orign );
			if( dis < min_dis ) {
				min_dis = dis;
				min_idx = j;
			}
		}
		p->map_to_idx = min_idx;

		printf("(%d,%d,%d) maps to (%d,%d,%d)\n", p->red, p->green, p->blue, 
				map_arr[min_idx].red, map_arr[min_idx].green, map_arr[min_idx].blue);

		struct to_map* q = malloc( sizeof(struct to_map) );
		p->next = q;
		p = q;
		scanf("%d %d %d", &p->red, &p->green, &p->blue);
		p->next = NULL;
		p->map_to_idx = -1;
	}

	return 0;
}
