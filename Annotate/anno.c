#include <stdio.h>
#include "../src/anno.h"

int main(int argc, char *argv[]) {
    int i_sum = 0;
    int i = 0;

    APPROX(1, _START);
    for(; i<14; i++){
        i_sum += i;
    }
    APPROX(1, _END);

    printf("i_sum= %d\n", i_sum);

    double d_sum = 0;

    APPROX(2, _START);
    d_sum += 12.3;
    d_sum += 45.6;
    /*for(i=50; i<55; i++){
        d_sum += 12.345;
    }*/
    APPROX(2, _END);

    printf("d_sum= %f\n", d_sum);

    return 1;
}
