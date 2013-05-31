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

#ifndef __TESTS_H__
#define __TESTS_H__

void testHello (FILE *outfile, int taskid, int numtasks, char *testargs);

void testMemory (FILE *outfile, int taskid, int numtasks, char *testargs);
int HPCC_Stream(int vectorsize, int doIO, double *copyGBs, double *scaleGBs, double *addGBs, double *triadGBs, int *failure);

void testCompute (FILE *outfile, int taskid, int numtasks, char *testargs);

void testInterconnect (FILE *outfile, int taskid, int numtasks, char *testargs);

#endif /* __TESTS_H__ */
