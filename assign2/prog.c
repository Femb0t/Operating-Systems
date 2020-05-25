#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

double dAvgWaitTime = 0;       //time in ready queue
double dAvgTurnaroundTime = 0; //time to finish each process
double dAvgJobTime = 0;        //total execution time
double dThroughput;            //#processes / execution time

typedef struct PCB{
    int pID[8];      //Process number
    int pPriority[8];//Process Priority (for PR)
    int cpuBurst;    //CPU Utilization time (ms)
    int reg[8];      //PID array
    int qEnterClk;   //time process entered queue
    int waitTime;    //process wait time
    int numOfProcesses; //number of processes read
    struct PCB *next;
} PCB;

typedef struct CPU{

} CPU;

void readFile(char *file, PCB *pcb){
    FILE *filein = fopen(file, "r");
    int i = 0;

    if(filein == NULL){
        printf("Error opening file\n");
        fclose(filein);
        exit;
    }

    char readLine[256];
    int col0;
    int col1;
    int col2;
    char *p = (char *) malloc(10*sizeof(char));

    while(!feof(filein)){
        fscanf(filein, "%d %d %d", &col0, &col1, &col2);
        printf("%d %d %d\n", col0, col1, col2);
        pcb->pID[i] = col0;       //pID
        pcb->pPriority[i] = col1; //pPriority
        pcb->cpuBurst = col2;     //cpuBurst
        pcb->qEnterClk = 0;
        pcb->waitTime = 0;
        pcb->reg[i] = col2;       //array of cpu time
        i++;
        //save as last
    }
    pcb->numOfProcesses = i;
    printf("\n");

    fclose(filein);
    free(p);
    return;
}

void fifo(PCB *pcb){
    int last = 0;
    int totalJobtime = 0; //total time for all processes
    int waitTime = 0;
    int turnaround = 0;
    int i = 0;

    while(i < pcb->numOfProcesses){
        pcb->qEnterClk += last;       //time process entered the queue
        waitTime += last;             //for finding the average wait time (p1 = 0, p2 = p1->reg[i], p3 = p2 + p1)
        last += pcb->reg[i];          //add the current cpuBurst for the next process
        totalJobtime += pcb->reg[i];  //add cpuBurst to total execution time
        turnaround += pcb->reg[i];    //sum of cpuBursts
        printf("Process %d is complete at %d ms\n", pcb->pID[i], last);
        //increment to next pcb
        i++;
    }
    printf("\n");
    double avgWaitTime = (double) waitTime/pcb->numOfProcesses;
    double avgTurnaround = (double) turnaround/pcb->numOfProcesses;
    double throughput = (double) pcb->numOfProcesses/totalJobtime;
    printf("Average Waiting Time = %-5.2f %2s (%d/%d)\n", avgWaitTime, "ms", waitTime, i); //avg wait, (totalwait, #processes)
    printf("Average Turnaround Time = %-5.2f %2s (%d/%d)\n", avgTurnaround, "ms", turnaround, i); //avg turnaround, turnaround, #processes
    printf("Throughput = %3.2f %-11s (%d/%d)\n", throughput, "jobs per ms", i, totalJobtime);//throughput, #processes, totaljobtime
}

void swap(int *xp, int *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
}

void sjf(PCB *pcb){
    int i = 0;
    int j = 0;
    int remaining = pcb->numOfProcesses;

    for(i = 0; i < remaining - 1; i++){
        for(j = 0; j < remaining - i - 1; j++){
            //sort based on cpu time
            if(pcb->reg[j] > pcb->reg[j + 1]){
                swap(&pcb->reg[j], &pcb->reg[j + 1]); //sort shortest first
                swap(&pcb->pID[j], &pcb->pID[j + 1]); //keep pID array matched with above sort (1:1)
            }
            //call fifo
        }
    }
    fifo(pcb);
}

void pr(PCB *pcb){
    int i = 0;
    int j = 0;
    int remaining = pcb->numOfProcesses;

    for(i = 0; i < remaining - 1; i++){
        for(j = 0; j < remaining - i - 1; j++){
            //sort based on priority
            if(pcb->pPriority[j] > pcb->pPriority[j + 1]){
                swap(&pcb->pPriority[j], &pcb->pPriority[j + 1]); //sort shortest first
                swap(&pcb->reg[j], &pcb->reg[j + 1]);
                swap(&pcb->pID[j], &pcb->pID[j + 1]); //keep pID array matched with above sort (1:1)
            }
            //call fifo
        }
    }
    fifo(pcb);
}
//input example:
//pID pPriority cpuBurst

void rr(PCB *pcb, int q){
    int i = 1;
    int j = 0;
    int k = 0;
    int l = 0;
    bool check = true;
    int last = 0;
    int waitTime = 0;
    int totalJobtime = 0;
    int turnaround = 0;
    int remaining = 0;

    while(j < pcb->numOfProcesses){
        for(i = 1; i <= q; i++){      //calc up to quantum
            if(pcb->reg[k] > 0){  //burst > 0, can still be used
                turnaround += 1;      //add to total cpu time
                pcb->reg[k] -= 1; //decrement remaining cpu time
                totalJobtime += 1;    //add cycle to total job time
                last += 1;
            }
            if(pcb->reg[k] == 0 && pcb->pID[k] != 0){ //burst = 0 and pID not cleared yet, process complete
                printf("Process %d is complete at %d ms\n", pcb->pID[k], totalJobtime);
                pcb->pID[k] = 0; //set pID to 0 for the process so we don't need to calculate it
                break;
            }
            waitTime += i;             //for finding the average wait time (p1 = 0, p2 = p1->reg[i], p3 = p2 + p1)
            pcb->qEnterClk += last;           //time process entered the queue
        }              
        j++;
        k++;
        if(j == pcb->numOfProcesses){
            for(l = 0; l < pcb->numOfProcesses; l++){
                if(pcb->pID[l] == 0){ //check last process
                    continue;
                }
                else{
                    j = 0;
                    waitTime += 1;
                    break;
                }
            }
        }
        if(k == pcb->numOfProcesses){
            k = 0;
        }
    }
    waitTime += 1;
    printf("\n");
    double avgWaitTime = (double) waitTime/pcb->numOfProcesses;
    double avgTurnaround = (double) turnaround/pcb->numOfProcesses;
    double throughput = (double) pcb->numOfProcesses/totalJobtime;
    printf("Average Waiting Time = %-5.2f %2s (%d/%d)\n", avgWaitTime, "ms", waitTime, pcb->numOfProcesses); //avg wait, (totalwait, #processes)
    printf("Average Turnaround Time = %-5.2f %2s (%d/%d)\n", avgTurnaround, "ms", turnaround, pcb->numOfProcesses); //avg turnaround, turnaround, #processes
    printf("Throughput = %3.2f %-11s (%d/%d)\n", throughput, "jobs per ms", pcb->numOfProcesses, totalJobtime);//throughput, #processes, totaljobtime
}

void main(int argc, char *argv[]){
    if(argc < 5){
        printf("Not enough arguments\n");
        return;
    }
    char *sAlg;    //get algorithm as a string
    int iAlg = 0; //for switch statement
    int quantum;
    int i = 1;
    char *fileName;
    sAlg = (char *) malloc(4*sizeof(char));
    fileName = (char *) malloc(40*sizeof(char));

    while(i < argc){
        if(strcmp(argv[i], "-alg") == 0){
            strcpy(sAlg, argv[i + 1]); //save algorithm as a string
        }
        if(strcmp(argv[i], "-quantum") == 0){
            quantum = atoi(argv[i + 1]);
        }
        if(strcmp(argv[i], "-input") == 0){
            strcpy(fileName, argv[i + 1]);
        }
        i++;
    }

    if(strcmp(sAlg, "FIFO") == 0){
        iAlg = 1;
    }
    if(strcmp(sAlg, "SJF") == 0){
        iAlg = 2;
    }
    if(strcmp(sAlg, "PR") == 0){
        iAlg = 3;
    }
    if(strcmp(sAlg, "RR") == 0){
        iAlg = 4;
    }

    printf("Student Name: Adolfo Montero\n");
    printf("Input File Name: %s\n", fileName);
    printf("CPU Scheduling Alg: %s\n\n", sAlg);

    PCB pcb;
    readFile(fileName, &pcb);

    switch(iAlg){
        case 1:
            //FIFO
            fifo(&pcb);
            return;
        case 2:
            //SJF
            sjf(&pcb);
            return;
        case 3:
            //PR
            pr(&pcb);
            return;
        case 4:
            //RR
            rr(&pcb, quantum);
            return;
    }
    free(sAlg);
    free(fileName);
    return;
}