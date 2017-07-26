#include <stdio.h>
#include <stdlib.h>
#include "pad_time.h"


void calc(){
    pad_usleep(100);
}

int main(int argc, char **argv) {
    uint64_t start_time, end_time;
    size_t i;
    // 1 execucacao
    start_time = micros_since_epoch();
    pad_usleep(100);
    end_time = micros_since_epoch();

    printf("Elapsed time: %lu us\n", (end_time-start_time));


    // Tempo médio
    int n=10000;
    start_time = micros_since_epoch();
    for(i=0; i<n; ++i){
        pad_usleep(100);
    }
    end_time = micros_since_epoch();
    printf("Elapsed time: %f us\n", ((float)end_time-(float)start_time)/(float)n);
    printf("Elapsed time: %lu us\n", (end_time-start_time)/n);
    return 0;
}
