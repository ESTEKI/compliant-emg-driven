//
// LSTMnet.cpp
//
// Code generation for function 'LSTMnet'
//

// Include files
#include "LSTMnet.h"
#include "DeepLearningNetwork.h"
#include "LSTMnet_data.h"
#include "LSTMnet_internal_types.h"
#include "predict.h"
#include "rt_nonfinite.h"

// Variable Definitions
static net2lstm_miladch235_netWork0_0 mynet;

static boolean_T mynet_not_empty;

static emlrtRSInfo emlrtRSI{
    7,                                                         // lineNo
    "LSTMnet",                                                 // fcnName
    "E:\\projects\\New projects\\myo armband\\LSTM\\LSTMnet.m" // pathName
};

static emlrtRSInfo b_emlrtRSI{
    5,                                                         // lineNo
    "LSTMnet",                                                 // fcnName
    "E:\\projects\\New projects\\myo armband\\LSTM\\LSTMnet.m" // pathName
};

static emlrtRSInfo c_emlrtRSI{
    69,                        // lineNo
    "loadDeepLearningNetwork", // fcnName
    "D:\\Programs\\MATLAB21\\Polyspace\\toolbox\\shared\\coder\\coder\\lib\\+"
    "coder\\loadDeepLearningNetwork.m" // pathName
};

static emlrtRSInfo d_emlrtRSI{
    1,                         // lineNo
    "loadDeepLearningNetwork", // fcnName
    "D:\\Programs\\MATLAB21\\Polyspace\\toolbox\\shared\\dlcoder_base\\dlcoder_"
    "base\\+coder\\+internal\\loadDeepLearningNetwork.p" // pathName
};

static emlrtRSInfo e_emlrtRSI{
    1,                                 // lineNo
    "DeepLearningNetwork/callSetSize", // fcnName
    "C:"
    "\\ProgramData\\MATLAB\\SupportPackages\\R2021a\\toolbox\\shared\\dlcoder_"
    "base\\supportpackages\\shared_dl_targets\\+coder\\@DeepLearningN"
    "etwork\\DeepLearningNetwork.p" // pathName
};

// Function Definitions
real32_T LSTMnet(const emlrtStack *sp, const real_T x[3])
{
  net2lstm_miladch235_netWork0_0 *iobj_0;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack st;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  d_st.prev = &c_st;
  d_st.tls = c_st.tls;
  covrtLogFcn(&emlrtCoverageInstance, 0, 0);
  if (covrtLogIf(&emlrtCoverageInstance, 0, 0, 0, !mynet_not_empty)) {
    covrtLogBasicBlock(&emlrtCoverageInstance, 0, 0);
    st.site = &b_emlrtRSI;
    iobj_0 = &mynet;
    b_st.site = &c_emlrtRSI;
    c_st.site = &d_emlrtRSI;
    d_st.site = &e_emlrtRSI;
    coder::DeepLearningNetwork_setup(&d_st, iobj_0);
    mynet_not_empty = true;
  }
  covrtLogBasicBlock(&emlrtCoverageInstance, 0, 1);
  st.site = &emlrtRSI;
  return coder::DeepLearningNetwork_predict(&st, &mynet, x);
}

void LSTMnet_init()
{
  mynet_not_empty = false;
}

// End of code generation (LSTMnet.cpp)
