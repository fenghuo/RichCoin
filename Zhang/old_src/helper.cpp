#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include "helper.h"

using namespace std;

int CliqueCount(int *g, int gsize) {
    int i;
    int j;
    int k;
    int l;
    int m;
    int count = 0;
    int sgsize = 5;     /* for R(5,5) */

    for(i=0;i < gsize-sgsize+1; i++)
    {   
        for(j=i+1;j < gsize-sgsize+2; j++)
        {   
            for(k=j+1;k < gsize-sgsize+3; k++)
            {   
                if((g[i*gsize+j] == g[i*gsize+k]) &&
                   (g[i*gsize+j] == g[j*gsize+k]))
                {   
                    for(l=k+1;l < gsize-sgsize+4; l++)
                    {   
                        if((g[i*gsize+j] == g[i*gsize+l]) &&
                           (g[i*gsize+j] == g[j*gsize+l]) &&
                           (g[i*gsize+j] == g[k*gsize+l]))
                        {   
                            for(m=l+1;m < gsize-sgsize+5; m++)
                            {   
                                if((g[i*gsize+j] == g[i*gsize+m]) &&
                                   (g[i*gsize+j] == g[j*gsize+m]) &&
                                   (g[i*gsize+j] == g[k*gsize+m]) &&
                                   (g[i*gsize+j] == g[l*gsize+m]))
                                {
                                     count++;
                                }
                            }
                        }
                     }
                }
             }
        }
     }

     return(count);
}


int InitG(int halfone, int *randG) {
    int randList[LTSIZE];
    int ntimes = 10000;
    int i, j, k, iter;
    int count;

    for(iter = 0; iter < ntimes; iter ++) {
        
        for(i = 0; i < halfone; i ++) {
            randList[i] = 1;
        }
        for(i = halfone; i < LTSIZE; i ++) {
            randList[i] = 0;
        }

        random_shuffle(&randList[0], &randList[LTSIZE]);

        k = 0;
        for(i = 0; i < GSIZE; i ++) {
            for(j = 0; j < GSIZE; j ++) {
                if(j <= i) {
                    randG[i*GSIZE+j] = 0;
                } else {
                    randG[i*GSIZE+j] = randList[k];
                    k ++;
                }
            }
        }

        count = CliqueCount(randG, GSIZE);
        
        if(count <= 1000) {
            return(iter);
        }
    }
    
    cerr << "More iterations needed to initialize graph." << endl;
    return EXIT_FAILURE;
}


int PrintG(int *randG, char *filename) {
    int i;
    ofstream matG;

    matG.open(filename);
    for(i = 0; i < MTSIZE; i ++) {
        matG << randG[i];
    }

    matG.close();
   
    return 1;
}


void SwapAll(int *randG, int *besti, int *bestj) {
    int i, j;
    int count;
    int bestCount;

    bestCount = 1000;

    for(i = 0; i < GSIZE; i ++) {
        for(j = i+1; j < GSIZE; j ++) {
    
            Swap(randG, i, j);

            count = CliqueCount(randG, GSIZE);
            if(count < bestCount) {
                bestCount = count;
                (*besti) = i;
                (*bestj) = j;
            }

            Swap(randG, i, j);
        }
    }
}
