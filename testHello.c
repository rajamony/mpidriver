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

#include <stdio.h>
#include "tests.h"


void
testHello (int taskid, int numtasks, char *testargs) {
    printf ( "Hello world from process %d of %d\n", taskid, numtasks);
}
