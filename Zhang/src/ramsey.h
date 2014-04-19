#if !defined(RAMSEY_H)
#define RAMSEY_H

void shuffle(int *array, int n);
void InitGraph(int *g, int size, int seed);
void InitEdges(int *edges, int size, int seed);
int CliqueCount_EA(int *g, int gsize, int best_count);
void OutputGraph(int *g, int gsize, int seed);


#endif

