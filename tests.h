#ifndef __TESTS_H__
#define __TESTS_H__

void testHello (int taskid, int numtasks, char *testargs);

void testMemory (int taskid, int numtasks, char *testargs);
int HPCC_Stream(int vectorsize, int doIO, double *copyGBs, double *scaleGBs, double *addGBs, double *triadGBs, int *failure);

void testCompute (int taskid, int numtasks, char *testargs);

#endif /* __TESTS_H__ */
