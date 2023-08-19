//
// File: LSTMnet_internal_types.h
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 26-Sep-2021 16:49:07
//

#ifndef LSTMNET_INTERNAL_TYPES_H
#define LSTMNET_INTERNAL_TYPES_H

// Include Files
#include "LSTMnet_types.h"
#include "rtwtypes.h"
#include "MWCNNLayer.hpp"
#include "MWTargetNetworkImpl.hpp"
#include "MWTensorBase.hpp"

// Type Definitions
class net2lstm_miladch235_netWork0_0 {
public:
  net2lstm_miladch235_netWork0_0();
  void setSize(int seqLength);
  void resetState();
  void updateState();
  void setup();
  void predict();
  void cleanup();
  float *getLayerOutput(int layerIndex, int portIndex);
  int getLayerOutputSize(int layerIndex, int portIndex);
  float *getInputDataPointer(int b_index);
  float *getInputDataPointer();
  float *getOutputDataPointer(int b_index);
  float *getOutputDataPointer();
  int getBatchSize();
  ~net2lstm_miladch235_netWork0_0();

private:
  void allocate();
  void postsetup();
  void deallocate();
  int maxSequenceLength;
  int numLayers;
  boolean_T isInitialized;
  MWTensorBase *inputTensors[1];
  MWTensorBase *outputTensors[1];
  MWCNNLayer *layers[4];
  MWTargetNetworkImpl *targetImpl;
};

#endif
//
// File trailer for LSTMnet_internal_types.h
//
// [EOF]
//
