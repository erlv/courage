#include <stdio.h>

int main() {

    int tt_case=0;
    scanf("%d", &tt_case);
    
    int i=0;
    for(; i < tt_case; i++) {
        float tt_leng;
        int m = 0;
        int n = 0;
        scanf("%d %d", &m, &n);
        
        if( (n%2 == 0) && ( m > 2 ) ) {
            tt_leng = m*n+0.41;
        } else {
            tt_leng = m*n;
        }
    
        printf("Scenario #%d:\n%.2f\n", i, tt_leng);
    }
    return 0;
}
