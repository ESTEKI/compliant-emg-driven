//
// LSTMnet_mexutil.cpp
//
// Code generation for function 'LSTMnet_mexutil'
//

// Include files
#include "LSTMnet_mexutil.h"
#include "LSTMnet_data.h"
#include "rt_nonfinite.h"
#include <cstdlib>
#include <cstring>

// Function Definitions
void checkRunTimeError(const emlrtStack *sp, const char_T *errMsg,
                       const char_T *file, uint32_T line)
{
  emlrtRTEInfo rtInfo;
  uint32_T errMsgLen;
  rtInfo = createEmlrtInfoStruct(file, line);
  errMsgLen = (uint32_T)strlen(errMsg);
  emlrtErrorWithMessageIdR2018a(sp, &rtInfo, "dnn_core:cnncodegen:RunTimeError",
                                "dnn_core:cnncodegen:RunTimeError", 3,
                                mxCHAR_CLASS, errMsgLen, errMsg);
}

emlrtRTEInfo createEmlrtInfoStruct(const char_T *file, uint32_T line)
{
  emlrtRTEInfo b_rtInfo;
  uint32_T len;
  char_T *brk;
  char_T *fn;
  char_T *pn;
  len = (uint32_T)strlen(file);
  pn = (char_T *)calloc(len + 1U, 1U);
  fn = (char_T *)calloc(len + 1U, 1U);
  memcpy(pn, file, len);
  memcpy(fn, file, len);
  brk = strrchr(fn, '.');
  *brk = '\x00';
  brk = strrchr(fn, '/');
  if (brk == nullptr) {
    brk = strrchr(fn, '\\');
  }
  if (brk == nullptr) {
    brk = fn;
  } else {
    brk++;
  }
  b_rtInfo.lineNo = static_cast<int32_T>(line);
  b_rtInfo.colNo = 0;
  b_rtInfo.fName = brk;
  b_rtInfo.pName = pn;
  return b_rtInfo;
}

// End of code generation (LSTMnet_mexutil.cpp)
