//
// File: LSTMnet.cpp
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 26-Sep-2021 16:49:07
//

// Include Files
#include "LSTMnet.h"
#include "DeepLearningNetwork.h"
#include "LSTMnet_data.h"
#include "LSTMnet_initialize.h"
#include "LSTMnet_internal_types.h"
#include "predict.h"

// Variable Definitions
static boolean_T mynet_not_empty;

// Function Definitions
//
// Arguments    : const double x[3]
// Return Type  : float
//
float LSTMnet(const double x[3])
{
  static net2lstm_miladch235_netWork0_0 mynet;
  if (!isInitialized_LSTMnet) {
    LSTMnet_initialize();
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
void LSTMnet_init()
{
  mynet_not_empty = false;
}

//
// File trailer for LSTMnet.cpp
//
// [EOF]
//
