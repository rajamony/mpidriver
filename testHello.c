#include <stdio.h>
#include "tests.h"


void
testHello (int taskid, int numtasks) {
    printf ( "Hello world from process %d of %d\n", taskid, numtasks);
}
