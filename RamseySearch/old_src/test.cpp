
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]) {

	int i;
	int j;
	int k;
	int gsize = 10;
	int n_list = gsize*(gsize-1)/2;
	int glist[n_list];
	int gmatrix[gsize*gsize];

	for(i = 0; i < n_list; i ++)
	{
		glist[i] = i;
		cout << i << " ";
	}
	cout << "\n" << endl;

	k = 0;
	for(i = 0; i < gsize; i ++)
	{
		for(j = 0; j < gsize; j ++)
		{
			if(j > i)
			{
				gmatrix[i*gsize+j] = glist[k];
				k ++;
			}
			else
			{
				gmatrix[i*gsize+j] = 0;
			}
			cout << i << "," << j << " : " << gmatrix[i*gsize+j] << " ";
		}
		cout << "\n";
	}

	return 0;
}
