//
// LSTMnetwork22esfand400_terminate.cpp
//
// Code generation for function 'LSTMnetwork22esfand400_terminate'
//

// Include files
#include "LSTMnetwork22esfand400_terminate.h"
#include "LSTMnetwork22esfand400_data.h"
#include "_coder_LSTMnetwork22esfand400_mex.h"
#include "rt_nonfinite.h"

// Function Definitions
void LSTMnetwork22esfand400_atexit()
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

void LSTMnetwork22esfand400_terminate()
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

// End of code generation (LSTMnetwork22esfand400_terminate.cpp)
