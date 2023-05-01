//
// File: LSTMfinal1network.cpp
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 18-Oct-2021 17:53:04
//

// Include Files
#include "stdafx.h"
#include "LSTMfinal1network.h"
#include "DeepLearningNetwork.h"
#include "LSTMfinal1network_data.h"
#include "LSTMfinal1network_initialize.h"
#include "LSTMfinal1network_internal_types.h"
#include "predict.h"

// Variable Definitions
static boolean_T mynet_not_empty;

// Function Definitions
//
// Arguments    : const double x[3]
// Return Type  : float
//
float LSTMfinal1network(const double x[3])
{
  static LSTMfinal1network0_0 mynet;
  if (!isInitialized_LSTMfinal1network) {
    LSTMfinal1network_initialize();
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
void LSTMfinal1network_init()
{
  mynet_not_empty = false;
}

//
// File trailer for LSTMfinal1network.cpp
//
// [EOF]
//
