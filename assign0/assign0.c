//Adolfo Montero

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "myio.h"

int iMAX;
double dMAX;
char LONGEST[1000];
int longestString = 0;


void IntMax(int *iArr, int count){
    int i = 0;
    int max = iArr[i];
    while( i < count){
        if(iArr[i] > max){
            max = iArr[i];
        }
        i++;
    }
    //Save Results
    iMAX = max;
    return;
}

void DblMax(double dArr[], int count){
    int i = 0;
    double max = dArr[i];
    while( i < count){
        if(dArr[i] > max){
            max = dArr[i];
        }
        i++;
    }
    //Save Results
    dMAX = max;
    return;
}

/*
removes whitespace from an input file and writes it to an output file
will leave 1 space between words
*/
void rmWhiteSpace(char *input, char *output){
    //Open files and check for their existence (make output file if it doesn't exist)
    FILE* filein = fopen(input, "r");
    FILE* fileOut = fopen(output, "wb");

    if(filein == NULL){
        errExit("Input file doesn't exist: %s", strerror(errno));
    }
    if(fileOut == NULL){
        errExit("Error making output file: %s", strerror(errno));
    }

    char readLine[256];
    char buffer[256];
    while(fgets(readLine, sizeof(readLine), filein)){
        int i = 0;
        int j = 0;
        for(i = 0; readLine[i] != '\0'; i++){
            //if not a space
            if(readLine[i] != ' ' && readLine[i] != EOF){
                buffer[j++] = readLine[i];
            }
            //if a space and next is not a space
            if(readLine[i] == ' ' && readLine[i+1] != ' '){
                buffer[j++] = readLine[i];
            }
            buffer[j] = '\0';
        }
        //Print buffer to stdout
        //printf("%s\n", buffer);

        //save edited string to file
        fprintf(fileOut, "%s", buffer);
    }
    
    //close files
    fclose(filein);
    fclose(fileOut);

    return;
}

void main(int argc, char *argv[]){
    if(argc == 3){
        //run file code
        rmWhiteSpace(argv[1], argv[2]);
        exit(0);
    }

    //get 3 or 4 command line arguments
    if(argc !=  4){
        printf("NEEDS 2 ARGUMENTS FOR FILE STRIPPING OR 3 ARGUMENTS FOR MAX PROGRAM\n");
        return;
    }

    //Read how many ints, doubles, and lines we need to process
    int iCount = atoi(argv[1]);
    int dCount = atoi(argv[2]);
    int sCount = atoi(argv[3]);
    
    //Show Arguments received
    //printf("Input Arguments:\nInt: %d\nDouble: %d\nLines: %d\n", iCount, dCount, sCount);
    
    int count = 0;
    //Integer
    int iArr[100];
    printf("Enter %d integers\n", iCount);
    while(count < iCount){
        int iNum = ReadInteger(); //read int
        iArr[count] = iNum;
        count++;
    }
    //find and store max integer
    IntMax(iArr, count);
    printf("\n");

    //Reset Count
    count = 0;

    //Double
    double dArr[100];
    printf("Enter %d doubles\n", dCount);
    while(count < dCount){
        double dNum = ReadDouble(); //read double
        dArr[count] = dNum;
        count++;
    }
    //find and store max double
    DblMax(dArr, count);

    printf("\nEnter %d lines\n", sCount);

    //Reset Count
    count = 0;
    //Line
    while(count < sCount){
        if(count == 0){
            //throwaway line
            char *badLine = ReadLine();
        }
        char *line = ReadLine();
        count++;
    }
    printf("\n");

    //Output results
    printf("Max Int: %d\nMax Dbl: %lf\nMax Line: %s\n", iMAX, dMAX, LONGEST);

    return;
}