//
// File: LSTMnetwork17esfand400.cpp
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 08-Mar-2022 13:02:09
//

// Include Files
#include "LSTMnetwork17esfand400.h"
#include "DeepLearningNetwork.h"
#include "LSTMnetwork17esfand400_data.h"
#include "LSTMnetwork17esfand400_initialize.h"
#include "LSTMnetwork17esfand400_internal_types.h"
#include "predict.h"

// Variable Definitions
static boolean_T mynet_not_empty;

// Function Definitions
//
// Arguments    : const double x[3]
// Return Type  : float
//
float LSTMnetwork17esfand400(const double x[3])
{
  static LSTMnetwork17esfand4000_0 mynet;
  if (!isInitialized_LSTMnetwork17esfand400) {
    LSTMnetwork17esfand400_initialize();
  }
  if (!mynet_not_empty) {
    coder::DeepLearningNetwork_setup(&mynet);
    mynet_not_empty = true;
  }
  return coder::DeepLearningNetwork_predict(&mynet, x);
}

//
// Arguments    : void
// Return Type  : void
//
void LSTMnetwork17esfand400_init()
{
  mynet_not_empty = false;
}

//
// File trailer for LSTMnetwork17esfand400.cpp
//
// [EOF]
//
