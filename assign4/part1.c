#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#ifndef RAND_MAX
#define RAND_MAX ((int) ((unsigned) ~0 >> 1))
#endif
#define MAX_ARR 20000

int arraySize;
int NUM_THREADS = 2;
double dArr[MAX_ARR];
double sorted[MAX_ARR]; //sorted array to insert
long t;


//Returns a random double within the range low-high
double RandomReal(double low, double high){
    double d;
    d = (double) rand() / ((double) RAND_MAX + 1);
    return (low + (d * (high - low)));
}

void *sort(void *threadid){
    int i, j;
    int n = arraySize/NUM_THREADS; //number of elements in array
    double temp; //temp value
    for(i = t * n; i < n * (t + 1); i++){ //t is current thread
        for(j = i + 1; j < n * (t + 1); j++){
            if(dArr[i] > dArr[j]){
                temp = dArr[i];
                dArr[i] = dArr[j];
                dArr[j] = temp;
            }
        }
    }
    pthread_exit(NULL);
}

void sort2Arrays(int sizeArr){
    int i = 0;
    int j = 0;
    int k = 0;
    int x;
    int z = 0;
    double a[sizeArr];
    double b[sizeArr];
    for(x = 0; x < sizeArr; x++){
        a[x] = dArr[z];
        z++;
    }
    for(x = 0; x < sizeArr; x++){
        b[x] = dArr[z];
        z++;
    }
    int m = arraySize/NUM_THREADS;
    int n = arraySize;
    double c[arraySize];
    for(i = 0; i < m + n; ){
        if(j < m && k < n){
            if(a[j] < b[k]){
                sorted[i] = a[j];
                j++;
            }
            else{
                sorted[i] = b[k];
                k++;
            }
            i++;
        }
        else if(j == m){
            for(; i < m + n; ){
                sorted[i] = b[k];
                k++;
                i++;
            }
        }
        else{
            for(; i < m + n; ){
                sorted[i] = a[j];
                j++;
                i++;
            }
        }
    }
    //printf("\nSorted:\n");
    //for(i = 0; i < arraySize; i++){
    //    printf("%f\n", sorted[i]);
    //}
}

void main(int argc, char *argv[]){
    if(argc < 2){ //size of array
        printf("Incorrect number of arguments passed\n");
        return;
    }

    clock_t begin;
    begin = clock();
    double cpu_time_used = 0.0;

    int error;
    int rc;
    arraySize = atoi(argv[1]); //parameter passed in
    if(argc == 3){
        t = atoi(argv[2]);
        NUM_THREADS = t;
    }
    pthread_t threads[NUM_THREADS];
    int k;

    for(k = 0; k < arraySize; k++){ //initialize the double array to random doubles
        dArr[k] = RandomReal(1.0, 1000.0);
    }
    //printf("Before:\n");
    //for(k = 0; k < arraySize; k++){
    //    printf("%f\n", dArr[k]);
    //}
    //printf("\n");

    for(t = 0; t < NUM_THREADS; t++){
        rc = pthread_create(&threads[t], NULL, sort, (void *) t); //2 threads
        if(rc){
            printf("Error: %d\n", rc);
            exit(-1);
        }
    }

    for(t = 0; t < NUM_THREADS; t++){
        pthread_join(threads[t], NULL); //wait for threads
    }

                //arr, start, arr2, start2, output
    sort2Arrays(arraySize/NUM_THREADS);

    clock_t end = clock();
    double finish = (double) end;
    double start = (double) begin;
    cpu_time_used += ((double) (end - begin)) / (double) CLOCKS_PER_SEC;
    printf("Sorting is done in %fms when %d threads are used\n", cpu_time_used, t);


    //1 Thread
    begin = clock();
    for(k = 0; k < arraySize; k++){ //initialize the double array to random doubles
        dArr[k] = RandomReal(1.0, 1000.0);
    }
    sort;
    sort2Arrays(arraySize);
    end = clock();
    finish = (double) end;
    start = (double) begin;
    cpu_time_used += ((double) (end - begin)) / (double) CLOCKS_PER_SEC;
    printf("Sorting is done in %fms when one threads are used\n", cpu_time_used);

}

