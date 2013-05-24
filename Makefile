CC = mpicc
CFLAGS = -g -O -Wall -std=gnu99 -fopenmp -MMD

OBJS = main.o testHello.o testMemory.o stream.o

driver:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

run: runmemory

runhello:
	mpirun -np 4 driver -t helloworld

runmemory:
	mpirun -np 1 driver -t memory -a "-n10 -m4"

clean:
	-rm -f $(OBJS)

-include *.d
