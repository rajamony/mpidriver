#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <mpi.h>
#include <stdint.h>
#include "utils.h"

#define ESTIMATOR_FRACTION	(1 / (double) 16)
#define SEED_NUMITERATIONS	1024
#define MAX_RUNTIME		((double) (15*60))	/* mapRuntimeToIterationcount won't attempt to find an iteration count for runtimes greater than this*/
#define MIN_RUNTIME		((double) (10))		/* mapRuntimeToIterationcount won't attempt to find an iteration count for runtimes smaller than this*/

// Call func with an ever-increasing iteration count until we have consumed ESTIMATOR_FRACTION of the desired runtime.
uint64_t mapRuntimeToIterationcount (double runtime, int (*func)(uint64_t numiterations)) {
    // Don't even attempt to find the required number of iterations for a runtime that exceeds MAX_RUNTIME. This is a sanity check
    if (runtime > MAX_RUNTIME || runtime < MIN_RUNTIME) {
	fprintf (stderr, "Provided runtime %f must be in range (%f,%f)\n", runtime, MIN_RUNTIME, MAX_RUNTIME);
	exit (-1);
    }

    uint64_t numiterations = 0;
    double delta = 0, prevdelta = 0, checkruntime = ESTIMATOR_FRACTION * runtime;
    int dummy = 0;

    for (numiterations = SEED_NUMITERATIONS; ; numiterations *= 2) { 
	double starttime = MPI_Wtime();
	dummy += (*func) (numiterations);
	delta = MPI_Wtime() - starttime;
	fprintf (stdout, "mapRuntimeToIterationcount: %.3f seconds --> iteration-count of %llu\n", delta, numiterations);
	if ((delta >= checkruntime) && (prevdelta > 0)) 
	    break;
	prevdelta = delta;
    }
    // Now linearly iterpolate between checkruntime and delta to find the desired number of iterations 

    assert ((prevdelta < checkruntime) && (delta >= checkruntime));
    double frac = checkruntime / delta + ((dummy > 0) ? 1e-6 : 2e-6);
    fprintf (stdout, "delta = %.3f, prevdelta = %.3f, frac = %f\n", delta, prevdelta, frac);
    return (uint64_t) ((numiterations * frac) * (1 / ESTIMATOR_FRACTION));
}

#if 1
#include <math.h>

int runnerfunc (uint64_t n) {
    double x = 0;
    for (uint64_t i = 0; i < n; i++) {
        // x += sin((double) i);
	double y = ((double) i) / 10;
        x += i / (i + y);
    }
    return (int) x;
}

int main (int argc, char **argv) {
    signed char c;
    double runtime = 0;

    while ((c = getopt(argc, argv, "t:")) != -1) {
        switch (c) {
	    case 't':
		runtime = atof (optarg);
		break;
	    default:
	        fprintf (stderr, "Can only provide runtime as option\n");
		exit (-1);
	}
    }

    uint64_t num = mapRuntimeToIterationcount (runtime, runnerfunc);

    double start = MPI_Wtime();
    int x = runnerfunc (num);
    fprintf (stderr, "Discovered that %f seconds runtime maps to %llu iterations. Verified runtime is %f seconds (dummy %d)\n", runtime, num, MPI_Wtime() - start, x); 
}

#endif
