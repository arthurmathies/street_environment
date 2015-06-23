/*
 * File: objectTracker.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 23-Jun-2015 12:44:02
 */

#ifndef __OBJECTTRACKER_H__
#define __OBJECTTRACKER_H__

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "objectTracker_types.h"

/* Function Declarations */
extern void objectTracker(short init, const emxArray_real_T *laneModel, double
  lanePieceLength, double r[4], double Pk[16], double Q, double Rx, double Ry,
  const emxArray_real_T *x_measure, const emxArray_real_T *y_measure, double
  delta_x);

#endif

/*
 * File trailer for objectTracker.h
 *
 * [EOF]
 */
