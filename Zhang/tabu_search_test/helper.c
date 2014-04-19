#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "helper.h"

/**
 ** local functions defined by Yuanyang.
 **/

void
shuffle(int *randList, int n)
{
	int i, j;
	int t;

	for (i = 0; i < n - 1; i ++)
	{
		j = i + rand() / (RAND_MAX / (n - i) + 1);
		t = randList[j];
		randList[j] = randList[i];
		randList[i] = t;
	}

	return;

}


void
InitG(int *g, int gsize, int seed)
{
	int listsize = gsize*(gsize-1)/2;
	int randList[listsize];
	int i, j, k, iter;
	int count;
	int n_one = (int) listsize / 2;
	int n_zero = listsize-n_one;

	srand(seed);

	for(i = 0; i < n_one; i ++)
	{
		randList[i] = 1;
	}
	memset(&randList[n_one], 0, n_zero*sizeof(int));

	shuffle(randList, listsize);

	k = 0;
	for(i = 0; i < gsize; i ++)
	{
		for(j = 0; j < gsize; j ++)
		{
			if( j <= i )
			{
				g[i*gsize+j] = 0;
			}
			else
			{
				g[i*gsize+j] = randList[k];
				k ++;
			}
		}
	}

	return;
	
}

