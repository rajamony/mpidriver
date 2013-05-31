/****************************************************************************
 * Licensed Materials - Property of IBM 
 * Copyright (C) IBM Corp. 2013, All Rights Reserved
 * 
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License v1.0 which accompanies this
 * distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 * 
 * US Government Users Restricted Rights - Use, duplication or 
 * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 * 
 * Contributor(s): Ram Rajamony, IBM
 *
 ***************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <wordexp.h>
#include <mpi.h>
#include <stdint.h>
#include "utils.h"

#define ESTIMATE_FACTOR		16
#define SEED_NUMITERATIONS	1			/* Note - we double this before using */
#define MAX_RUNTIME		((double) (15*60))	/* mapRuntimeToIterationcount won't attempt to find an iteration count for runtimes greater than this*/
#define MIN_RUNTIME		((double) (3))		/* mapRuntimeToIterationcount won't attempt to find an iteration count for runtimes smaller than this*/

static FILE *outputfile = NULL;

void InitUtilsOutput (FILE *of) {
    outputfile = of;	// What an ugly hack!
}

// Call func with an ever-increasing iteration count until we have consumed 1/ESTIMATE_FACTOR of the desired runtime.
uint64_t mapRuntimeToIterationcount (double runtime, int (*func)(uint64_t numiterations)) {
    uint64_t numiterations = SEED_NUMITERATIONS;
    double delta = 0, checkruntime = runtime / (double) ESTIMATE_FACTOR;
    int dummy = 0;

    if (runtime > MAX_RUNTIME || runtime < MIN_RUNTIME) { // Sanity check
	fprintf (outputfile, "Provided runtime %f must be in range (%f,%f)\n", runtime, MIN_RUNTIME, MAX_RUNTIME);
	exit (-1);
    }

    assert (numiterations);
    do {
	double starttime = MPI_Wtime();
	dummy += (*func) (numiterations);
	delta = MPI_Wtime() - starttime;
	// fprintf (outputfile, "mapRuntimeToIterationcount: %.3f seconds --> iteration-count of %lu\n", delta, numiterations);
	numiterations *= 2;
    } while (delta < checkruntime);
    numiterations /= 2;

    double frac = runtime / delta + ((dummy > 0) ? 1e-6 : 2e-6);
    if (frac < 1) frac = 1;
    uint64_t ret = (uint64_t) (numiterations * frac);
    fprintf (outputfile, "mapRuntimeToIterationcount: delta = %.3f, frac = %f, estimating %lu iterations for %.0f seconds runtime\n", delta, frac, ret, runtime);
    return ret;
}


void parseOptions (const char *options, const char *optstring, void (*parser) (int)) {
    signed char c;
    wordexp_t wep;
    optind = 1;	// rewind getopt
    wordexp ("dummy", &wep, 0);      // Get argv[0] out of the way
    wordexp (options, &wep, WRDE_NOCMD | WRDE_APPEND);

    while ((c = getopt(wep.we_wordc, wep.we_wordv, optstring)) != -1)
        (*parser) (c);

    wordfree (&wep);
}


void exitUnhappily (const char *because) {
    FILE *fp = fopen ("unhappy", "w+");
    fprintf (fp, "i am not happy because %s\n", because);
    // MPI_Abort (MPI_COMM_WORLD, 0);
    exit (0);
}

#if 0
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
	        fprintf (outputfile, "Can only provide runtime as option\n");
		exit (-1);
	}
    }

    uint64_t num = mapRuntimeToIterationcount (runtime, runnerfunc);

    double start = MPI_Wtime();
    int x = runnerfunc (num);
    fprintf (outputfile, "Discovered that %f seconds runtime maps to %llu iterations. Verified runtime is %f seconds (dummy %d)\n", runtime, num, MPI_Wtime() - start, x); 
}

#endif
