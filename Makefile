CC = mpicc
CFLAGS = -g -O -Wall -std=gnu99

OBJS = main.o tests.o

driver:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

run:
	mpirun -np 4 driver -t helloworld
