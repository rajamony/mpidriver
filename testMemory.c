#include <unistd.h>
#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <stdint.h>
#include "tests.h"
#include "utils.h"

static int vectorsize = 100*1000000;
static double runtime = 15;
static int numrounds = 1;
static double copyGBs, scaleGBs, addGBs, triadGBs;

static void myparser (int c) {
    switch (c) {
	case 'l':
	    vectorsize = strtol (optarg, NULL, 0);
	    break;
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
    int failure;
    for (uint64_t i = 0; i < n; i++)
	HPCC_Stream (vectorsize, 0, &copyGBs, &scaleGBs, &addGBs, &triadGBs, &failure);
    return (int) (copyGBs + scaleGBs + addGBs + triadGBs);
}


void
testMemory (int taskid, int numtasks, char *options) {
    int x = 0;
    printf ( "Testing memory - this is task %d of %d. Options <%s>\n", taskid, numtasks, options);
    parseOptions (options, myparser);
    uint64_t numiterations = mapRuntimeToIterationcount (runtime, kernel);

    for (int i = 0; i < numrounds; i++) {
	double starttime = MPI_Wtime();
	MPI_Barrier (MPI_COMM_WORLD);
	x += kernel (numiterations);
	MPI_Barrier (MPI_COMM_WORLD);
	if (taskid == 0)
	    fprintf (stdout, "Iteration %3d (of %3d), time = %.1f seconds, bandwidths: %7.3f %7.3f %7.3f %7.3f (dummy = %d)\n", i,
	    			numrounds, MPI_Wtime() - starttime, copyGBs, scaleGBs, addGBs, triadGBs, x);
    }
}

