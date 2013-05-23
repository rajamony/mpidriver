#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <mpi.h>
#include "tests.h"

typedef struct {
    char *name;
    void (*func) (int taskid, int numtasks);
} Test;

Test tests[] = {
	{"helloworld",	testHello}, 
	{"memory", 	testMemory}, 
    };


int 
main (int argc, char *argv[]) {
    int rank, size;
    signed char c;
    char testname[32];

    memset (testname, 0, sizeof(testname));
    while ((c = getopt(argc, argv, "t:")) != -1) {
        switch (c) {
	    case 't':
		strncpy (testname, optarg, sizeof(testname) - 1);
		break;
	    default:
	        fprintf (stderr, "Must provide test name\n");
		exit (-1);
	}
    }

    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);

    for (int i = 0; i < sizeof(tests)/sizeof(Test); i++)
	if (0 == strcmp (testname, tests[i].name))
	    tests[i].func (rank, size);

    MPI_Finalize ();
    return 0;
}
