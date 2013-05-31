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
#include <mpi.h>
#include <stdio.h>
#include <stdint.h>
#include "tests.h"
#include "utils.h"

#define VECTORSIZE	(50 * 1000000)

static double runtime = 15;
static int numrounds = 1;
static uint64_t numiterations = 0;
static double copyGBs, scaleGBs, addGBs, triadGBs;

static void myparser (int c) {
    switch (c) {
	case 'n':
	    numiterations = strtol (optarg, NULL, 0);
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
	HPCC_Stream (VECTORSIZE, 0, &copyGBs, &scaleGBs, &addGBs, &triadGBs, &failure);
    return (int) (copyGBs + scaleGBs + addGBs + triadGBs);
}


void
testMemory (int taskid, int numtasks, char *options) {
    int x = 0;
    printf ( "Testing memory - this is task %d of %d. Options <%s>\n", taskid, numtasks, options);
    parseOptions (options, myparser);
    if ((runtime != 0) && numiterations) {
        fprintf (stderr, "testMemory: Only one of iteration count OR runtime can be specified\n");
	exit (-1);
    }
    numiterations = mapRuntimeToIterationcount (runtime, kernel);

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

