#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "fifo.h"	/* for taboo list */


#define MAXSIZE (512)

#define TABOOSIZE (500)
#define BIGCOUNT (9999999)

#define IMAX (2000000000)

int *g;
int *new_g;
int gsize;
int count;
int i,j,x,y;
int best_count;
int best_i;
int best_j;
int best_x;
int best_y;
void *taboo_list;
int flag=1;
int lastcount=IMAX;


/***
 *** example of very simple search for R(6,6) counter examples
 ***
 *** starts with a small randomized graph and works its way up to successively
 *** larger graphs one at a time
 ***
 *** uses a taboo list of size #TABOOSIZE# to hold and encoding of and edge
 *** (i,j)+clique_count
 ***/

/*
 * PrintGraph
 *
 * prints in the right format for the read routine
 */
void PrintGraph(int *g, int gsize)
{
	int i;
	int j;

	fprintf(stdout,"%d\n",gsize);

	for(i=0; i < gsize; i++)
	{
		for(j=0; j < gsize; j++)
		{
			fprintf(stdout,"%d ",g[i*gsize+j]);
		}
		fprintf(stdout,"\n");
	}

	return;
}

void WriteGraph(int *g, int gsize)
{
	int i;
	int j;

	char str[50];
	sprintf(str, "result/%d.txt", gsize);

	FILE*f;
	f=fopen(str,"w");

	for(i=0; i < gsize; i++)
	{
		for(j=0; j < gsize; j++)
		{
			fprintf(f,"%d",g[i*gsize+j]);
		}
	}

	fclose(f);
	return;
}
void DumpGraph(int *g, int gsize)
{
	int i;
	int j;

	FILE*f;
	f=fopen("dump","w");

	fprintf(f,"%d\n",gsize);
	for(i=0; i < gsize; i++)
	{
		for(j=0; j < gsize; j++)
		{
			fprintf(f,"%d",g[i*gsize+j]);
		}
	}

	fclose(f);
	return;
}


void ReadDump()
{
	int i;
	int j;

	FILE*f;
	f=fopen("dump","r");

	fscanf(f,"%d\n",&gsize);


	g = (int *)malloc(gsize*gsize*sizeof(int));

	for(i=0; i < gsize; i++)
	{
		for(j=0; j < gsize; j++)
		{
			char t;
			fscanf(f,"%c",&t);
			g[i*gsize+j]=(t-'0')+0;
		}
	}

	fclose(f);
	return ;
}
void ReadGraph(int *g, int gsize)
{
	int i;
	int j;

	char str[50];
	sprintf(str, "result/%d.txt", gsize);

	FILE*f;
	f=fopen(str,"r");

	for(i=0; i < gsize; i++)
	{
		for(j=0; j < gsize; j++)
		{
			char t;
			fscanf(f,"%c",&t);
			g[i*gsize+j]=(t-'0')+0;
		}
	}

	fclose(f);
	return;
}

/*
 * CopyGraph 
 *
 * copys the contents of old_g to corresponding locations in new_g
 * leaving other locations in new_g alone
 * that is
 * 	new_g[i,j] = old_g[i,j]
 */
void CopyGraph(int *old_g, int o_gsize, int *new_g, int n_gsize)
{
	int i;
	int j;

	/*
	 * new g must be bigger
	 */
	if(n_gsize < o_gsize)
		return;

	for(i=0; i < o_gsize; i++)
	{
		for(j=0; j < o_gsize; j++)
		{
			new_g[i*n_gsize+j] = old_g[i*o_gsize+j];
		}
	}

	for(i=0;i<n_gsize;i++)
	{
		for(j=o_gsize;j<n_gsize;j++)
		{
			new_g[i*n_gsize+j] = rand()%2;
			new_g[j*n_gsize+i] = 0;
		}
	}

	return;
}


/*
 ***
 *** returns the number of monochromatic cliques in the graph presented to
 *** it
 ***
 *** graph is stored in row-major order
 *** only checks values above diagonal
 */

int CliqueCount(int *g,
	     int gsize)
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
int CliqueCount2(int *g,int gsize,int maxcount)
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
								if(count>maxcount)
									return IMAX;
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

void GoodCopy(int*o,int os,int*n,int ns)
{
	int i,count,c;
	int * ng = (int *)malloc((ns)*(ns)*sizeof(int));
	count=IMAX;
	for(i=0;i<ns/10;i++)
	{
		CopyGraph(o,os,ng,ns);
		c = CliqueCount(ng,ns);
		printf("%d\n",c);
		if(c<count)
		{
			memcpy(n,ng,ns*ns*sizeof(int));
			count=c;
		}
	}
	free(ng);
}

void swap(int*g,int x,int y)
{
	if(x==y)
		g[x] = 1 - g[x];
	else
	{
		int z=g[x];
		g[x]=g[y];
		g[y]=z;
	}
}


void search1()
{

		best_count = BIGCOUNT;
		for(i=0; i < gsize; i++)
		{
			for(j=i+1; j < gsize; j++)
			{
				// flip it
				g[i*gsize+j] = 1 - g[i*gsize+j];
				count = CliqueCount2(g,gsize,best_count);

				// * is it better and the i,j,count not taboo?
				if((count < best_count) && 
//					!FIFOFindEdge(taboo_list,i,j))
					!FIFOFindEdgeCount(taboo_list,i,j,count))
				{
					best_count = count;
					best_i = i;
					best_j = j;
				}

				// * flip it back
				g[i*gsize+j] = 1 - g[i*gsize+j];
			}
		}
		if(best_count == BIGCOUNT) {
			printf("no best edge found, terminating\n");
			exit(1);
		}
	
		/*
		 * keep the best flip we saw
		 */
		g[best_i*gsize+best_j] = 1 - g[best_i*gsize+best_j];


		/*
		 * taboo this graph configuration so that we don't visit
		 * it again
		 */
		count = CliqueCount(g,gsize);
//		FIFOInsertEdge(taboo_list,best_i,best_j);
		FIFOInsertEdgeCount(taboo_list,best_i,best_j,count);
//		FIFOInsertEdge(taboo_list,best_x,best_y);

		/*		
		printf("ce size: %d, best_count: %d, swap: (%d,%d)\n",
			gsize,
			best_count,
			best_x,
			best_y
			);
		*/
		printf("ce size: %d, best_count: %d, best edge: (%d,%d), new color: %d\n",
			gsize,
			best_count,
			best_i,
			best_j,
			g[best_i*gsize+best_j]);
		DumpGraph(g,gsize);
		/*
		 * rinse and repeat
		 */

}

void search2()
{

		best_count = BIGCOUNT;
		for(x=0; x < gsize*gsize; x++)
		{
			if(x/gsize>=x%gsize)
				continue;
			for(y=x; y < gsize*gsize; y++)
			{
				if(y/gsize>=y%gsize)
					continue;
	
				if(g[x]==g[y] && x!=y)
					continue;
				// flip it
				swap(g,x,y);
	
				printf(" %d - %d : %d\n",x,y,best_count);
				count = CliqueCount2(g,gsize,best_count);

				// * is it better and the i,j,count not taboo?
				if((count < best_count) && 
//					!FIFOFindEdge(taboo_list,i,j))
					!FIFOFindEdgeCount(taboo_list,x,y,count))
				{
					best_count = count;
					best_x = x;
					best_y = y;
				}

				// * flip it back
				swap(g,x,y);
			}
		}
		if(best_count == BIGCOUNT) {
			printf("no best edge found, terminating\n");
			exit(1);
		}
	
		/*
		 * keep the best flip we saw
		 */

		swap(g,x,y);

		/*
		 * taboo this graph configuration so that we don't visit
		 * it again
		 */
		count = CliqueCount(g,gsize);
//		FIFOInsertEdge(taboo_list,best_i,best_j);
		FIFOInsertEdgeCount(taboo_list,best_x,best_y,count);
//		FIFOInsertEdge(taboo_list,best_x,best_y);

		printf("ce size: %d, best_count: %d, swap: (%d,%d)\n",
			gsize,
			best_count,
			best_x,
			best_y
			);
}

void found()
{

			printf("Eureka!  Counter-example found!\n");
			PrintGraph(g,gsize);
			WriteGraph(g,gsize);

			new_g = (int *)malloc((gsize+1)*(gsize+1)*sizeof(int));
			if(new_g == NULL)
				exit(1);

			//CopyGraph(g,gsize,new_g,gsize+1);
			GoodCopy(g,gsize,new_g,gsize+1);

			free(g);
			g = new_g;
			gsize = gsize+1;

			taboo_list = FIFOResetEdge(taboo_list);
}

int
main(int argc,char *argv[])
{

	srand (time(NULL));

	/*
	 * start with graph of size 8
	 */

	ReadDump(g);
	/*
	 * make a fifo to use as the taboo list
	 */
	taboo_list = FIFOInitEdge(TABOOSIZE);
	if(taboo_list == NULL) {
		exit(1);
	}

	/*
	 * start out with all zeros
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

			found();
			continue;
		}

		search1();
		//search2();

	}

	FIFODeleteGraph(taboo_list);


	return(0);

}
