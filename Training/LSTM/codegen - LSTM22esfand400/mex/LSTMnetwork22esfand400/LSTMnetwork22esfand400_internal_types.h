//
// LSTMnetwork22esfand400_internal_types.h
//
// Code generation for function 'LSTMnetwork22esfand400'
//

#pragma once

// Include files
#include "LSTMnetwork22esfand400_types.h"
#include "rtwtypes.h"
#include "MWCNNLayer.hpp"
#include "MWTargetNetworkImpl.hpp"
#include "MWTensorBase.hpp"
#include "emlrt.h"

// Type Definitions
class LSTMnetwork22esfand4000_0 {
public:
  LSTMnetwork22esfand4000_0();
  void setSize(int32_T seqLength);
  void resetState();
  void updateState();
  void setup();
  void predict();
  void cleanup();
  real32_T *getLayerOutput(int32_T layerIndex, int32_T portIndex);
  int32_T getLayerOutputSize(int32_T layerIndex, int32_T portIndex);
  real32_T *getInputDataPointer(int32_T b_index);
  real32_T *getInputDataPointer();
  real32_T *getOutputDataPointer(int32_T b_index);
  real32_T *getOutputDataPointer();
  int32_T getBatchSize();
  ~LSTMnetwork22esfand4000_0();

private:
  void allocate();
  void postsetup();
  void deallocate();
  int32_T maxSequenceLength;
  int32_T numLayers;
  boolean_T isInitialized;
  MWTensorBase *inputTensors[1];
  MWTensorBase *outputTensors[1];
  MWCNNLayer *layers[4];
  MWTargetNetworkImpl *targetImpl;
};

// End of code generation (LSTMnetwork22esfand400_internal_types.h)
