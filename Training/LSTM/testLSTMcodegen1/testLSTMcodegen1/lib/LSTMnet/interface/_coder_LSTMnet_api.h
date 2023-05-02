//
// File: _coder_LSTMnet_api.h
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 26-Sep-2021 16:49:07
//

#ifndef _CODER_LSTMNET_API_H
#define _CODER_LSTMNET_API_H

// Include Files
#include "emlrt.h"
#include "tmwtypes.h"
#include <algorithm>
#include <cstring>

// Variable Declarations
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

// Function Declarations
real32_T LSTMnet(real_T x[3]);

void LSTMnet_api(const mxArray *prhs, const mxArray **plhs);

void LSTMnet_atexit();

void LSTMnet_initialize();

void LSTMnet_terminate();

void LSTMnet_xil_shutdown();

void LSTMnet_xil_terminate();

#endif
//
// File trailer for _coder_LSTMnet_api.h
//
// [EOF]
//
