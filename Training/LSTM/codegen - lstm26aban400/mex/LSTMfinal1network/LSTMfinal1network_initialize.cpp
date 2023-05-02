//
// LSTMfinal1network_initialize.cpp
//
// Code generation for function 'LSTMfinal1network_initialize'
//

// Include files
#include "LSTMfinal1network_initialize.h"
#include "LSTMfinal1network.h"
#include "LSTMfinal1network_data.h"
#include "_coder_LSTMfinal1network_mex.h"
#include "rt_nonfinite.h"

// Variable Definitions
static const volatile char_T *emlrtBreakCheckR2012bFlagVar{nullptr};

// Function Declarations
static void LSTMfinal1network_once();

// Function Definitions
static void LSTMfinal1network_once()
{
  mex_InitInfAndNan();
  // Allocate instance data
  covrtAllocateInstanceData(&emlrtCoverageInstance);
  // Initialize Coverage Information
  covrtScriptInit(
      &emlrtCoverageInstance,
      "E:\\projects\\New projects\\myo armband\\LSTM\\LSTMfinal1network.m", 0U,
      1U, 2U, 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U);
  // Initialize Function Information
  covrtFcnInit(&emlrtCoverageInstance, 0U, 0U, "LSTMfinal1network", 0, -1, 159);
  // Initialize Basic Block Information
  covrtBasicBlockInit(&emlrtCoverageInstance, 0U, 1U, 134, -1, 155);
  covrtBasicBlockInit(&emlrtCoverageInstance, 0U, 0U, 65, -1, 128);
  // Initialize If Information
  covrtIfInit(&emlrtCoverageInstance, 0U, 0U, 43, 60, -1, 132);
  // Initialize MCDC Information
  // Initialize For Information
  // Initialize While Information
  // Initialize Switch Information
  // Start callback for coverage engine
  covrtScriptStart(&emlrtCoverageInstance, 0U);
  LSTMfinal1network_init();
}

void LSTMfinal1network_initialize()
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
    LSTMfinal1network_once();
  }
}

// End of code generation (LSTMfinal1network_initialize.cpp)
