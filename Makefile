PROGRAM = a-star-simulation
CC      = gcc 
CFLAGS = -std=c99 -Wall  -Wextra 
LDCLIBS  = -lglut -lGLU -lGL -lm

OBJ	=	simulation.o \
		graph.o \
		node.o	\
		pathfinding.o \
		queue.o  \
		graphic.o
	 
VPATH	= src

$(PROGRAM): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDCLIBS) 
	
	
simulation.o: simulation.c graphic.c
	$(CC) $(CFLAGS) -c $^ $(CFLAGS)

graph.o: graph.c
	$(CC) $(CFLAGS) -c $^ $(CFLAGS)

node.o: node.c
	$(CC) $(CFLAGS) -c $^ $(CFLAGS)	

pathfinding.o: pathfinding.c
	$(CC) $(CFLAGS) -c $^ $(CFLAGS)	

queue.o: queue.c
	$(CC) $(CFLAGS) -c $^ $(CFLAGS)	
	
	
	
.PHONY: clean

clean:
	rm -f *.o *.out src/*.o src/*.out $(PROGRAM) 
