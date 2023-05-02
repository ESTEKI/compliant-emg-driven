//
// File: LSTMnetwork22esfand400_internal_types.h
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 13-Mar-2022 15:03:25
//

#ifndef LSTMNETWORK22ESFAND400_INTERNAL_TYPES_H
#define LSTMNETWORK22ESFAND400_INTERNAL_TYPES_H

// Include Files
#include "LSTMnetwork22esfand400_types.h"
#include "rtwtypes.h"
#include "MWCNNLayer.hpp"
#include "MWTargetNetworkImpl.hpp"
#include "MWTensorBase.hpp"

// Type Definitions
class LSTMnetwork22esfand4000_0 {
public:
  LSTMnetwork22esfand4000_0();
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
  ~LSTMnetwork22esfand4000_0();

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
// File trailer for LSTMnetwork22esfand400_internal_types.h
//
// [EOF]
//
