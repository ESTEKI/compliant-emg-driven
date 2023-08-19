//
// File: LSTMnet_DatasetF_internal_types.h
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 31-Aug-2022 14:02:02
//

#ifndef LSTMNET_DATASETF_INTERNAL_TYPES_H
#define LSTMNET_DATASETF_INTERNAL_TYPES_H

// Include Files
#include "LSTMnet_DatasetF_types.h"
#include "rtwtypes.h"
#include "MWCNNLayer.hpp"
#include "MWTargetNetworkImpl.hpp"
#include "MWTensorBase.hpp"

// Type Definitions
class LSTMnet_DatasetF0_0 {
public:
  LSTMnet_DatasetF0_0();
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
  ~LSTMnet_DatasetF0_0();

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
// File trailer for LSTMnet_DatasetF_internal_types.h
//
// [EOF]
//
