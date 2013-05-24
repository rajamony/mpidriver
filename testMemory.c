#include <wordexp.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "tests.h"


void
testMemory (int taskid, int numtasks, char *options) {
    signed char c;
    wordexp_t wep;
    int vectorsize = 1, numiterations = 1;

    printf ( "Testing memory - this is task %d of %d. Options <%s>\n", taskid, numtasks, options);

    optind = 1;	// rewind getopt
    wordexp ("dummy", &wep, 0);      // Get argv[0] out of the way
    wordexp (options, &wep, WRDE_NOCMD | WRDE_APPEND);

    while ((c = getopt(wep.we_wordc, wep.we_wordv, "n:i:")) != -1) {
        switch (c) {
	    case 'n':
		vectorsize = atoi (optarg); // strtol (optarg, NULL, 0);
		break;
	    case 'i':
		numiterations = strtol (optarg, NULL, 0);
		break;
	    default:
	        fprintf (stderr, "Must provide valid arguments\n");
		exit (-1);
	}
    }
    wordfree (&wep);

    for (int i = 0; i < numiterations; i++) {
	int failure;
	double copyGBs, scaleGBs, addGBs, triadGBs;

	fprintf (stdout, "Iteration %3d (of %d)   Vectorsize: %d\n", i, numiterations, vectorsize);
	HPCC_Stream (vectorsize*1000000, 0, &copyGBs, &scaleGBs, &addGBs, &triadGBs, &failure);
	fprintf (stderr, "Achieved bandwidths of %f %f %f %f\n", copyGBs, scaleGBs, addGBs, triadGBs);
    }
}
