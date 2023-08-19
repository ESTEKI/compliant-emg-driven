//
// LSTMfinal1network_terminate.cpp
//
// Code generation for function 'LSTMfinal1network_terminate'
//

// Include files
#include "LSTMfinal1network_terminate.h"
#include "LSTMfinal1network_data.h"
#include "_coder_LSTMfinal1network_mex.h"
#include "rt_nonfinite.h"

// Function Definitions
void LSTMfinal1network_atexit()
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  // Free instance data
  covrtFreeInstanceData(&emlrtCoverageInstance);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

void LSTMfinal1network_terminate()
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

// End of code generation (LSTMfinal1network_terminate.cpp)
