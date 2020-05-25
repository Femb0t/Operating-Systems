#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int wordCount(char *input){
    //Open files and check for their existence (make output file if it doesn't exist)
    FILE* filein = fopen(input, "r");
    int wordcount = 0;

    if(filein == NULL){
        errExit("Input file doesn't exist: %s", strerror(errno));
    }

    char readLine[256];
    while(fgets(readLine, sizeof(readLine), filein)){
        int i = 0;
        for(i = 0; readLine[i] != '\0'; i++){
            //if a char and next is a space, tab, or EOF
            if(readLine[i] != ' ' && (readLine[i + 1] == ' ' || readLine[i + 1] == '\t' || readLine[i + 1] == EOF)){
                wordcount++;
            }
            if(readLine[i] == '\n' && readLine[i - 1] != ' '){
                wordcount++;
            }
        }
    }
    wordcount++;
    //close file
    fclose(filein);
    return wordcount;
}

void main(int argc, char *argv[]){
    int pid;
    int wordcount = 0;

    //Must have at least 1 file name passed
    if(argc < 2){
        //not enough inputs
        printf("Not enough arguments\n");
        return;
    }
    int paramNum = argc - 1; //number of files to read
    int i = 1;

    //loop through the files and create a fork for each
    for(i = 1; i <= paramNum; i++){
        pid = fork();
        if(pid == 0){//child
            wordcount = wordCount(argv[i]); //send first file
            printf("Child process for %s: number of words is %d\n", argv[i], wordcount);
            exit(0);
        }
        else {//parent
            waitpid(-1, NULL, 0);
        }
    }
    printf("All %d files have been counted!\n", paramNum);
    return;
}