#if !defined(RAMSEY_H)
#define RAMSEY_H

#define CINDEX(i,j,gsize) i*gsize+j-(i+1)*(i+2)/2

//static inline int CalIndex(int i, int j, int gsize);
void PrintGraph(int *g, int gsize);
void GetNewGraph(int *old_g, int o_gsize, int *new_g, int n_gsize, int seed);
void shuffle(int *array, int n);
void InitGraph(int *g, int size, int seed);
void InitEdges(int *edges, int size, int seed);
int CliqueCount(int *g, int gsize);
int CliqueCount_EA(int *g, int gsize);
void OutputGraph(int *g, int gsize, int seed);


#endif

