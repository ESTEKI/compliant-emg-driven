//
// File: DeepLearningNetwork.cpp
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 23-Oct-2021 18:00:09
//

// Include Files
#include "DeepLearningNetwork.h"
#include "LSTMfinal1network_internal_types.h"
#include "MWCNNLayer.hpp"
#include "MWFCLayer.hpp"
#include "MWOutputLayer.hpp"
#include "MWRNNLayer.hpp"
#include "MWSequenceInputLayer.hpp"
#include "MWTargetNetworkImpl.hpp"
#include "MWTensor.hpp"
#include "MWTensorBase.hpp"

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void LSTMfinal1network0_0::allocate()
{
  this->targetImpl->allocate(2, this->layers, this->numLayers, 0);
  for (int idx{0}; idx < 4; idx++) {
    this->layers[idx]->allocate();
  }
  (static_cast<MWTensor<float> *>(this->inputTensors[0]))
      ->setData(this->layers[0]->getLayerOutput(0));
}

//
// Arguments    : void
// Return Type  : void
//
void LSTMfinal1network0_0::cleanup()
{
  this->deallocate();
  for (int idx{0}; idx < 4; idx++) {
    this->layers[idx]->cleanup();
  }
  if (this->targetImpl) {
    this->targetImpl->cleanup();
  }
}

//
// Arguments    : void
// Return Type  : void
//
void LSTMfinal1network0_0::deallocate()
{
  this->targetImpl->deallocate();
  for (int idx{0}; idx < 4; idx++) {
    this->layers[idx]->deallocate();
  }
}

//
// Arguments    : void
// Return Type  : void
//
void LSTMfinal1network0_0::postsetup()
{
  this->targetImpl->postSetup();
}

//
// Arguments    : void
// Return Type  : void
//
void LSTMfinal1network0_0::resetState()
{
  (dynamic_cast<MWRNNLayer *>(this->layers[1]))->resetState();
}

//
// Arguments    : void
// Return Type  : void
//
void LSTMfinal1network0_0::setup()
{
  if (this->isInitialized) {
    this->resetState();
  } else {
    this->isInitialized = true;
    this->targetImpl->preSetup();
    (static_cast<MWSequenceInputLayer *>(this->layers[0]))
        ->createSequenceInputLayer(this->targetImpl, this->inputTensors[0],
                                   false, 0);
    (static_cast<MWRNNLayer *>(this->layers[1]))
        ->createRNNLayer(this->targetImpl, this->layers[0]->getOutputTensor(0),
                         3, 10, false, false, 0, 0, 0, "tanh", "sigmoid", 1,
                         "./codegen/lib/LSTMfinal1network/"
                         "cnn_LSTMfinal1network0_0_lstm_w.bin",
                         "./codegen/lib/LSTMfinal1network/"
                         "cnn_LSTMfinal1network0_0_lstm_b.bin",
                         "./codegen/lib/LSTMfinal1network/"
                         "cnn_LSTMfinal1network0_0_lstm_hx.bin",
                         "./codegen/lib/LSTMfinal1network/"
                         "cnn_LSTMfinal1network0_0_lstm_cx.bin",
                         1);
    (static_cast<MWFCLayer *>(this->layers[2]))
        ->createFCLayer(
            this->targetImpl, this->layers[1]->getOutputTensor(0), 10, 1,
            "./codegen/lib/LSTMfinal1network/cnn_LSTMfinal1network0_0_fc_w.bin",
            "./codegen/lib/LSTMfinal1network/cnn_LSTMfinal1network0_0_fc_b.bin",
            0);
    (static_cast<MWOutputLayer *>(this->layers[3]))
        ->createOutputLayer(this->targetImpl,
                            this->layers[2]->getOutputTensor(0), -1);
    this->outputTensors[0] = this->layers[3]->getOutputTensor(0);
  }
}

//
// Arguments    : void
// Return Type  : ::LSTMfinal1network0_0
//
LSTMfinal1network0_0::LSTMfinal1network0_0()
{
  this->numLayers = 4;
  this->isInitialized = false;
  this->targetImpl = 0;
  this->layers[0] = new MWSequenceInputLayer;
  this->layers[0]->setName("sequenceinput");
  this->layers[1] = new MWRNNLayer;
  this->layers[1]->setName("lstm");
  this->layers[2] = new MWFCLayer;
  this->layers[2]->setName("fc");
  this->layers[3] = new MWOutputLayer;
  this->layers[3]->setName("regressionoutput");
  this->layers[3]->setInPlaceIndex(0, 0);
  this->targetImpl = new MWTargetNetworkImpl;
  this->inputTensors[0] = new MWTensor<float>;
  this->inputTensors[0]->setHeight(1);
  this->inputTensors[0]->setWidth(1);
  this->inputTensors[0]->setChannels(3);
  this->inputTensors[0]->setBatchSize(1);
  this->maxSequenceLength = -1;
}

//
// Arguments    : void
// Return Type  : void
//
LSTMfinal1network0_0::~LSTMfinal1network0_0()
{
  this->cleanup();
  for (int idx{0}; idx < 4; idx++) {
    delete this->layers[idx];
  }
  if (this->targetImpl) {
    delete this->targetImpl;
  }
  delete this->inputTensors[0];
}

//
// Arguments    : void
// Return Type  : int
//
int LSTMfinal1network0_0::getBatchSize()
{
  return this->inputTensors[0]->getBatchSize();
}

//
// Arguments    : int b_index
// Return Type  : float *
//
float *LSTMfinal1network0_0::getInputDataPointer(int b_index)
{
  return (static_cast<MWTensor<float> *>(this->inputTensors[b_index]))
      ->getData();
}

//
// Arguments    : void
// Return Type  : float *
//
float *LSTMfinal1network0_0::getInputDataPointer()
{
  return (static_cast<MWTensor<float> *>(this->inputTensors[0]))->getData();
}

//
// Arguments    : int layerIndex
//                int portIndex
// Return Type  : float *
//
float *LSTMfinal1network0_0::getLayerOutput(int layerIndex, int portIndex)
{
  return this->targetImpl->getLayerOutput(this->layers, layerIndex, portIndex);
}

//
// Arguments    : int layerIndex
//                int portIndex
// Return Type  : int
//
int LSTMfinal1network0_0::getLayerOutputSize(int layerIndex, int portIndex)
{
  return this->layers[layerIndex]
             ->getOutputTensor(portIndex)
             ->getNumElements() *
         sizeof(float);
}

//
// Arguments    : int b_index
// Return Type  : float *
//
float *LSTMfinal1network0_0::getOutputDataPointer(int b_index)
{
  return (static_cast<MWTensor<float> *>(this->outputTensors[b_index]))
      ->getData();
}

//
// Arguments    : void
// Return Type  : float *
//
float *LSTMfinal1network0_0::getOutputDataPointer()
{
  return (static_cast<MWTensor<float> *>(this->outputTensors[0]))->getData();
}

//
// Arguments    : void
// Return Type  : void
//
void LSTMfinal1network0_0::predict()
{
  for (int idx{0}; idx < 4; idx++) {
    this->layers[idx]->predict();
  }
}

//
// Arguments    : int seqLength
// Return Type  : void
//
void LSTMfinal1network0_0::setSize(int seqLength)
{
  if (seqLength != this->inputTensors[0]->getSequenceLength()) {
    this->inputTensors[0]->setSequenceLength(seqLength);
    for (int idx{0}; idx < 4; idx++) {
      this->layers[idx]->propagateSize();
    }
    if (this->maxSequenceLength != -1) {
      this->deallocate();
    }
    this->maxSequenceLength = seqLength;
    this->allocate();
    this->postsetup();
  }
}

//
// Arguments    : void
// Return Type  : void
//
void LSTMfinal1network0_0::updateState()
{
  (dynamic_cast<MWRNNLayer *>(this->layers[1]))->updateState();
}

//
// Arguments    : LSTMfinal1network0_0 *obj
// Return Type  : void
//
namespace coder {
void DeepLearningNetwork_setup(LSTMfinal1network0_0 *obj)
{
  obj->setup();
}

} // namespace coder

//
// File trailer for DeepLearningNetwork.cpp
//
// [EOF]
//
