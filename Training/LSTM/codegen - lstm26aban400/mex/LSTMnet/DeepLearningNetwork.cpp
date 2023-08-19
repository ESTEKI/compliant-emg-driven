//
// DeepLearningNetwork.cpp
//
// Code generation for function 'DeepLearningNetwork'
//

// Include files
#include "DeepLearningNetwork.h"
#include "LSTMnet_data.h"
#include "LSTMnet_internal_types.h"
#include "LSTMnet_mexutil.h"
#include "rt_nonfinite.h"
#include "MWCNNLayer.hpp"
#include "MWDeepLearningUtils.hpp"
#include "MWFCLayer.hpp"
#include "MWOutputLayer.hpp"
#include "MWRNNLayer.hpp"
#include "MWSequenceInputLayer.hpp"
#include "MWTargetNetworkImpl.hpp"
#include "MWTensor.hpp"
#include "MWTensorBase.hpp"

// Function Definitions
void net2lstm_miladch235_netWork0_0::allocate()
{
  this->targetImpl->allocate(2, this->layers, this->numLayers, 0);
  for (int32_T idx{0}; idx < 4; idx++) {
    this->layers[idx]->allocate();
  }
  (static_cast<MWTensor<real32_T> *>(this->inputTensors[0]))
      ->setData(this->layers[0]->getLayerOutput(0));
}

void net2lstm_miladch235_netWork0_0::cleanup()
{
  this->deallocate();
  for (int32_T idx{0}; idx < 4; idx++) {
    this->layers[idx]->cleanup();
  }
  if (this->targetImpl) {
    this->targetImpl->cleanup();
  }
}

void net2lstm_miladch235_netWork0_0::deallocate()
{
  this->targetImpl->deallocate();
  for (int32_T idx{0}; idx < 4; idx++) {
    this->layers[idx]->deallocate();
  }
}

void net2lstm_miladch235_netWork0_0::postsetup()
{
  this->targetImpl->postSetup();
}

void net2lstm_miladch235_netWork0_0::resetState()
{
  (dynamic_cast<MWRNNLayer *>(this->layers[1]))->resetState();
}

void net2lstm_miladch235_netWork0_0::setup()
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
                         3, 128, false, false, 0, 0, 0, "tanh", "sigmoid", 1,
                         "E:\\projects\\New projects\\myo "
                         "armband\\LSTM\\codegen\\mex\\LSTMnet\\cnn_net2lstm_"
                         "miladch235_netWork0_0_lstm_w.bin",
                         "E:\\projects\\New projects\\myo "
                         "armband\\LSTM\\codegen\\mex\\LSTMnet\\cnn_net2lstm_"
                         "miladch235_netWork0_0_lstm_b.bin",
                         "E:\\projects\\New projects\\myo "
                         "armband\\LSTM\\codegen\\mex\\LSTMnet\\cnn_net2lstm_"
                         "miladch235_netWork0_0_lstm_hx.bin",
                         "E:\\projects\\New projects\\myo "
                         "armband\\LSTM\\codegen\\mex\\LSTMnet\\cnn_net2lstm_"
                         "miladch235_netWork0_0_lstm_cx.bin",
                         1);
    (static_cast<MWFCLayer *>(this->layers[2]))
        ->createFCLayer(this->targetImpl, this->layers[1]->getOutputTensor(0),
                        128, 1,
                        "E:\\projects\\New projects\\myo "
                        "armband\\LSTM\\codegen\\mex\\LSTMnet\\cnn_net2lstm_"
                        "miladch235_netWork0_0_fc_w.bin",
                        "E:\\projects\\New projects\\myo "
                        "armband\\LSTM\\codegen\\mex\\LSTMnet\\cnn_net2lstm_"
                        "miladch235_netWork0_0_fc_b.bin",
                        0);
    (static_cast<MWOutputLayer *>(this->layers[3]))
        ->createOutputLayer(this->targetImpl,
                            this->layers[2]->getOutputTensor(0), -1);
    this->outputTensors[0] = this->layers[3]->getOutputTensor(0);
  }
}

int32_T net2lstm_miladch235_netWork0_0::getBatchSize()
{
  return this->inputTensors[0]->getBatchSize();
}

real32_T *net2lstm_miladch235_netWork0_0::getInputDataPointer(int32_T b_index)
{
  return (static_cast<MWTensor<real32_T> *>(this->inputTensors[b_index]))
      ->getData();
}

real32_T *net2lstm_miladch235_netWork0_0::getInputDataPointer()
{
  return (static_cast<MWTensor<real32_T> *>(this->inputTensors[0]))->getData();
}

real32_T *net2lstm_miladch235_netWork0_0::getLayerOutput(int32_T layerIndex,
                                                         int32_T portIndex)
{
  return this->targetImpl->getLayerOutput(this->layers, layerIndex, portIndex);
}

int32_T net2lstm_miladch235_netWork0_0::getLayerOutputSize(int32_T layerIndex,
                                                           int32_T portIndex)
{
  return this->layers[layerIndex]
             ->getOutputTensor(portIndex)
             ->getNumElements() *
         sizeof(real32_T);
}

real32_T *net2lstm_miladch235_netWork0_0::getOutputDataPointer(int32_T b_index)
{
  return (static_cast<MWTensor<real32_T> *>(this->outputTensors[b_index]))
      ->getData();
}

real32_T *net2lstm_miladch235_netWork0_0::getOutputDataPointer()
{
  return (static_cast<MWTensor<real32_T> *>(this->outputTensors[0]))->getData();
}

net2lstm_miladch235_netWork0_0::net2lstm_miladch235_netWork0_0()
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
  this->inputTensors[0] = new MWTensor<real32_T>;
  this->inputTensors[0]->setHeight(1);
  this->inputTensors[0]->setWidth(1);
  this->inputTensors[0]->setChannels(3);
  this->inputTensors[0]->setBatchSize(1);
  this->maxSequenceLength = -1;
}

net2lstm_miladch235_netWork0_0::~net2lstm_miladch235_netWork0_0()
{
  this->cleanup();
  for (int32_T idx{0}; idx < 4; idx++) {
    delete this->layers[idx];
  }
  if (this->targetImpl) {
    delete this->targetImpl;
  }
  delete this->inputTensors[0];
}

void net2lstm_miladch235_netWork0_0::predict()
{
  for (int32_T idx{0}; idx < 4; idx++) {
    this->layers[idx]->predict();
  }
}

void net2lstm_miladch235_netWork0_0::setSize(int32_T seqLength)
{
  if (seqLength != this->inputTensors[0]->getSequenceLength()) {
    this->inputTensors[0]->setSequenceLength(seqLength);
    for (int32_T idx{0}; idx < 4; idx++) {
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

void net2lstm_miladch235_netWork0_0::updateState()
{
  (dynamic_cast<MWRNNLayer *>(this->layers[1]))->updateState();
}

namespace coder {
void DeepLearningNetwork_setup(const emlrtStack *sp,
                               net2lstm_miladch235_netWork0_0 *obj)
{
  TRY_DL_ERROR_CHECK();

  obj->setup();
  CATCH_DL_ERROR_CHECK();

  checkRunTimeError(sp, err.what(), __FILE__, __LINE__);
  LEAVE_DL_ERROR_CHECK();
}

} // namespace coder

// End of code generation (DeepLearningNetwork.cpp)
