#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "fifo.h"	/* for taboo list */
#include "ramsey.h"	 /*local defined functions*/

#define MAXSIZE (512)

#define TABOOSIZE (500)
#define BIGCOUNT (9999999)

/***
 *** starts with a small randomized graph and works its way up to successively
 *** larger graphs one at a time
 ***
 *** uses a taboo list of size #TABOOSIZE# to hold and encoding of and edge
 *** (i,j)+clique_count
 ***
 *** This verion abandons the way to store graph as a matrix. Instead, it stores
 *** a graph as a int array with length gsize*(gsize-1)/2.
 ***
 ***/


int
main(int argc,char *argv[])
{
	int *g;
	int *new_g;
	int *new_edges;
	int seed = 1;
	int gsize;
	int listsize;
	int count;
	int i;
	int j;
	int k;
	int best_count;
	int best_i;
	int best_j;
	void *taboo_list;

	/*
	 * start with graph of size 15
	 */
	gsize = 14;
	listsize = gsize*(gsize-1)/2;
	g = (int *)malloc(listsize*sizeof(int));
	if(g == NULL) {
		exit(1);
	}

	/*
	 * make a fifo to use as the taboo list
	 */
	taboo_list = FIFOInitEdge(TABOOSIZE);
	if(taboo_list == NULL) {
		exit(1);
	}

	/*
	 * start out with half 1 and half 0.
	 */
	InitGraph(g, gsize, seed);

	/*
	 * while we do not have a publishable result
	 */
	while(gsize < 102)
	{
		/*
		 * find out how we are doing
		 */
		count = CliqueCount(g,gsize);

		/*
		 * if we have a counter example
		 */
		if(count == 0)
		{
			printf("Eureka!  Counter-example found!\n");
			PrintGraph(g,gsize);
			OutputGraph(g, gsize, seed);
			/*
			 * make a new graph one size bigger
			 */
			listsize = (gsize+1)*gsize/2;
			new_g = (int *)malloc(listsize*sizeof(int));
			if(new_g == NULL)
				exit(1);
			/*
			 * copy the old graph into the new graph leaving the
			 * last row and last column alone
			 */
			GetNewGraph(g,gsize,new_g,gsize+1, seed);

			/*
			 * throw away the old graph and make new one the
			 * graph
			 */
			free(g);
			g = new_g;
			gsize = gsize+1;

			/*
			 * reset the taboo list for the new graph
			 */
			taboo_list = FIFOResetEdge(taboo_list);

			/*
			 * keep going
			 */
			continue;
		}

		/*
		 * otherwise, we need to consider flipping an edge
		 *
		 * let's speculative flip each edge, record the new count,
		 * and unflip the edge.  We'll then remember the best flip and
		 * keep it next time around
		 *
		 * only need to work with upper triangle of matrix =>
		 * notice the indices
		 */
		best_count = BIGCOUNT;
		for(i=0; i < listsize-1; i++)
		{
			for(j=i+1; j < listsize; j++)
			{
				/*
				 * if they are two different edges, flip it
				 */
				if(g[i] != g[j])
				{
					g[i] = 1 - g[i];
					g[j] = 1 - g[j];

					//printf("The best count is %d, and the count is %d.", best_count, count);
					count = CliqueCount_EA(g,gsize);
					//count = CliqueCount(g, gsize);
					/*
					* is it better and the i,j,count not taboo?
					*/
					if((count < best_count) && 
//						!FIFOFindEdge(taboo_list,i,j))
						!FIFOFindEdgeCount(taboo_list,i,j,count))
					{
						best_count = count;
						best_i = i;
						best_j = j;
					}

					/*
					* flip it back
					*/
					g[i] = 1 - g[i];
					g[j] = 1 - g[j];
				}
			}
		}

		if(best_count == BIGCOUNT) {
			printf("no best edge found, terminating\n");
			exit(1);
		}
		
		/*
		 * keep the best flip we saw
		 */
		g[best_i] = 1 - g[best_i];
		g[best_j] = 1 - g[best_j];

		/*
		 * taboo this graph configuration so that we don't visit
		 * it again
		 */
		count = CliqueCount(g,gsize);
//		FIFOInsertEdge(taboo_list,best_i,best_j);
		FIFOInsertEdgeCount(taboo_list,best_i,best_j,count);

		printf("ce size: %d, best_count: %d, best edge pair: (%d,%d)\n",
			gsize,
			best_count,
			best_i,
			best_j);

		/*
		 * rinse and repeat
		 */
	}

	FIFODeleteGraph(taboo_list);


	return(0);

}
