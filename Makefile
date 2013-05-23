CC = mpicc
CFLAGS = -g -O -Wall -std=gnu99

OBJS = main.o testHello.o testMemory.o

driver:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

run: runmemory

runhello:
	mpirun -np 4 driver -t helloworld

runmemory:
	mpirun -np 4 driver -t memory
