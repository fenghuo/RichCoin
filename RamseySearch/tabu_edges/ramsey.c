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
 * fast inline function to calculate the index of the edge between
 * node i and node j in an int array.
 */
//static inline int
//CINDEX(int i, int j, int gsize)
//{
//	return i*gsize+j-(i+1)*(i+2)/2;
//}


/*
 * print the right format of the read routine.
 */
void
PrintGraph(int *g, int gsize)
{
	int i;
	int j;

	fprintf(stdout, "%d\n", gsize);

	for(i = 0; i < gsize; i ++)
	{
		for(j = 0; j < gsize; j ++)
		{
			if(j > i)
			{
				fprintf(stdout, "%d ", g[CINDEX(i,j,gsize)]);
			}
			else
			{
				fprintf(stdout, "0 ");
			}
		}
		fprintf(stdout, "\n");
	}

	return;
}


/*
 * generate new graph from old graph. add half-half edges.
 */
void
GetNewGraph(int *old_g, int o_gsize, int *new_g, int n_gsize, int seed)
{
	int i;
	int j;
	int o_count;
	int n_count;
	int k;
	int *n_edges;

	if(n_gsize < o_gsize)
		return;

	/*
	 * get a new random edge list.
	 */
	n_edges = (int *)malloc(o_gsize*sizeof(int));
	if(n_edges == NULL)
	{
		exit(1);
	}

	InitEdges(n_edges, o_gsize, seed);

	k = 0;
	o_count = 0;
	n_count = 0;
	for(i = o_gsize-1; i >= 1; i --)
	{
		for(j = 0; j < i; j ++)
		{
			new_g[n_count++] = old_g[o_count++];
		}
		new_g[n_count++] = n_edges[k++];
	}
	new_g[n_count] = n_edges[k];

	free(n_edges);

	return;
}


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
	int i;
	int listsize = gsize*(gsize-1)/2;
	int n_one;
	int n_zero;

	if(gsize%4 != 1)
	{
		n_one = (int) listsize/2;
		n_zero = listsize - n_one;
	}
	else
	{
		n_zero = (int) listsize/2;
		n_one = listsize - n_zero;
	}

	srand(seed);

	for(i = 0; i < n_one; i ++)
	{
		g[i] = 1;
	}
	memset(&g[n_one], 0, n_zero*sizeof(int));

	shuffle(g, listsize);

	return;

}

/*
 * initialize half-half edges.
 */
void
InitEdges(int *edges, int gsize, int seed)
{
	int n_one;
	int n_zero;
	int i;

	if(gsize%4 != 1)
	{
		n_one = (int) gsize/2;
		n_zero = gsize - n_one;
	}
	else
	{
		n_zero = (int) gsize/2;
		n_one = gsize - n_zero;
	}

	for(i = 0; i < n_one; i ++)
	{
		edges[i] = 1;
	}
	memset(&edges[n_one], 0, n_zero*sizeof(int));

	shuffle(edges, gsize);
	return;
}


/*
 * returns the number of monochromatic cliques in the graph.
 */

int 
CliqueCount(int *g, int gsize)
{
    int i;
    int j;
    int k;
    int l;
    int m;
    int n;
    int count=0;
    int sgsize = 6;
    
    for(i=0;i < gsize-sgsize+1; i++)
    {
	for(j=i+1;j < gsize-sgsize+2; j++)
        {
	    for(k=j+1;k < gsize-sgsize+3; k++) 
        { 
		if((g[CINDEX(i,j,gsize)] == g[CINDEX(i,k,gsize)]) && 
		   (g[CINDEX(i,j,gsize)] == g[CINDEX(j,k,gsize)]))
		{
		    for(l=k+1;l < gsize-sgsize+4; l++) 
		    { 
			if((g[CINDEX(i,j,gsize)] == g[CINDEX(i,l,gsize)]) && 
			   (g[CINDEX(i,j,gsize)] == g[CINDEX(j,l,gsize)]) && 
			   (g[CINDEX(i,j,gsize)] == g[CINDEX(k,l,gsize)]))
			{
			    for(m=l+1;m < gsize-sgsize+5; m++) 
			    {
				if((g[CINDEX(i,j,gsize)] == g[CINDEX(i,m,gsize)]) && 
				   (g[CINDEX(i,j,gsize)] == g[CINDEX(j,m,gsize)]) &&
				   (g[CINDEX(i,j,gsize)] == g[CINDEX(k,m,gsize)]) && 
				   (g[CINDEX(i,j,gsize)] == g[CINDEX(l,m,gsize)]))
				{
					for(n=m+1; n < gsize-sgsize+6; n++)
					{
					if((g[CINDEX(i,j,gsize)] == g[CINDEX(i,n,gsize)]) &&
					   (g[CINDEX(i,j,gsize)] == g[CINDEX(j,n,gsize)]) &&
					   (g[CINDEX(i,j,gsize)] == g[CINDEX(k,n,gsize)]) &&
					   (g[CINDEX(i,j,gsize)] == g[CINDEX(l,n,gsize)]) &&
					   (g[CINDEX(i,j,gsize)] == g[CINDEX(m,n,gsize)]))
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
        }
     }
    return(count);
}


/*
 * The early abandon version of CliqueCount.
 */

int 
CliqueCount_EA(int *g, int gsize)
{
    int i;
    int j;
    int k;
    int l;
    int m;
    int n;
    int count=0;
    int sgsize = 6;
    
    for(i=0;i < gsize-sgsize+1; i++)
    {
	for(j=i+1;j < gsize-sgsize+2; j++)
        {
	    for(k=j+1;k < gsize-sgsize+3; k++) 
        { 
		if((g[CINDEX(i,j,gsize)] == g[CINDEX(i,k,gsize)]) && 
		   (g[CINDEX(i,j,gsize)] == g[CINDEX(j,k,gsize)]))
		{
		    for(l=k+1;l < gsize-sgsize+4; l++) 
		    { 
			if((g[CINDEX(i,j,gsize)] == g[CINDEX(i,l,gsize)]) && 
			   (g[CINDEX(i,j,gsize)] == g[CINDEX(j,l,gsize)]) && 
			   (g[CINDEX(i,j,gsize)] == g[CINDEX(k,l,gsize)]))
			{
			    for(m=l+1;m < gsize-sgsize+5; m++) 
			    {
				if((g[CINDEX(i,j,gsize)] == g[CINDEX(i,m,gsize)]) && 
				   (g[CINDEX(i,j,gsize)] == g[CINDEX(j,m,gsize)]) &&
				   (g[CINDEX(i,j,gsize)] == g[CINDEX(k,m,gsize)]) && 
				   (g[CINDEX(i,j,gsize)] == g[CINDEX(l,m,gsize)]))
				{
					for(n=m+1; n < gsize-sgsize+6; n++)
					{
					if((g[CINDEX(i,j,gsize)] == g[CINDEX(i,n,gsize)]) &&
					   (g[CINDEX(i,j,gsize)] == g[CINDEX(j,n,gsize)]) &&
					   (g[CINDEX(i,j,gsize)] == g[CINDEX(k,n,gsize)]) &&
					   (g[CINDEX(i,j,gsize)] == g[CINDEX(l,n,gsize)]) &&
					   (g[CINDEX(i,j,gsize)] == g[CINDEX(m,n,gsize)]))
					{
			      			count++;
			      			if(count > 500) return(count);
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
	int j;
	char fn_output[50];
	FILE *fp;

	sprintf(fn_output, "results/Ramsey_%d_%d.txt", gsize, seed);
	puts(fn_output);
	fp = fopen(fn_output, "w");

	//fprintf(stdout, "file opened!");
	for(i = 0; i < gsize; i ++)
	{
		for(j = 0; j < gsize; j ++)
		{
			if(j > i)
			{
				fprintf(fp, "%d", g[CINDEX(i,j,gsize)]);
			}
			else
			{
				fprintf(fp, "0");
			}
		}
	}
	fprintf(fp, "\n");
	//printf("Ready to close");
	fclose(fp);

}
