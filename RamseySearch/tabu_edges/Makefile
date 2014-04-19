CC = gcc
TOP = .
INCLUDES = -I$(TOP)
CFLAGS = -g 
COMPILE = $(CC) $(INCLUDES) $(CFLAGS)

INCL=./jrb.h ./jval.h ./dllist.h ./fifo.h ./ramsey.h
COMMON=.
PRED=.
BOBJ=$(COMMON)/jval.o $(COMMON)/jrb.o $(COMMON)/dllist.o $(COMMON)/fifo.o $(COMMON)/ramsey.o
LIBS= -lm 

PROGRAMS = tabu_edges_search

all: $(PROGRAMS)


tabu_edges_search: tabu_edges_search.c $(INCL) ${BOBJ}
	$(COMPILE) ${INCLUDES} -o tabu_edges_search tabu_edges_search.c $(BOBJ) $(LIBS)

fifo.o: fifo.c fifo.h jrb.h jval.h dllist.h
	$(COMPILE) ${INCLUDES} -c fifo.c

jrb.o: jrb.c jrb.h jval.h
	$(COMPILE) ${INCLUDES} -c jrb.c

jval.o: jval.c jval.h
	$(COMPILE) ${INCLUDES} -c jval.c

dllist.o: dllist.c dllist.h jval.h
	$(COMPILE) ${INCLUDES} -c dllist.c

ramsey.o: ramsey.c ramsey.h
	$(COMPILE) ${INCLUDES} -c ramsey.c

clean:
	rm -f *.o core
