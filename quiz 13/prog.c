#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

double BALANCE=0.0; //volatile global variable

typedef struct param {
    int x;
    double y;
} paramT;

void *deposit(void *args){
    //takes a (int) and b (double) as parameters and executes
    int i;
    paramT *ptr = (paramT *) args;

    for(i = 0; i < ptr->x; i++){
        BALANCE += ptr->y;
    }
    return NULL;
}

void *withdraw(void *args){
    //takes c (int) and d (double) as parameters and executes
    int i;
    paramT *ptr = (paramT *) args;

    for(i = 0; i < ptr->x; i++){
        BALANCE -= ptr->y;
    }
    return NULL;
}


void main(int argc, char *argv[]){
    if(argc != 5){ //#deposits amountEachDeposit #withdrawal amountEachWithdrawal
        printf("Incorrect number of arguments passed\n");
        return;
    }

    int nDep = atoi(argv[1]);
    double amtDep = atof(argv[2]);
    int nWDrawal = atoi(argv[3]);
    double amtWDrawal = atof(argv[4]);

    paramT dep, with;
    pthread_t t1, t2;
    double total;

    dep.x = nDep;           //a
    dep.y = amtDep;         //b
    with.x = nWDrawal;      //c
    with.y = amtWDrawal;    //d

    //printf("%d %f %d %f\n", nDep, amtDep, nWDrawal, amtWDrawal);

    pthread_create(&t1, NULL, deposit, &dep);
    pthread_create(&t2, NULL, withdraw, &with);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    total = (dep.x * dep.y) - (with.x * with.y);
    printf("B = %lf vs T = %lf\n", BALANCE, total);

    return;
}

// adolf@Needler ~/Operating Systems/quiz 13
// $ make
// gcc -c -o prog.o prog.c
// gcc -o prog prog.o

// adolf@Needler ~/Operating Systems/quiz 13
// $ ./prog.exe 1 2.3 3 5.32
// 1 2.300000 3 5.320000
// B = -13.660000 vs T = -13.660000

// adolf@Needler ~/Operating Systems/quiz 13
// $ ./prog.exe 2 2.5 3 5.0
// 2 2.500000 3 5.000000
// B = -10.000000 vs T = -10.000000

// adolf@Needler ~/Operating Systems/quiz 13
// $ ./prog.exe 2 2.5 3 5.0
// 2 2.500000 3 5.000000
// B = -10.000000 vs T = -10.000000

// adolf@Needler ~/Operating Systems/quiz 13
// $ ./prog.exe 2 2.5 3 5.0
// 2 2.500000 3 5.000000
// B = -10.000000 vs T = -10.000000

// adolf@Needler ~/Operating Systems/quiz 13
// $ ./prog.exe 2 2.5 3 5.0
// 2 2.500000 3 5.000000
// B = -10.000000 vs T = -10.000000

// adolf@Needler ~/Operating Systems/quiz 13
// $ ./prog.exe 2 2.5 3 5.0
// 2 2.500000 3 5.000000
// B = -10.000000 vs T = -10.000000

// adolf@Needler ~/Operating Systems/quiz 13
// $ ./prog.exe 1000 3.5 500 2.8
// 1000 3.500000 500 2.800000
// B = 2100.000000 vs T = 2100.000000

// adolf@Needler ~/Operating Systems/quiz 13
// $ ./prog.exe 10000 10.8 5000 5.7
// 10000 10.800000 5000 5.700000
// B = 79500.000000 vs T = 79500.000000


/*
My balance was a global variable and was therefore shared between the two threads 
allowing for seamless interaction with each other. This made both outputs the same.
*/