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

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <mpi.h>
#include <stdint.h>
#include "tests.h"
#include "utils.h"

#define OUTPUTFILE	"OUT.txt"

typedef struct {
    char *name;
    void (*func) (FILE *of, int taskid, int numtasks, char *args);
} Test;

Test tests[] = {
	{"helloworld",		testHello},
	{"memory", 		testMemory},
	{"compute", 		testCompute},
	{"interconnect", 	testInterconnect},
    };


int 
main (int argc, char *argv[]) {
    int rank, size;
    signed char c;
    char testname[32];
    char testoptions[200];
    FILE *outfile = fopen (OUTPUTFILE, "a");

    if (outfile == NULL) {
        perror ("file open");
    }
    assert (outfile);

    testname[0] = testoptions[0] = '\0';
    while ((c = getopt(argc, argv, "dt:o:")) != -1) {
        switch (c) {
	    case 'd':
 		outfile = stderr;
		break;
	    case 't':
		strncpy (testname, optarg, sizeof(testname) - 1);
		testname[sizeof(testname)-1] = '\0';
		break;
	    case 'o':
		strncpy (testoptions, optarg, sizeof(testoptions) - 1);
		testoptions[sizeof(testoptions)-1] = '\0';
		assert ((strlen (testoptions) < (sizeof(testoptions) - 1)) && "Test arguments too long, increase size of testoptions");
		break;
	    default:
	        fprintf (outfile, "Must provide test name\n");
		exit (-1);
	}
    }
    InitUtilsOutput (outfile);

    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);

    for (int i = 0; i < sizeof(tests)/sizeof(Test); i++)
	if (0 == strcmp (testname, tests[i].name)) {
	    MPI_Barrier (MPI_COMM_WORLD);
	    tests[i].func (outfile, rank, size, testoptions);
	    break;
	}

    fclose (outfile);
    MPI_Finalize ();
    return 0;
}
