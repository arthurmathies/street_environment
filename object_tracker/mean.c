/*
 * File: mean.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 23-Jun-2015 12:44:02
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "objectTracker.h"
#include "mean.h"

/* Function Definitions */

/*
 * Arguments    : const emxArray_real_T *x
 * Return Type  : double
 */
double mean(const emxArray_real_T *x)
{
  double y;
  int k;
  if (x->size[0] == 0) {
    y = 0.0;
  } else {
    y = x->data[0];
    for (k = 2; k <= x->size[0]; k++) {
      y += x->data[k - 1];
    }
  }

  y /= (double)x->size[0];
  return y;
}

/*
 * File trailer for mean.c
 *
 * [EOF]
 */
