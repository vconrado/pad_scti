#include <stdio.h>
#include <stdlib.h>
#include "pad_time.h"
#include "omp.h"

#define ROWS 10000
#define COLS 10000

/*

 Indexação Matrix

  i-1, j-1 | i-1,j  | i-1, j+1
    i, j-1 |   i,j  |   i, j+1
  i+1, j-1 | i+1,j  | i+1, j+1
 

  export OMP_NUM_THREADS=2
*/


// basic
void calc1(float **in, float **out){
    size_t i,j;
    for(j=1; j<COLS-1; ++j){
        for(i=1; i<ROWS-1; ++i){
            out[i][j] = (   in[i-1][j-1] + in[i-1][j] + in[i-1][j+1] + 
                            in[i][j-1]   + in[i][j]   + in[i][j+1] + 
                            in[i+1][j-1] + in[i+1][j] + in[i+1][j+1])/9.0;
                            
        }
    }
}


// uso da cache
void calc2(float **in, float **out){
    size_t i,j;
    for(i=1; i<ROWS-1; ++i){
        for(j=1; j<COLS-1; ++j){
            out[i][j] = (   in[i-1][j-1] + in[i-1][j] + in[i-1][j+1] + 
                            in[i][j-1]   + in[i][j]   + in[i][j+1] + 
                            in[i+1][j-1] + in[i+1][j] + in[i+1][j+1])/9.0;
        }
    }
}

// loop unroling
void calc3(float **in, float **out){
    size_t i,j;
    for(i=1; i<ROWS-4; i+=4){
        for(j=1; j<COLS-1; ++j){
            out[i][j] = (   in[i-1][j-1] + in[i-1][j] + in[i-1][j+1] + 
                            in[i][j-1]   + in[i][j]   + in[i][j+1] + 
                            in[i+1][j-1] + in[i+1][j] + in[i+1][j+1])/9.0;
            // i+1
            out[i+1][j] = ( in[i][j-1] + in[i][j]     + in[i][j+1] + 
                            in[i+1][j-1] + in[i+1][j] + in[i+1][j+1] + 
                            in[i+1][j-2] + in[i+2][j] + in[i+2][j+1])/9.0;
            // i+2
            out[i+2][j] = ( in[i+1][j-1] + in[i+1][j] + in[i+1][j+1] + 
                            in[i+2][j-1] + in[i+2][j] + in[i+2][j+1] + 
                            in[i+3][j-2] + in[i+3][j] + in[i+3][j+1])/9.0;
            // i+3
            out[i+3][j] = ( in[i+2][j-1] + in[i+2][j] + in[i+2][j+1] + 
                            in[i+3][j-1] + in[i+3][j] + in[i+3][j+1] + 
                            in[i+4][j-2] + in[i+4][j] + in[i+4][j+1])/9.0;
        }
    }
}


// openmp outer loop
void calc4(float **in, float **out){
    size_t i,j;
    #pragma omp parallel for shared(out, in)
    for(i=1; i<ROWS-1; ++i){
        for(j=1; j<COLS-1; ++j){
            out[i][j] = (   in[i-1][j-1] + in[i-1][j] + in[i-1][j+1] + 
                            in[i][j-1]   + in[i][j]   + in[i][j+1] + 
                            in[i+1][j-1] + in[i+1][j] + in[i+1][j+1])/9.0;
        }
    }
}

// openmp inner loop
void calc5(float **in, float **out){
    size_t i,j;
    for(i=1; i<ROWS-1; ++i){
        #pragma omp parallel for shared(out, in)
        for(j=1; j<COLS-1; ++j){
            out[i][j] = (   in[i-1][j-1] + in[i-1][j] + in[i-1][j+1] + 
                            in[i][j-1]   + in[i][j]   + in[i][j+1] + 
                            in[i+1][j-1] + in[i+1][j] + in[i+1][j+1])/9.0;
        }
    }
}


float** create_matrix(){
    size_t i;
    float** mat = (float**) malloc(sizeof(float*)*ROWS);
    for(i=0; i<ROWS; i++){
        mat[i] = (float*) malloc(sizeof(float)*COLS);
    }
    return mat;
}

int main(int argc, char **argv) {
    uint64_t start_time, end_time;
    size_t i, j;
    float **in;
    float **out;

    in = create_matrix();

    for(i=0; i<ROWS; ++i){
        for(j=0; j<COLS; ++j){
            in[i][j] = (float)i/(j+1.);
        }
    }


    out = create_matrix();
    
    start_time = micros_since_epoch();
    calc1(in, out);
    end_time = micros_since_epoch();
    printf("Elapsed time: %2.6f ms\n", (end_time-start_time)/1000.);


    start_time = micros_since_epoch();
    calc2(in, out);
    end_time = micros_since_epoch();
    printf("Elapsed time: %2.6f ms\n", (end_time-start_time)/1000.);
    
    
    start_time = micros_since_epoch();
    calc3(in, out);
    end_time = micros_since_epoch();
    printf("Elapsed time: %2.6f ms\n", (end_time-start_time)/1000.);
    
    start_time = micros_since_epoch();
    calc4(in, out);
    end_time = micros_since_epoch();
    printf("Elapsed time: %2.6f ms\n", (end_time-start_time)/1000.);

    
    start_time = micros_since_epoch();
    calc5(in, out);
    end_time = micros_since_epoch();
    printf("Elapsed time: %2.6f ms\n", (end_time-start_time)/1000.);

    return 0;
}
