//
// predict.h
//
// Code generation for function 'predict'
//

#pragma once

// Include files
#include "rtwtypes.h"
#include "covrt.h"
#include "emlrt.h"
#include "mex.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Type Declarations
class LSTMfinal1network0_0;

// Function Declarations
namespace coder {
real32_T DeepLearningNetwork_predict(const emlrtStack *sp,
                                     LSTMfinal1network0_0 *obj,
                                     const real_T varargin_1[3]);

}

// End of code generation (predict.h)
