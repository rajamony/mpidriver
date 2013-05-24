CC = mpicc
CFLAGS = -g -O -Wall -std=gnu99 -fopenmp -MMD

OBJS = main.o testHello.o testMemory.o stream.o testCompute.o

driver:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

run: runmemory

runhello:
	mpirun -np 4 driver -t helloworld

runmemory:
	mpirun -np 1 driver -t memory -o "-n10 -i4"

runcompute:
	mpirun -np 1 driver -t compute -o "-i4"

clean:
	-rm -f $(OBJS)

-include *.d
