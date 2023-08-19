//
// File: _coder_LSTMfinal1network_api.h
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 18-Oct-2021 17:53:04
//

#ifndef _CODER_LSTMFINAL1NETWORK_API_H
#define _CODER_LSTMFINAL1NETWORK_API_H

// Include Files
#include "emlrt.h"
#include "tmwtypes.h"
#include <algorithm>
#include <cstring>

// Variable Declarations
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

// Function Declarations
real32_T LSTMfinal1network(real_T x[3]);

void LSTMfinal1network_api(const mxArray *prhs, const mxArray **plhs);

void LSTMfinal1network_atexit();

void LSTMfinal1network_initialize();

void LSTMfinal1network_terminate();

void LSTMfinal1network_xil_shutdown();

void LSTMfinal1network_xil_terminate();

#endif
//
// File trailer for _coder_LSTMfinal1network_api.h
//
// [EOF]
//
