CC = mpicc
CFLAGS = -g -O -Wall -std=gnu99 -fopenmp -MMD

OBJS = main.o testHello.o testMemory.o stream.o testCompute.o testInterconnect.o utils.o

driver:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

run: runmemory

runhello:
	mpirun -np 4 driver -t helloworld

runmemory:
	mpirun -np 1 driver -t memory -o "-t23 -r4"

runcompute:
	mpirun -np 1 driver -t compute -o "-t13 -r10"

runinterconnect:
	mpirun -np 1 driver -t interconnect -o "-i4"

clean:
	-rm -f $(OBJS)

-include *.d
