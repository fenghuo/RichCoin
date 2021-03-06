#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>

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
int best_count5;
int best_count4;
int best_count3;
int best_i;
int best_j;
int best_x;
int best_y;
void *taboo_list;
int flag=1;
int lastcount=IMAX;
int count5;
int count4;
int count3;
int count2;
int count1;
int*gg=NULL;
int*g5=NULL;
int gmax,gmaxv;
int cc;

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

#define GET(g, X, Y ,size)  ((X) < (Y) ? (g[X*size+Y]) : (g[Y*size+X]))
#define SET(g, X, Y ,size,c)  ((X) < (Y) ? (g[X*size+Y]=c) : (g[Y*size+X]=c))

int CliqueCount4(int *g,int gsize,int x,int y);
int CliqueCount(int *g,int gsize);

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
	sprintf(str, "paley/%d.txt", gsize);

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
    count5=0;
    cc=0;
    
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
					count5++;
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
								if(g[i*gsize+j])
									cc++;
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


//Legendre

int L(int a, int p)
       {
            if (a == 1)
                return 1;
            if (a % 2 == 0)
                return (int)(L(a / 2, p) *pow(-1, (p * p - 1) / 8));
            if ((a % 2 != 0) && (a != 1))
                return (int)(L(p % a, a) * pow(-1, (a - 1) * (p - 1) / 4));
            return 0;
        } 


int* build(int size)
{
	g=(int*)malloc(size*size*sizeof(int));
	memset(g,0,size*size*sizeof(int));
	for(i=0;i<size;i++)
		for(j=i+1;j<size;j++)
			if(L(j-i,size)==1)
				g[size*i+j]=1;
	return g;
}

int
main(int argc,char *argv[])
{

	srand (time(NULL));

	/*
	 * start with graph of size 8
	 */

	//ReadDump(g);
	
	
	/*
	int og=gsize;
        gsize=100;
        new_g=(int*)malloc(gsize*gsize*sizeof(int));
        CopyGraph(g,og,new_g,gsize);
        free(g);
        g=new_g;
	draw(g,gsize);
	*/

	gsize=125 ;

	printf(" -- \n");

	g=build(gsize);

	PrintGraph(g,gsize);
	WriteGraph(g,gsize);

	count = CliqueCount(g,gsize);

	printf("%d : %d \n",gsize,count);


	return(0);

}
