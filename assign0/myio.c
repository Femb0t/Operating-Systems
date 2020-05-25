#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "myio.h"

extern int longestString;
extern char LONGEST[1000];

void errExit(const char szFmt[], ...);

/*
finds the max line length and stores the string in a global variable
*/
void LineMax(char *line){
    int i = 0;
    int size = strlen(line);
    if(size > longestString){
        longestString = size; //store the line size as a global int just in case
        strcpy(LONGEST, line); //store the line into the global variable
    }
    return;
}

/* 
 * Function: ReadLine
 * Usage: s = ReadLine();
 * ---------------------
 * ReadLine reads a line of text from standard input and returns
 * the line as a string.  The newline character that terminates
 * the input is not stored as part of the string.
 */
char *ReadLine(void){
    char line[1000];
    fgets(line, 1000, stdin);
    LineMax(line);
    char *rLine;
    strcpy(rLine, line);
    return rLine;
}

/* 
 * Function: ReadLineFile
 * Usage: s = ReadLineFile(infile);
 * ----------------------------
 * ReadLineFile reads a line of text from the input file which 
 * is already open and pointed by infile. It then reads the line, 
 * dynamically allocates space, and returns the line as a string. 
 * The newline character that terminates the input is not stored 
 * as part of the string.  
 * The ReadLineFile function returns NULL if infile is at the 
 * end-of-file position. 
 * Note: the above ReadLine(); can simply be implemented as 
 *  char *ReadLine() { return(ReadLineFile(stdin)); } 
 */
//char *ReadLineFile(FILE *infile);

/*
 * Function: ReadDouble
 * Usage: x = ReadDouble();
 * ---------------------
 * ReadDouble reads a line of text from standard input and scans
 * it as a double.  As described above, you can call ReadLine() to
 * read the line. If the number cannot be scanned as double or if
 * extra characters follow after the number ends, this function 
 * gives an error and keep asking user to enter a valid double. 
 * The followings are valid doubless: 24, -35, +43, -23.54, +45.3 
 * The followings are not valid doubles: 34  5, 3afg, 3.3.3, --45.5   
 */
double ReadDouble(void){
    double dNum;
    int result = scanf("%lf", &dNum);
    if(result == EOF){
        errExit("Not a double: %s", strerror(errno));
    }
    return dNum;
}

/*
 * Function: ReadInteger
 * Usage: i = ReadInteger();
 * ------------------------
 * ReadInteger reads a line of text from standard input and scans
 * it as an integer. To read the line, you can call the ReadLine()
 * function that is described below. If all the characters in the
 * given line are digits, then convert it to an integer and 
 * return it. The very first character could be the sign character
 * '-' or '+'. 
 * Otherwise, if the given string contains any other character, 
 * this function will give an error meassage and keep asking user
 * to enter a valid sequnece of characters that can be converted to
 * an integer. 
 * The followings are valid integers: 234, -345, +453.
 * The followings are not valid integers: 34  5, 3afg, 3-3, --45.   
 */
int ReadInteger(void){
    int iNum;
    int result = scanf("%d", &iNum);
    if(result == EOF){
        errExit("not an int: %s", strerror(errno));
    }
    return iNum;
}