/*
 * File: objectTracker_emxutil.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 17-Jun-2015 12:18:42
 */

#ifndef __OBJECTTRACKER_EMXUTIL_H__
#define __OBJECTTRACKER_EMXUTIL_H__

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "objectTracker_types.h"

/* Function Declarations */
extern void b_emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);
extern void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
  elementSize);
extern void emxFree_real_T(emxArray_real_T **pEmxArray);
extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif

/*
 * File trailer for objectTracker_emxutil.h
 *
 * [EOF]
 */
