#include <wordexp.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "tests.h"



void
testInterconnect (int taskid, int numtasks, char *options) {
    signed char c;
    wordexp_t wep;
    int numiterations = 1;

    printf ( "Testing interconnect - this is task %d of %d. Options <%s>\n", taskid, numtasks, options);

    optind = 1;	// rewind getopt
    wordexp ("dummy", &wep, 0);      // Get argv[0] out of the way
    wordexp (options, &wep, WRDE_NOCMD | WRDE_APPEND);

    while ((c = getopt(wep.we_wordc, wep.we_wordv, "i:")) != -1) {
        switch (c) {
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
	fprintf (stdout, "Iteration %3d (of %d)\n", i, numiterations);
	// Do something interconnect heavy here
    }
}
