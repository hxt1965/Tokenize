//insert needed functions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct transition
{
    int gotoState;
    char result;
};

int main( int argc, char* argv[])
{
    FILE *fp;
    char *ptr, buf[256], *token;
    int nOfStates, startState, acceptState;
    struct transition **matrix;
    if(argc!=2) {
        printf("Usage: ./tokenize tmfile\n");
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
                    matrix = (struct trasition**) malloc(nOfStates *
                            sizeof(struct transition**));
                    for(int i=0; i<nOfStates; i++)
                        matrix[i] = (struct transition*) malloc(12 *
                                sizeof(struct transition));
                    
                    for(int i=0; i<nOfStates; i++)
                        for (int j=0; j<12; j++)
                        {
                            matrix[i][j].gotoState = 99;
                            matrix[i][j].result = 'd';
                        }
                }
                if(cnt==1)
                    startState = atoi(token);
                if(cnt==2)
                    acceptState = atoi(token);
            }
            else {
                
                token = strtok(buf, " ");
                int state = atoi(token), cState;
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
 	printf("Scanning using the following matrix:\n");
    printf(" \t0\t1\t2\t3\t4\t5\t6\t7\t8\t9\t10\t11\n");
    for(int i=0; i<nOfStates; i++)
    {
        printf("%d", i);
        for(int j = 0; j<12; j++)
        {
            printf("\t%d%c", matrix[i][j].gotoState,
                    matrix[i][j].result);
        }
        printf("\n");
    }
  return 0;
}               
