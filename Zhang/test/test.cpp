#include <iostream>
#include <fstream>
using namespace std;

#define MatSize 10

int CliqueCount(int *g, int gsize);
void CountZeroOne(int *g, int gsize, int *countZero, int *countOne);

int main() {

    int matGraph[MatSize*MatSize];
    int i, j;
    int count;
    int countZero=0, countOne=0;
    ifstream inFile;

    inFile.open("mat10.txt", ios::in);
    if(! inFile) {
        cerr << "unable to open file mat10.txt for reading" << endl;
        return 1;
    }
        
    for(i = 0; i < MatSize*MatSize; i++) {
        inFile >> matGraph[i];
        cout << matGraph[i];
    }

    inFile.close();

    count = CliqueCount(matGraph, MatSize);
    cout << count << endl;

    CountZeroOne(matGraph, MatSize, &countZero, &countOne);
    cout << countZero << ", " << countOne << endl;

    return 0;
}


int CliqueCount(int *g, int gsize) {
    int i;
    int j;
    int k;
    int l;
    int m;
    int count = 0;
    int sgsize = 5;	/* for R(5,5) */

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


void CountZeroOne(int *g, int gsize, int *countZero, int *countOne) {
    int i;
    int j;
    int sgsize = MatSize/2;

    for(i = 0; i < gsize-sgsize+4; i++) {
        for(j = i+1; j < gsize-sgsize+5; j++) {
           if(g[i*gsize+j] == 0) {
               (*countZero) ++;
           } else {
               (*countOne) ++;
           }
        }
    }
    cout << (*countZero) << ", " << (*countOne) << endl;
}
