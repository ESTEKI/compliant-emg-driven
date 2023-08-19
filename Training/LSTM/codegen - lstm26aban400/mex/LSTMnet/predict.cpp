//
// predict.cpp
//
// Code generation for function 'predict'
//

// Include files
#include "predict.h"
#include "DeepLearningNetwork.h"
#include "LSTMnet_data.h"
#include "LSTMnet_internal_types.h"
#include "LSTMnet_mexutil.h"
#include "rt_nonfinite.h"
#include "MWDeepLearningUtils.hpp"
#include <cstring>

// Variable Definitions
static emlrtRSInfo f_emlrtRSI{
    1,                             // lineNo
    "DeepLearningNetwork/predict", // fcnName
    "C:"
    "\\ProgramData\\MATLAB\\SupportPackages\\R2021a\\toolbox\\shared\\dlcoder_"
    "base\\supportpackages\\shared_dl_targets\\+coder\\@DeepLearningN"
    "etwork\\predict.p" // pathName
};

static emlrtRSInfo g_emlrtRSI{
    1,                                   // lineNo
    "DeepLearningNetwork/predictForRNN", // fcnName
    "C:"
    "\\ProgramData\\MATLAB\\SupportPackages\\R2021a\\toolbox\\shared\\dlcoder_"
    "base\\supportpackages\\shared_dl_targets\\+coder\\@DeepLearningN"
    "etwork\\predictForRNN.p" // pathName
};

// Function Definitions
namespace coder {
real32_T DeepLearningNetwork_predict(const emlrtStack *sp,
                                     net2lstm_miladch235_netWork0_0 *obj,
                                     const real_T varargin_1[3])
{
  emlrtStack b_st;
  emlrtStack st;
  real32_T miniBatch[3];
  real32_T varargout_1;
  st.prev = sp;
  st.tls = sp->tls;
  st.site = &f_emlrtRSI;
  b_st.prev = &st;
  b_st.tls = st.tls;
  miniBatch[0] = static_cast<real32_T>(varargin_1[0]);
  miniBatch[1] = static_cast<real32_T>(varargin_1[1]);
  miniBatch[2] = static_cast<real32_T>(varargin_1[2]);
  obj->setSize(1);
  b_st.site = &g_emlrtRSI;
  memcpy(obj->getInputDataPointer(0), miniBatch, obj->getLayerOutputSize(0, 0));
  TRY_DL_ERROR_CHECK();

  obj->predict();
  CATCH_DL_ERROR_CHECK();

  checkRunTimeError(&b_st, err.what(), __FILE__, __LINE__);
  LEAVE_DL_ERROR_CHECK();

  memcpy(&varargout_1, obj->getLayerOutput(3, 0),
         obj->getLayerOutputSize(3, 0));
  return varargout_1;
}

} // namespace coder

// End of code generation (predict.cpp)
