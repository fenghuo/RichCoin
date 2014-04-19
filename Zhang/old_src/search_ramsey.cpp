/******************************************************************************
* search_ramsey.cpp                                                           *
*                                                                             *
* Description: This file contains the C source code for searching 5-th ramsey *
*              number.                                                        *
*                                                                             *
* Author: Yuanyang Zhang <zhangyy1209@gmail.com>,                             *
*         Li Zheng <hellobuaazl@gmail.com>.                                   *
*                                                                             *
*******************************************************************************/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#include "helper.h"

using namespace std;

int main(int argc, char* argv[]) {

    if(argc < 2) {
        cerr << "usage: ./search_Ramsey <random seed>\n";
        return EXIT_FAILURE;
    }

    int seed = atoi(argv[1]);
    int randG[MTSIZE]; // 42*42
    int iter;
    int count;
    int halfone;
    int i, j;
    char result[20];
    ofstream findG;

    srand(seed);
    
    halfone = (int) LTSIZE/2;

    InitG(halfone, randG);

    iter = 0;
    count = CliqueCount(randG, GSIZE);
    cout << count << endl;    

    i = 0; 
    j = 0;
  
    while(count != 0) {
         SwapAll(randG, &i, &j);
         cout << i << ", " << j << endl;
         Swap(randG, i, j);
         count = CliqueCount(randG, GSIZE);
         cout << iter << ", " << count << endl;
         iter ++;
    }

    if(count <= 20) {
        sprintf(result, "newG_%d.txt", count);
        PrintG(randG, result);
    }

    cout << iter << ", " << count << "\n";
    
    return 0;
}
