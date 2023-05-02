//
// LSTMnet_initialize.cpp
//
// Code generation for function 'LSTMnet_initialize'
//

// Include files
#include "LSTMnet_initialize.h"
#include "LSTMnet.h"
#include "LSTMnet_data.h"
#include "_coder_LSTMnet_mex.h"
#include "rt_nonfinite.h"

// Variable Definitions
static const volatile char_T *emlrtBreakCheckR2012bFlagVar{nullptr};

// Function Declarations
static void LSTMnet_once();

// Function Definitions
static void LSTMnet_once()
{
  mex_InitInfAndNan();
  // Allocate instance data
  covrtAllocateInstanceData(&emlrtCoverageInstance);
  // Initialize Coverage Information
  covrtScriptInit(&emlrtCoverageInstance,
                  "E:\\projects\\New projects\\myo armband\\LSTM\\LSTMnet.m",
                  0U, 1U, 2U, 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U);
  // Initialize Function Information
  covrtFcnInit(&emlrtCoverageInstance, 0U, 0U, "LSTMnet", 0, -1, 169);
  // Initialize Basic Block Information
  covrtBasicBlockInit(&emlrtCoverageInstance, 0U, 1U, 144, -1, 165);
  covrtBasicBlockInit(&emlrtCoverageInstance, 0U, 0U, 65, -1, 138);
  // Initialize If Information
  covrtIfInit(&emlrtCoverageInstance, 0U, 0U, 43, 60, -1, 142);
  // Initialize MCDC Information
  // Initialize For Information
  // Initialize While Information
  // Initialize Switch Information
  // Start callback for coverage engine
  covrtScriptStart(&emlrtCoverageInstance, 0U);
  LSTMnet_init();
}

void LSTMnet_initialize()
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  mexFunctionCreateRootTLS();
  emlrtBreakCheckR2012bFlagVar = emlrtGetBreakCheckFlagAddressR2012b();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, nullptr);
  emlrtEnterRtStackR2012b(&st);
  emlrtLicenseCheckR2012b(&st, (const char_T *)"neural_network_toolbox", 2);
  if (emlrtFirstTimeR2012b(emlrtRootTLSGlobal)) {
    LSTMnet_once();
  }
}

// End of code generation (LSTMnet_initialize.cpp)
