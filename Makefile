CC = mpicc
CFLAGS = -g -O -Wall -std=gnu99 -fopenmp -MMD

OBJS = main.o testHello.o testMemory.o stream.o testCompute.o testInterconnect.o utils.o

all:	driver

driver:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

run: runmemory

runhello:
	mpirun -np 1 driver -t helloworld 

runmemory:
	mpirun -np 1 driver -t memory -o "-n1 -r30 -e5" 

runcompute:
	mpirun -np 1 driver -t compute -o "-n1541519990 -r30 -e4" 

runinterconnect:
	mpirun -np 1 driver -t interconnect -o "-i4" 

clean:
	-rm -f $(OBJS)

-include *.d

