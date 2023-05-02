/* Copyright 2018 The MathWorks, Inc. */

#ifndef _MW_RNN_LAYER_
#define _MW_RNN_LAYER_

#include "MWCNNLayer.hpp"

class MWTargetNetworkImpl;
class MWTensorBase;

/**
 * Codegen class for RNN layer
 */
class MWTargetNetworkImpl;

enum RNNMode { LSTM = 0, GRU };
enum RNNBiasMode { SINGLE_INPUT_BIAS = 0, DOUBLE_BIAS };
enum GRUMultiplicationMode { AFTER = 0, BEFORE = 1 };
enum StateActEnum { TANH = 0, SOFTSIGN };
enum GateActEnum { SIGMOID = 0, HARD_SIGMOID };
static StateActEnum strtoStateAct(const std::string&);
static GateActEnum strtoGateAct(const std::string&);

class MWRNNLayer : public MWCNNLayer {
  public:
    MWRNNLayer() {
    }
    ~MWRNNLayer() {
    }
    void createRNNLayer(MWTargetNetworkImpl*,
                        MWTensorBase*,
                        int,
                        int,
                        bool,
                        bool,
                        int,
                        int,
                        int,
                        const char*,
                        const char*,
                        int,
                        const char*,
                        const char*,
                        const char*,
                        const char*,
                        int);
    void propagateSize();
    void resetState();
    void updateState();
    int getNumGates() const;

  public:
    int hiddenSize;
    bool lastMode;
    bool isBidirectional;
    RNNMode rnnMode;
    RNNBiasMode rnnBiasMode;
    GRUMultiplicationMode gruMultiplicationMode;
    int numFeatures;
    int numLayers;
    StateActEnum stateActivationFunc;
    GateActEnum gateActivationFunc;
};

#endif
