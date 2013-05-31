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
#include <mpi.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "tests.h"
#include "utils.h"

static double runtime = 0, expectation = 1e100;
static int numrounds = 1;
static uint64_t numiterations = 0;
static FILE *outputfile = NULL;

static void myparser (int c) {
    switch (c) {
	case 't':
	    runtime = strtof (optarg, NULL);
	    break;
	case 'r':
	    numrounds = strtol (optarg, NULL, 0);
	    break;
	case 'e':
	    expectation = strtof (optarg, NULL);
	    break;
	case 'n':
	    numiterations = strtoull (optarg, NULL, 0);
	    break;
	default:
	    fprintf (outputfile, "Must provide valid arguments\n");
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
testCompute (FILE *of, int taskid, int numtasks, char *options) {
    int x = 0;
    outputfile = of;
    fprintf (outputfile, "Testing compute - this is task %d of %d. Options <%s>\n", taskid, numtasks, options);
    parseOptions (options, "t:r:n:e:", myparser);
    if ((runtime != 0) && numiterations) {
        fprintf (outputfile, "testCompute: Only one of iteration count OR runtime can be specified\n");
	exit (-1);
    }
    if (numiterations == 0)
	numiterations = mapRuntimeToIterationcount (runtime, kernel);

    for (int i = 0; i < numrounds; i++) {
	double starttime = MPI_Wtime();
	MPI_Barrier (MPI_COMM_WORLD);
	x += kernel (numiterations);
	MPI_Barrier (MPI_COMM_WORLD);
	double endtime = MPI_Wtime();
	if (taskid == 0) {
	    fprintf (outputfile, "Iteration %3d (of %3d) took %.1f seconds (dummy = %d)\n", i, numrounds, endtime - starttime, x);
	    if ((endtime - starttime) > expectation)
	        exitUnhappily("too long");
	}
	MPI_Barrier (MPI_COMM_WORLD);
    }
}
