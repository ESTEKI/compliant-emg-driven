//
// _coder_LSTMnetwork22esfand400_mex.cpp
//
// Code generation for function '_coder_LSTMnetwork22esfand400_mex'
//

// Include files
#include "_coder_LSTMnetwork22esfand400_mex.h"
#include "LSTMnetwork22esfand400_data.h"
#include "LSTMnetwork22esfand400_initialize.h"
#include "LSTMnetwork22esfand400_terminate.h"
#include "_coder_LSTMnetwork22esfand400_api.h"
#include "rt_nonfinite.h"
#include <stdexcept>

void emlrtExceptionBridge();
void emlrtExceptionBridge()
{
  throw std::runtime_error("");
}
// Function Definitions
void LSTMnetwork22esfand400_mexFunction(int32_T nlhs, mxArray *plhs[1],
                                        int32_T nrhs, const mxArray *prhs[1])
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  const mxArray *outputs;
  st.tls = emlrtRootTLSGlobal;
  // Check for proper number of arguments.
  if (nrhs != 1) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 1, 4,
                        22, "LSTMnetwork22esfand400");
  }
  if (nlhs > 1) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 22,
                        "LSTMnetwork22esfand400");
  }
  // Call the function.
  LSTMnetwork22esfand400_api(prhs[0], &outputs);
  // Copy over outputs to the caller.
  emlrtReturnArrays(1, &plhs[0], &outputs);
}

void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs,
                 const mxArray *prhs[])
{
  mexAtExit(&LSTMnetwork22esfand400_atexit);
  // Module initialization.
  LSTMnetwork22esfand400_initialize();
  try {
    emlrtShouldCleanupOnError((emlrtCTX *)emlrtRootTLSGlobal, false);
    // Dispatch the entry-point.
    LSTMnetwork22esfand400_mexFunction(nlhs, plhs, nrhs, prhs);
    // Module termination.
    LSTMnetwork22esfand400_terminate();
  } catch (...) {
    emlrtCleanupOnException((emlrtCTX *)emlrtRootTLSGlobal);
    throw;
  }
}

emlrtCTX mexFunctionCreateRootTLS()
{
  emlrtCreateRootTLSR2021a(&emlrtRootTLSGlobal, &emlrtContextGlobal, nullptr, 1,
                           (void *)&emlrtExceptionBridge);
  return emlrtRootTLSGlobal;
}

// End of code generation (_coder_LSTMnetwork22esfand400_mex.cpp)
