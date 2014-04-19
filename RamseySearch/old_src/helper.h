#ifndef _HELPER_H_
#define _HELPER_H_

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define GSIZE 42
#define MTSIZE 1764 // matrix size
#define LTSIZE 861 // list size

inline void Swap(int *randG, int nodei, int nodej) {
    randG[nodei*GSIZE+nodej] = 1-randG[nodei*GSIZE+nodej];
}

int CliqueCount(int *g, int size);
int InitG(int halfone, int *randG);
int PrintG(int *randG, char *filename);
void SwapAll(int *randG, int *besti, int *bestj);

#endif
