#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "ramsey.h"

/**
 ** local functions defined by Yuanyang.
 **/


/*
 * randomly shuffle a list.
 */
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


/*
 * Initialize a graph with half 1 and half 0.
 */
void
InitGraph(int *g, int gsize, int seed)
{
	int listsize = gsize*(gsize-1)/2;
	int *randList;
	int i, j, k, iter;
	int count;
	int n_one = (int) listsize / 2;
	int n_zero = listsize-n_one;

	srand(seed);

	randList = (int *)malloc(listsize*sizeof(int));
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

	free(randList);
	return;

}

/*
 * initialize half-half edges.
 */
void
InitEdges(int *edges, int gsize, int seed)
{
	int n_one = (int) gsize/2;
	int n_zero = gsize - n_one;
	int i;

	for(i = 0; i < n_one; i ++)
	{
		edges[i] = 1;
	}
	memset(&edges[n_one], 0, n_zero*sizeof(int));

	shuffle(edges, gsize);
	return;
}


/*
 * CliqueCount early abandon version.
 */
int
CliqueCount_EA(int *g, int gsize, int best_count)
{
	int i;
	int j;
	int k;
	int l;
	int m;
	int n;
	int count = 0;
	int sgsize = 6;

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
				   (g[i*gsize+j] == g[l*gsize+m])) {
					for(n=m+1; n < gsize-sgsize+6; n++)
					{
						if((g[i*gsize+j]
							== g[i*gsize+n]) &&
						   (g[i*gsize+j]
							== g[j*gsize+n]) &&
						   (g[i*gsize+j]
							== g[k*gsize+n]) &&
						   (g[i*gsize+j]
							== g[l*gsize+n]) &&
						   (g[i*gsize+j]
							== g[m*gsize+n])) {
			      					count++;
			      					if(count > 500) return(count);
							//if(best_count >= 1000 & count > 1500) return(count);
							//if(best_count >= 100 & count > 150) return(count);
						}
					}
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


/*
 * print out the graph.
 */
void
OutputGraph(int *g, int gsize, int seed)
{
	int i;
	int g_length = gsize*gsize;
	char fn_output[50];
	FILE *fp;

	sprintf(fn_output, "results/Ramsey_%d_%d.txt", gsize, seed);
	fp = fopen(fn_output, "w");

	for(i = 0; i < g_length; i ++)
	{
		fprintf(fp, "%d", g[i]);
	}
	fprintf(fp, "\n");
	fclose(fp);

}
