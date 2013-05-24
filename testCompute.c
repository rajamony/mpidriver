#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "tests.h"
#include "utils.h"

static double runtime = 15;
static int numrounds = 1;

static void myparser (int c) {
    switch (c) {
	case 't':
	    runtime = strtof (optarg, NULL);
	    break;
	case 'r':
	    numrounds = strtol (optarg, NULL, 0);
	    break;
	default:
	    fprintf (stderr, "Must provide valid arguments\n");
	    exit (-1);
    }
}


static int kernel (uint64_t n) {
    double x = 0;
#ifdef _OPENMP
#pragma omp parallel for reduction(+:x)  
#endif
    for (uint64_t i = 0; i < n; i++) {
	double y = ((double) i) / 10;
        x += i / (i + y);
    }
    return (int) x;
}


void
testCompute (int taskid, int numtasks, char *options) {
    printf ( "Testing compute - this is task %d of %d. Options <%s>\n", taskid, numtasks, options);
    parseOptions (options, myparser);
    uint64_t numiterations = mapRuntimeToIterationcount (runtime, kernel);

    for (int i = 0; i < numrounds; i++) {
	double starttime = MPI_Wtime();
	MPI_Barrier (MPI_COMM_WORLD);
	kernel (numiterations);
	MPI_Barrier (MPI_COMM_WORLD);
	if (taskid == 0)
	    fprintf (stdout, "Iteration %3d (of %3d) took %.1f seconds\n", i, numrounds, MPI_Wtime() - starttime);
    }
}
