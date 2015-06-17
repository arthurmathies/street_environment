/* 
 * File: _coder_objectTracker_api.h 
 *  
 * MATLAB Coder version            : 2.7 
 * C/C++ source code generated on  : 17-Jun-2015 12:18:42 
 */

#ifndef ___CODER_OBJECTTRACKER_API_H__
#define ___CODER_OBJECTTRACKER_API_H__
/* Include Files */ 
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"

/* Type Definitions */ 
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T
struct emxArray_real_T
{
    real_T *data;
    int32_T *size;
    int32_T allocatedSize;
    int32_T numDimensions;
    boolean_T canFreeData;
};
#endif /*struct_emxArray_real_T*/
#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T
typedef struct emxArray_real_T emxArray_real_T;
#endif /*typedef_emxArray_real_T*/

/* Function Declarations */ 
extern void objectTracker_initialize(emlrtContext *aContext);
extern void objectTracker_terminate(void);
extern void objectTracker_atexit(void);
extern void objectTracker_api(const mxArray *prhs[10], const mxArray *plhs[2]);
extern void objectTracker_xil_terminate(void);
extern void objectTracker(int16_T init, emxArray_real_T *laneModel, real_T lanePieceLength, real_T r[3], real_T Pk[9], real_T Q, real_T R, emxArray_real_T *x_measure, emxArray_real_T *y_measure, real_T delta_x);

#endif
/* 
 * File trailer for _coder_objectTracker_api.h 
 *  
 * [EOF] 
 */
