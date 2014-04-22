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

void draw5(int*g,int gsize,int*p)
{
	int i,j;
	
	for(i=0;i<4;i++)
		for(j=i+1;j<5;j++)
			SET(g,gsize,p[i],p[j],1);

}

void draw6(int k,int * p)
{
	int i,j;

	for(i=0;i<gsize;i++)
	{
		for(j=0;j<5;j++)
			p[j]=(i+k*j)%gsize;
		draw5(g,gsize,p);
	}
}

void ST(int*g,int gsize,int a,int b)
{
	if(a>b)
	{
		int t=a;
		a=b;
		b=t;
	}
	g[a*gsize+b]=1;
}

void draw7(int*g,int gsize,int k)
{
	int i,j;
	for(i=0;i<gsize;i++)
		ST(g,gsize,i,(i+k)%gsize);

}

void draw(int * g, int gsize)
{
	int i,j;

	int p[5];

	g=memset(g,0,gsize*gsize*sizeof(int));

	for(i=1;i<5;i++)
		draw7(g,gsize,i);

	draw7(g,gsize,6);
	draw7(g,gsize,7);
	draw7(g,gsize,8);
	draw7(g,gsize,9);

	draw7(g,gsize,11);
	draw7(g,gsize,12);
	draw7(g,gsize,13);
	draw7(g,gsize,14);

	draw7(g,gsize,16);
	draw7(g,gsize,17);
	draw7(g,gsize,18);
	draw7(g,gsize,19);

	draw7(g,gsize,21);
	draw7(g,gsize,22);
	draw7(g,gsize,23);
	draw7(g,gsize,24);

	draw7(g,gsize,26);
	draw7(g,gsize,27);
	draw7(g,gsize,28);
	draw7(g,gsize,29);

	/*
	draw7(g,gsize,31);
	draw7(g,gsize,32);
	draw7(g,gsize,33);
	draw7(g,gsize,34);

	draw7(g,gsize,36);
	draw7(g,gsize,37);
	draw7(g,gsize,38);
	draw7(g,gsize,39);
	*/
	count=CliqueCount(g,gsize);

	printf("cc %d,count: %d\n",cc,count);

	int best;
	/*
	while(1)
	{
	best=IMAX;
	best_i=-1;
	for(i=0;i<gsize;i++)
		for(j=i+1;j<gsize;j++)
		{
			if(!g[i*gsize+j])
			{
				g[i*gsize+j]=1-g[i*gsize+j];
				count=CliqueCount(g,gsize);
				g[i*gsize+j]=1-g[i*gsize+j];
				if(count<best && !cc)
				{
					best_i=i;
					best_j=j;
					best=count;
					printf(" we can choose : %d, %d -> %d \n",i,j,count);
				}
			}

		}
	if(best_i<0)
		exit(1);		
	g[best_i*gsize+best_j]=1-g[best_i*gsize+best_j];
	}
*/
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
    count5=count4=count3=count2=count1=0;
    
    for(i=0;i < gsize-sgsize+1; i++)
    {
	for(j=i+1;j < gsize-sgsize+2; j++)
        {
	    for(k=j+1;k < gsize-sgsize+3; k++) 
            { 
		if((g[i*gsize+j] == g[i*gsize+k]) && 
		   (g[i*gsize+j] == g[j*gsize+k]))
		{
		    count3++;
		    for(l=k+1;l < gsize-sgsize+4; l++) 
		    { 
			if((g[i*gsize+j] == g[i*gsize+l]) && 
			   (g[i*gsize+j] == g[j*gsize+l]) && 
			   (g[i*gsize+j] == g[k*gsize+l]))
			{
			    count4++;
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

void add(int*p,int count) {
	int i,j,incr,decr;
	for(i=0;i<6;i++)
		for(j=i+1;j<6;j++)
		{
			decr=++gg[p[i]*gsize+p[j]];
			if(decr>gmax && !FIFOFindEdgeCount(taboo_list,p[i],p[j],count))
			{
				incr=CliqueCount4(g,gsize,p[i],p[j]);
				if(decr-incr>gmax)
				{
					gmax=decr-incr;
					gmaxv=p[i]*gsize+p[j];
					printf("%d,%d - %d\n",p[i],p[j],gmax);
				}
			}

		}
}

void add2(int*p,int count) {
	int i,j,incr,decr;
	for(i=0;i<6;i++)
		for(j=i+1;j<6;j++)
			++gg[p[i]*gsize+p[j]];
}

int CliqueCount3(int *g,int gsize)
{
    int i;
    int j;
    int k;
    int l;
    int m;
    int n;
    int count=0;
    int sgsize = 6;
    count5=count4=count3=count2=count1=0;
    
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
								g5[m*gsize+n]=count5;
							int t[6];
							t[0]=i;
							t[1]=j;
							t[2]=k;
							t[3]=l;
							t[4]=m;
							t[5]=n;
							add2(t,count);	
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

int CliqueCount4(int *g,int gsize,int x,int y)
{
    int i;
    int j;
    int k;
    int l;
    int m;
    int n;
    int count=0;
    int sgsize = 4;
    int c=1-g[x*gsize+y];

    for(i=0;i < gsize-sgsize+1; i++)
    {
	if(i==x || i==y)
		continue;
	
	if(GET(g,i,x,gsize)==c && GET(g,i,y,gsize)==c)
	{
		for(j=i+1;j < gsize-sgsize+2; j++)
	       	 {
		    if(j==x || j==y)
			continue;
		if(GET(g,j,x,gsize)==c && GET(g,j,y,gsize)==c && GET(g,j,i,gsize)==c)
		{

		    for(k=j+1;k < gsize-sgsize+3; k++) 
	            { 
			if(k==x || k==y)
				continue;
		
			if(GET(g,k,x,gsize)==c && GET(g,k,y,gsize)==c && GET(g,k,i,gsize)==c && GET(g,k,j,gsize)==c)
			{
			    for(l=k+1;l < gsize-sgsize+4; l++) 
			    { 	
				if(l==x || l==y)
					continue;
				if(GET(g,l,x,gsize)==c && GET(g,l,y,gsize)==c && GET(g,l,i,gsize)==c && GET(g,l,j,gsize)==c && GET(g,l,k,gsize)==c)
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

int CliqueCount5(int *g, int gsize)
{
    int i;
    int j;
    int k;
    int l;
    int m;
    int n;
    int count=0;
    int sgsize = 4;
    count5=0;
    
	int c=0;
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
			   // target m,n
			    for(m=0;m < gsize; m++) 
			    {
				if(m==i || m==j || m==k || m==l)
					continue;
				if((g[i*gsize+j] == g[i*gsize+m]) && 
				   (g[i*gsize+j] == g[j*gsize+m]) &&
				   (g[i*gsize+j] == g[k*gsize+m]) && 
				   (g[i*gsize+j] == g[l*gsize+m])) {
					for(n=m+1; n < gsize; n++)
					{
						if(n==i || n==j || n==k || n==l)
							continue;
						if((g[i*gsize+j] == g[i*gsize+n]) &&
						   (g[i*gsize+j] == g[j*gsize+n]) &&
						   (g[i*gsize+j] == g[k*gsize+n]) &&
						   (g[i*gsize+j] == g[l*gsize+n]) &&
						   (g[i*gsize+j] != g[m*gsize+n])) {
	
							gg[m*gsize+n]--;

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

int CliqueCount6(int *g, int gsize)
{
	int i,j;
	for(i=0;i<gsize;i++)
		for(j=i+1;j<gsize;j++)
			CliqueCount4(g,gsize,i,j);

}

int CliqueCount7(int *g,int gsize)
{
    int i;
    int j;
    int k;
    int l;
    int m;
    int n;
    int count=0;
    int sgsize = 6;
    count5=count4=count3=count2=count1=0;
    
    for(i=0;i < gsize-sgsize+1; i++)
    {
	for(j=i+1;j < gsize-sgsize+2; j++)
        {
		if(g[i*gsize+j])
			continue;
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
								g5[m*gsize+n]=count5;
							int t[6];
							t[0]=i;
							t[1]=j;
							t[2]=k;
							t[3]=l;
							t[4]=m;
							t[5]=n;
							add2(t,count);	
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

void update()
{

	best_count = count;
	best_count5 = count5;
	best_i = i;
	best_j = j;
	//printf("%d,%d --> %d\n",i,j,count);
}

void search1()
{

		best_count = BIGCOUNT;
		best_count5 = BIGCOUNT;
		for(i=0; i < gsize; i++)
		{
			for(j=i+1; j < gsize; j++)
			{
				// flip it
				g[i*gsize+j] = 1 - g[i*gsize+j];
				count = CliqueCount2(g,gsize,best_count);


				// * is it better and the i,j,count not taboo?
				if((count <= best_count) && 
//					!FIFOFindEdge(taboo_list,i,j))
					!FIFOFindEdgeCount(taboo_list,i,j,count))
				{
					printf("%d,%d : %d\n",i,j,count);
					if(count<best_count )
						update();
					else if(count5<=best_count5)
					{
						printf(" %d %d - %d,|%d |%d\n",i,j,count,count5,count4);
						if(count5<best_count5)
						{
							update();	
							best_count5=count5;
						}
						else if(count4<best_count4)
						{
							update();
							count4=best_count4;
						}
					}
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
		FIFOInsertEdgeCount(taboo_list,best_i,best_j,best_count);
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
		lastcount=best_count;

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

void search3()
{

		best_count = BIGCOUNT;
		for(i=0; i < gsize; i++)
		{
			for(j=gsize-3; j < gsize-2; j++)
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

void search4()
{

		best_count = BIGCOUNT;
		best_count5 = BIGCOUNT;

		for(i=0; i < gsize; i++)
		{
			for(j=i+1; j < gsize; j++) {
				// flip it
				g[i*gsize+j] = 1 - g[i*gsize+j];
			}

			count = CliqueCount2(g,gsize,best_count);
			if((count < best_count) && 
//				!FIFOFindEdge(taboo_list,i,j))
				!FIFOFindEdgeCount(taboo_list,i,0,count))
			{
				printf("%d,%d : %d\n",i,j,count);
				best_count=count;
				best_i=i;
				best_j=0;
			}
			for(j=i+1; j < gsize; j++)
			{
				// flip it
				g[i*gsize+j] = 1 - g[i*gsize+j];
			}
		}

		for(j=0; j < gsize; j++)
		{
			for(i=j-1;i>=0;i--)
			{
				// flip it
				g[i*gsize+j] = 1 - g[i*gsize+j];
			}

			count = CliqueCount2(g,gsize,best_count);
			if((count < best_count) && 
//				!FIFOFindEdge(taboo_list,i,j))
				!FIFOFindEdgeCount(taboo_list,0,j,count))
			{
				printf("%d,%d : %d\n",i,j,count);
				best_count=count;
				best_i=0;
				best_j=j;
			}
			for(i=j-1;i>=0;i--)
			{
				// flip it
				g[i*gsize+j] = 1 - g[i*gsize+j];
			}
		}


		if(best_count == BIGCOUNT) {
			exit(1);
		}
	
		/*
		 * keep the best flip we saw
		 */
		if(best_i)
		{
			i=best_i;
			for(j=i+1; j < gsize; j++)
			{
				// flip it
				g[i*gsize+j] = 1 - g[i*gsize+j];
			}
		}
		else
		{
			j=best_j;
			for(i=j-1;i>=0;i--)
			{
				// flip it
				g[i*gsize+j] = 1 - g[i*gsize+j];
			}

		}
		/*
		 * taboo this graph configuration so that we don't visit
		 * it again
		 */
		count = CliqueCount(g,gsize);
//		FIFOInsertEdge(taboo_list,best_i,best_j);
		FIFOInsertEdgeCount(taboo_list,best_i,best_j,best_count);
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
		lastcount=best_count;

		DumpGraph(g,gsize);
		/*
		 * rinse and repeat
		 */

}
void search5()
{

		best_count = BIGCOUNT;
		best_count5 = BIGCOUNT;

		for(i=0; i < gsize; i++)
		{
			for(j=i+1; j < gsize; j++) 
			{
				// flip it
			g[i*gsize+j] = 1 - g[i*gsize+j];

			count = CliqueCount2(g,gsize,best_count);
			if((count < best_count) && 
//				!FIFOFindEdge(taboo_list,i,j))
				!FIFOFindEdgeCount(taboo_list,i,j,count))
			{
				printf("%d,%d : %d\n",i,j,count);
				best_count=count;
				best_i=i;
				best_j=j;
			}
			for(j=i+1; j < gsize; j++)
			{
				// flip it
				g[i*gsize+j] = 1 - g[i*gsize+j];
			}
			}
		}

		if(best_count == BIGCOUNT) {
			exit(1);
		}
	
		/*
		 * keep the best flip we saw
		 */
		i=best_i;
		for(j=i+1; j <= best_j; j++)
		{
			// flip it
			g[i*gsize+j] = 1 - g[i*gsize+j];
		}
		/*
		 * taboo this graph configuration so that we don't visit
		 * it again
		 */
		count = CliqueCount(g,gsize);
//		FIFOInsertEdge(taboo_list,best_i,best_j);
		FIFOInsertEdgeCount(taboo_list,best_i,best_j,best_count);
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
	
		lastcount=best_count;

		DumpGraph(g,gsize);
		/*
		 * rinse and repeat
		 */

}

int N=30;

void search6()
{

		best_count = BIGCOUNT;
		best_count5 = BIGCOUNT;

		for(i=0; i < gsize; i++)
		{
			for(j=i+1; j < gsize; j++) 
			{
				// flip it
			g[i*gsize+j] = 1 - g[i*gsize+j];

			count = CliqueCount2(g,gsize,best_count);
			if((count < best_count) && 
//				!FIFOFindEdge(taboo_list,i,j))
				!FIFOFindEdgeCount(taboo_list,i,j,count))
			{
				printf("%d,%d : %d\n",i,j,count);
				best_count=count;
				best_i=i;
				best_j=j;
			}
			for(j=i+1; j < gsize; j++)
			{
				// flip it
				g[i*gsize+j] = 1 - g[i*gsize+j];
			}
			}
		}

		if(best_count == BIGCOUNT) {
			exit(1);
		}
	
		/*
		 * keep the best flip we saw
		 */
		i=best_i;
		for(j=i+1; j <= best_j; j++)
		{
			// flip it
			g[i*gsize+j] = 1 - g[i*gsize+j];
		}
		/*
		 * taboo this graph configuration so that we don't visit
		 * it again
		 */
		count = CliqueCount(g,gsize);
//		FIFOInsertEdge(taboo_list,best_i,best_j);
		FIFOInsertEdgeCount(taboo_list,best_i,best_j,best_count);
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
	
		lastcount=best_count;

		DumpGraph(g,gsize);
		/*
		 * rinse and repeat
		 */

}


void search7()
{
		int i,j;
		best_count = BIGCOUNT;
		best_count5 = BIGCOUNT;

		if(!gg)
		gg=(int*)malloc(gsize*gsize*sizeof(int));
		if(!g5)
		g5=(int*)malloc(gsize*gsize*sizeof(int));

		memset(gg,0,gsize*gsize*sizeof(int));
		memset(g5,0,gsize*gsize*sizeof(int));
		gmax=0;

		count=CliqueCount3(g,gsize);

		i=gmaxv/gsize;
		j=gmaxv%gsize;

	//	printf("test 10,30 : %d\n",gg[10*gsize+30]-CliqueCount4(g,gsize,10,30));
	//	exit(1);


		gmax=-IMAX;
		gmaxv=0;
		int c=0;
		int m,n,decr,incr;
		for(m=0;m<gsize;m++)
			for(n=m+1;n<gsize;n++)
			{
				decr=gg[m*gsize+n];
				if(decr>gmax && !FIFOFindEdgeCount(taboo_list,m,n,count))
			{
				c++;
				incr=CliqueCount4(g,gsize,m,n);
				if(decr-incr>gmax)
				{
					gmax=decr-incr;
					gmaxv=m*gsize+n;
					printf("%d,%d - %d\n",m,n,gmax);
				}
				else if(decr-incr==gmax && g5[m*gsize+n]<best_count5)
				{
					best_count5=g5[m*gsize+n];
					gmax=decr-incr;
					gmaxv=m*gsize+n;
					printf("%d,%d - %d\n",m,n,gmax);
				}
			}
			}

		printf(" loop %d\n",c);

		i=gmaxv/gsize;
		j=gmaxv%gsize;
	
		g[i*gsize+j] = 1 - g[i*gsize+j];

		best_count=count-gmax;
	
//		FIFOInsertEdge(taboo_list,i,j);
		FIFOInsertEdgeCount(taboo_list,i,j,count);
//		FIFOInsertEdge(taboo_list,best_x,best_y);

		printf("ce size: %d, best_count: %d, best edge: (%d,%d), new color: %d\n",
			gsize,
			best_count,
			i,
			j,
			g[i*gsize+j]);
	
		lastcount=best_count;

		DumpGraph(g,gsize);
}

void search8()
{
		int i,j;
		best_count = BIGCOUNT;
		best_count5 = BIGCOUNT;

		gg=(int*)malloc(gsize*gsize*sizeof(int));

		memset(gg,0,gsize*gsize*sizeof(int));
		gmax=0;

		count=CliqueCount3(g,gsize);
		CliqueCount6(g,gsize);

		printf("test 10,30 : %d\n",gg[10*gsize+30]);
		exit(1);

		gmax=-IMAX;
		gmaxv=0;
		int m,n,diff;
		for(m=0;m<gsize;m++)
			for(n=m+1;n<gsize;n++)
			{
				diff=gg[m*gsize+n];
				if(diff>gmax && !FIFOFindEdgeCount(taboo_list,m,n,count))
				{
					gmax=diff;
					gmaxv=m*gsize+n;
					printf("%d,%d - %d\n",m,n,gmax);
				}
			}

		i=gmaxv/gsize;
		j=gmaxv%gsize;
	
		g[i*gsize+j] = 1 - g[i*gsize+j];

		best_count=CliqueCount(g,gsize);

//		FIFOInsertEdge(taboo_list,i,j);
		FIFOInsertEdgeCount(taboo_list,i,j,count);
//		FIFOInsertEdge(taboo_list,best_x,best_y);

		printf("ce size: %d, best_count: %d, best edge: (%d,%d), new color: %d\n",
			gsize,
			best_count,
			i,
			j,
			g[i*gsize+j]);
	
		lastcount=best_count;

		DumpGraph(g,gsize);
}

void search9()
{
		int i,j;
		best_count = BIGCOUNT;
		best_count5 = BIGCOUNT;

		if(!gg)
		gg=(int*)malloc(gsize*gsize*sizeof(int));
		if(!g5)
		g5=(int*)malloc(gsize*gsize*sizeof(int));

		memset(gg,0,gsize*gsize*sizeof(int));
		memset(g5,0,gsize*gsize*sizeof(int));
		gmax=0;

		count=CliqueCount3(g,gsize);

		i=gmaxv/gsize;
		j=gmaxv%gsize;

	//	printf("test 10,30 : %d\n",gg[10*gsize+30]-CliqueCount4(g,gsize,10,30));
	//	exit(1);


		gmax=-IMAX;
		gmaxv=0;
		int c=0;
		int m,n,decr,incr;
		for(m=0;m<gsize;m++)
			for(n=m+1;n<gsize;n++)
			{
				decr=gg[m*gsize+n];
				if(decr>gmax && !FIFOFindEdgeCount(taboo_list,m,n,count))
			{
				c++;
				incr=CliqueCount4(g,gsize,m,n);
				if(decr-incr>gmax)
				{
					gmax=decr-incr;
					gmaxv=m*gsize+n;
					printf("%d,%d - %d\n",m,n,gmax);
				}
				else if(decr-incr==gmax && g5[m*gsize+n]<best_count5)
				{
					best_count5=g5[m*gsize+n];
					gmax=decr-incr;
					gmaxv=m*gsize+n;
					printf("%d,%d - %d\n",m,n,gmax);
				}
			}
			}

		printf(" loop %d\n",c);

		i=gmaxv/gsize;
		j=gmaxv%gsize;
	
		if(gmax<-2)
		{
			printf(" I Jump ###################################################\n");
			i=j=rand()%(gsize-1);
			while(j<=i)
				j=rand()%gsize;
		}

		g[i*gsize+j] = 1 - g[i*gsize+j];

		best_count=count-gmax;
	
//		FIFOInsertEdge(taboo_list,i,j);
		FIFOInsertEdgeCount(taboo_list,i,j,count);
//		FIFOInsertEdge(taboo_list,best_x,best_y);

		printf("ce size: %d, best_count: %d, best edge: (%d,%d), new color: %d\n",
			gsize,
			best_count,
			i,
			j,
			g[i*gsize+j]);
	
		lastcount=best_count;

		DumpGraph(g,gsize);
}

void search10()
{
	int i,j;
	
	memset(g,0,gsize*gsize*sizeof(int));

	//printf("%d\n",CliqueCount(g,gsize));
	draw(g,gsize);

	printf("%d\n",CliqueCount(g,gsize));

	exit(1);
}

//count7 only count1
void search11()
{
		int i,j;
		best_count = BIGCOUNT;
		best_count5 = BIGCOUNT;

		if(!gg)
		gg=(int*)malloc(gsize*gsize*sizeof(int));
		if(!g5)
		g5=(int*)malloc(gsize*gsize*sizeof(int));

		memset(gg,0,gsize*gsize*sizeof(int));
		memset(g5,0,gsize*gsize*sizeof(int));
		gmax=0;

		count=CliqueCount7(g,gsize);

		gmax=-IMAX;
		gmaxv=0;
		int c=0;
		int m,n,decr,incr;
		for(m=0;m<gsize;m++)
			for(n=m+1;n<gsize;n++)
			{
				decr=gg[m*gsize+n];
				if(decr>gmax && !FIFOFindEdgeCount(taboo_list,m,n,count))
			{
				incr=CliqueCount4(g,gsize,m,n);
				if(incr)
					continue;
				if(decr>gmax)
				{
					gmax=decr-incr;
					gmaxv=m*gsize+n;
					printf("%d,%d - %d\n",m,n,gmax);
				}
			}
			}

		i=gmaxv/gsize;
		j=gmaxv%gsize;
	
		g[i*gsize+j] = 1 - g[i*gsize+j];

		best_count=count-gmax;
	
//		FIFOInsertEdge(taboo_list,i,j);
		FIFOInsertEdgeCount(taboo_list,i,j,count);
//		FIFOInsertEdge(taboo_list,best_x,best_y);

		printf("ce size: %d, best_count: %d, best edge: (%d,%d), new color: %d\n",
			gsize,
			CliqueCount(g,gsize),
			i,
			j,
			g[i*gsize+j]);
	
		lastcount=best_count;

		DumpGraph(g,gsize);
}
void sort(int*s,int n)
{
	int i,j;
	for(i=0;i<n;i++)
		for(j=i+1;j<n;j++)
	{
		if(s[i]>s[j])
		{
			int t=s[j];
			s[j]=s[i];
			s[i]=t;
		}
	}
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
	int og=gsize;
        gsize=100;
        new_g=(int*)malloc(gsize*gsize*sizeof(int));
        CopyGraph(g,og,new_g,gsize);
        free(g);
        g=new_g;
	draw(g,gsize);
	*/

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

		search7();
		//search2();

	}

	FIFODeleteGraph(taboo_list);


	return(0);

}
