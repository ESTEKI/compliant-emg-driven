//
// File: LSTMnet_DatasetF.cpp
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 31-Aug-2022 14:02:02
//
#include "stdafx.h"
// Include Files
#include "LSTMnet_DatasetF.h"
#include "DeepLearningNetwork.h"
#include "LSTMnet_DatasetF_data.h"
#include "LSTMnet_DatasetF_initialize.h"
#include "LSTMnet_DatasetF_internal_types.h"
#include "predict.h"

// Variable Definitions
static boolean_T mynet_not_empty;

// Function Definitions
//
// Arguments    : const double x[3]
// Return Type  : float
//
float LSTMnet_DatasetF(const double x[3])
{
  static LSTMnet_DatasetF0_0 mynet;
  if (!isInitialized_LSTMnet_DatasetF) {
    LSTMnet_DatasetF_initialize();
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
void LSTMnet_DatasetF_init()
{
  mynet_not_empty = false;
}

//
// File trailer for LSTMnet_DatasetF.cpp
//
// [EOF]
//
