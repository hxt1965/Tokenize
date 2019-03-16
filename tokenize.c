/**
 * name of file: tokenize.c
 * author: Harsh Tagotra
 * date: 10/12/2018
 *
 * this program reads the transition data provided by an input
 * file and scans user provided input to check legitimacy of
 * tokens according to syntax provided by the matrix
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "classes.h"

/*structure defining each element of the transition matrix
 */
struct transition
{
    int gotoState;
    char result;
};

struct transition **matrix;

//declaration of function because it is created after the main
void scantok(int, int);

/**
 * writes the default values for all transition states
 */
void writeMatrix(int nOfStates, int n, 
        struct transition **matrix) {
    for(int i=0; i<nOfStates; i++)
          for (int j=0; j<n; j++)
    {
        matrix[i][j].gotoState = 99;
        matrix[i][j].result = 'd';
    }
}

/**
 * the main opens the file, checks for errors and reads the entire
 * transition matrix. It then displays it and goes on to 
 * accept an input from the user. In a while loop, 
 * it recognizes each token.
 */
int main( int argc, char* argv[])
{
    FILE *fp;
    char *ptr, buf[256], *token;
    int nOfStates, startState, acceptState;

    if(argc!=2) {
        fprintf(stderr, "usage: ./tokenize tmfile\n");
        return 0;
    }
    fp = fopen(argv[1], "r");
    if(fp == NULL) {
        perror( argv[1] );
        exit( EXIT_FAILURE );
    } else {
        int cnt=0;
        while( (ptr = fgets(buf, 256, fp)) != NULL) {
            if(cnt<=2)
            {
                token = strtok(buf, " ");
                token = strtok(NULL, " ");
                if(cnt==0) {
                    nOfStates = atoi(token);
                    //memory allocation for 2d array of structs
                    matrix =  malloc(nOfStates *
                            sizeof(struct transition**));
                    for(int i=0; i<nOfStates; i++)
                       matrix[i] = (struct transition*) malloc(12 *
                                sizeof(struct transition));
                    
                    //initializing each element with default value 
                    //of 99d
                    writeMatrix(nOfStates, 12, matrix); 
                }

                //conversion of first two lines
                if(cnt==1)
                    startState = atoi(token);
                if(cnt==2)
                    acceptState = atoi(token);
            }
            //the rest of the file containing transition 
            //information is read
            else {
                token = strtok(buf, " ");
                int state = atoi(token);
                
                token = strtok(NULL, " ");
                while(token!=NULL)
                {
                    int t=0, g=0;
                    char r[1];
                    memset(r, 0, sizeof(r));
                    sscanf(token, "%d/%d%s", &t, &g, r);

                    matrix[state][t].gotoState = g;
                    matrix[state][t].result = r[0];

                    token = strtok(NULL, "  ");
                }
            }
            cnt++;
        }
    }
    fclose(fp);

    //printing the entire matrix
    printf("Scanning using the following matrix:\n");
    printf("     0    1    2    3    4    5    6    7    8    9   10   11\n");
    for(int i=0; i<nOfStates; i++)
    {
        printf(" %d  ", i);
        for(int j = 0; j<12; j++)
        {
            printf("%2d%c", matrix[i][j].gotoState,
                   matrix[i][j].result);
            if(j!=11)
                printf("  ");
        }
        printf("\n");
    }

    scantok(startState, acceptState);

    return EXIT_SUCCESS;
}

/*
 *  returns the class of the letter through the ascii 
 *  value provided
 *  @params int ascii: ascii value of letter
 */
int getClass(int ascii) 
{
    if(ascii==EOF)
        return CC_EOF;
    if(ascii==32)
        return CC_WS;
    if(ascii == 10)
        return CC_NEWLINE;
    if( (ascii>=65 && ascii<=90) ||
            (ascii>=97 && ascii <=122) || ascii ==95)
        return CC_ALPHA;
    if(ascii==48)
        return CC_DIG_0;
    if(ascii>=49 && ascii<=55)
        return CC_DIG_1_7;
    if(ascii==56 || ascii==57)
        return CC_DIG_8_9;
    if(ascii==47)
        return CC_CHAR_SLASH;
    if(ascii==42)
        return CC_CHAR_STAR;
    if(ascii==37 || ascii==43 || ascii==45)
        return CC_ARITH_OP;
    
    return CC_OTHER;
}

/**
 * using getchar(), this function reads every character and 
 * finds what the transition states will be for each character
 * as it parses through the entire stream. Flags in the middle are used
 * to keeo track of errors and outputs are dispayed according to
 * recognized and rejected tokens.
 *
 * @param startState: the start state provided by tm file
 * @param acceptState: the accept state provided by the tm file
 */
void scantok(int startState, int acceptState) {
    char tok[64] = {"\0"};
    char letter;
    int ascii, currState=startState, nextState, rFlag=1;
    while( (letter = getchar()) )
    {
        //rFlag is defined to keep consistency on encountering errors
        if(rFlag==1)
            printf("%d ", currState);
        
        ascii = (int) letter;
        nextState = getClass(ascii);

        //transition found using the matrix
        if(rFlag==1)
            currState = matrix[currState][nextState].gotoState;

        if(letter==EOF) {
            printf("%d EOF\n", currState);
            break;
        }

        //checking for error state
        if(currState==99)
            rFlag = 0;

        //addition of chars to the buffer
        if(rFlag==1 && matrix[currState][nextState].result=='s') {
            char end[2] = {"\0"};
            end[0] = letter;
            strcat(tok, end);
        }

        if(rFlag==0 && letter==' ') {
            memset(tok, 0, 64);
            printf("%d rejected\n", currState);
            currState = startState;
            rFlag=1;
        }
        if(currState == acceptState && rFlag==1)
        {
            printf("%d recognized '%s'\n", currState, tok);
            currState=startState;
            memset(tok, 0, 64);    
        }
    }
}
