#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mpi.h>
#include "tests.h"

typedef struct {
    char *name;
    void (*func) (int taskid, int numtasks, char *args);
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
    char testargs[200];

    testname[0] = testargs[0] = '\0';
    while ((c = getopt(argc, argv, "t:a:")) != -1) {
        switch (c) {
	    case 't':
		strncpy (testname, optarg, sizeof(testname) - 1);
		testname[sizeof(testname)-1] = '\0';
		break;
	    case 'a':
		strncpy (testargs, optarg, sizeof(testargs) - 1);
		testargs[sizeof(testargs)-1] = '\0';
		assert ((strlen (testargs) < (sizeof(testargs) - 1)) && "Test arguments too long, increase size of testargs");
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
	    tests[i].func (rank, size, testargs);

    MPI_Finalize ();
    return 0;
}
