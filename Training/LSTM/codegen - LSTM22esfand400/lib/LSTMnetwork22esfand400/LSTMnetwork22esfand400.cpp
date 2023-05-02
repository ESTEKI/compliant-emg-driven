//
// File: LSTMnetwork22esfand400.cpp
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 13-Mar-2022 15:03:25
//

// Include Files
#include "LSTMnetwork22esfand400.h"
#include "DeepLearningNetwork.h"
#include "LSTMnetwork22esfand400_data.h"
#include "LSTMnetwork22esfand400_initialize.h"
#include "LSTMnetwork22esfand400_internal_types.h"
#include "predict.h"

// Variable Definitions
static boolean_T mynet_not_empty;

// Function Definitions
//
// Arguments    : const double x[3]
// Return Type  : float
//
float LSTMnetwork22esfand400(const double x[3])
{
  static LSTMnetwork22esfand4000_0 mynet;
  if (!isInitialized_LSTMnetwork22esfand400) {
    LSTMnetwork22esfand400_initialize();
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
void LSTMnetwork22esfand400_init()
{
  mynet_not_empty = false;
}

//
// File trailer for LSTMnetwork22esfand400.cpp
//
// [EOF]
//
