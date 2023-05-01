//
// File: predict.cpp
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 31-Aug-2022 14:02:02
//
#include "stdafx.h"
// Include Files
#include "predict.h"
#include "DeepLearningNetwork.h"
#include "LSTMnet_DatasetF_internal_types.h"
#include <cstring>

// Function Definitions
//
// Arguments    : LSTMnet_DatasetF0_0 *obj
//                const double varargin_1[3]
// Return Type  : float
//
namespace coder {
float DeepLearningNetwork_predict(LSTMnet_DatasetF0_0 *obj,
                                  const double varargin_1[3])
{
  float miniBatch[3];
  float varargout_1;
  miniBatch[0] = static_cast<float>(varargin_1[0]);
  miniBatch[1] = static_cast<float>(varargin_1[1]);
  miniBatch[2] = static_cast<float>(varargin_1[2]);
  obj->setSize(1);
  memcpy(obj->getInputDataPointer(0), miniBatch, obj->getLayerOutputSize(0, 0));
  obj->predict();
  memcpy(&varargout_1, obj->getLayerOutput(3, 0),
         obj->getLayerOutputSize(3, 0));
  return varargout_1;
}

} // namespace coder

//
// File trailer for predict.cpp
//
// [EOF]
//
