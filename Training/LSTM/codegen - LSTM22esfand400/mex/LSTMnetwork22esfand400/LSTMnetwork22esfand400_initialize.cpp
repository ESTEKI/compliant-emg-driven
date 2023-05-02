//
// LSTMnetwork22esfand400_initialize.cpp
//
// Code generation for function 'LSTMnetwork22esfand400_initialize'
//

// Include files
#include "LSTMnetwork22esfand400_initialize.h"
#include "LSTMnetwork22esfand400.h"
#include "LSTMnetwork22esfand400_data.h"
#include "_coder_LSTMnetwork22esfand400_mex.h"
#include "rt_nonfinite.h"

// Variable Definitions
static const volatile char_T *emlrtBreakCheckR2012bFlagVar{nullptr};

// Function Declarations
static void LSTMnetwork22esfand400_once();

// Function Definitions
static void LSTMnetwork22esfand400_once()
{
  mex_InitInfAndNan();
  // Allocate instance data
  covrtAllocateInstanceData(&emlrtCoverageInstance);
  // Initialize Coverage Information
  covrtScriptInit(
      &emlrtCoverageInstance,
      "E:\\projects\\New projects\\myo armband\\LSTM\\LSTMnetwork22esfand400.m",
      0U, 1U, 2U, 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U);
  // Initialize Function Information
  covrtFcnInit(&emlrtCoverageInstance, 0U, 0U, "LSTMnetwork22esfand400", 0, -1,
               179);
  // Initialize Basic Block Information
  covrtBasicBlockInit(&emlrtCoverageInstance, 0U, 1U, 154, -1, 175);
  covrtBasicBlockInit(&emlrtCoverageInstance, 0U, 0U, 80, -1, 148);
  // Initialize If Information
  covrtIfInit(&emlrtCoverageInstance, 0U, 0U, 58, 75, -1, 152);
  // Initialize MCDC Information
  // Initialize For Information
  // Initialize While Information
  // Initialize Switch Information
  // Start callback for coverage engine
  covrtScriptStart(&emlrtCoverageInstance, 0U);
  LSTMnetwork22esfand400_init();
}

void LSTMnetwork22esfand400_initialize()
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
    LSTMnetwork22esfand400_once();
  }
}

// End of code generation (LSTMnetwork22esfand400_initialize.cpp)
