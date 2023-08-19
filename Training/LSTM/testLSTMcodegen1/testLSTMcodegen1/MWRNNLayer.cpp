#include "stdafx.h"

#include "stdafx.h"
#include "MWRNNLayer.hpp"
#include "MWRNNLayerImpl.hpp"
#include "MWTensorBase.hpp"
#include "MWCNNLayer.hpp"
#include "MWTargetNetworkImpl.hpp"

void MWRNNLayer::createRNNLayer(MWTargetNetworkImpl* ntwk_impl,
                                MWTensorBase* m_in,
                                int m_num_features,
                                int m_hidden_units,
                                bool m_last_mode,
                                bool m_is_bidirectional,
                                int m_rnnMode,
                                int m_rnnBiasMode,
                                int m_gruMultiplicationMode,
                                const char* m_state_activation_func,
                                const char* m_gate_activation_func,
                                int m_num_layers,
                                const char* m_weights_file,
                                const char* m_bias_file,
                                const char* m_initial_hidden_state_file,
                                const char* m_initial_cell_state_file,
                                int outbufIdx) {
#if defined(MW_TARGET_TYPE_CUDNN) || defined(MW_TARGET_TYPE_MKLDNN) || \
    defined(MW_TARGET_TYPE_ARMNEON)
    setInputTensor(m_in);
    allocateOutputTensor<float>(-1, -1, -1, -1, -1, NULL);

    getOutputTensor(0)->setopBufIndex(outbufIdx);

    hiddenSize = m_hidden_units;
    lastMode = m_last_mode;
    isBidirectional = m_is_bidirectional;

    assert(m_rnnMode < 2);
    rnnMode = static_cast<RNNMode>(m_rnnMode);

    assert(m_rnnBiasMode < 2);
    rnnBiasMode = static_cast<RNNBiasMode>(m_rnnBiasMode);

    gruMultiplicationMode = static_cast<GRUMultiplicationMode>(m_gruMultiplicationMode);
    assert(gruMultiplicationMode == AFTER);

    numFeatures = m_num_features;
    numLayers = m_num_layers;
    stateActivationFunc = strtoStateAct(m_state_activation_func);
    gateActivationFunc = strtoGateAct(m_gate_activation_func);

    if (isBidirectional) {
        assert(rnnMode == LSTM);
    }

    m_impl = new MWRNNLayerImpl(this, ntwk_impl, m_num_features, m_hidden_units, m_last_mode,
                                 m_weights_file, m_bias_file, m_initial_hidden_state_file,
                                 m_initial_cell_state_file);

#else
    setInputTensor(m_in);

    // allocate output tensor
    // height = 1, width = 1, channels = numHiddenUnits, batchSize = batchSize, seqLength =
    // seqLength
    hiddenSize = m_hidden_units;
    lastMode = m_last_mode;
    isBidirectional = m_is_bidirectional;

    assert(m_rnnMode < 2);
    rnnMode = static_cast<RNNMode>(m_rnnMode);

    assert(m_rnnBiasMode < 2);
    rnnBiasMode = static_cast<RNNBiasMode>(m_rnnBiasMode);

    gruMultiplicationMode = static_cast<GRUMultiplicationMode>(m_gruMultiplicationMode);
    assert(gruMultiplicationMode == AFTER);

    numFeatures = m_num_features;
    stateActivationFunc = strtoStateAct(m_state_activation_func);
    gateActivationFunc = strtoGateAct(m_gate_activation_func);

    if (isBidirectional) {
        assert(rnnMode == LSTM);
    }

    int outSequenceLength = m_last_mode ? 1 : getInputTensor()->getSequenceLength();
    allocateOutputTensor<float>(1, 1, isBidirectional ? 2 * hiddenSize : hiddenSize,
                                getInputTensor()->getBatchSize(), outSequenceLength, NULL);

    // will need to lift this assumption for image sequences
    assert(getOutputTensor()->getHeight() == 1 && getOutputTensor()->getWidth() == 1);

    m_impl =
        new MWRNNLayerImpl(this, ntwk_impl, m_last_mode, m_num_layers, m_weights_file, m_bias_file,
                            m_initial_hidden_state_file, m_initial_cell_state_file, outbufIdx);
#endif
}

void MWRNNLayer::propagateSize() {
#if defined(MW_TARGET_TYPE_CUDNN) || defined(MW_TARGET_TYPE_MKLDNN) || \
    defined(MW_TARGET_TYPE_ARMNEON)
    // allocate output tensor
    // height = 1, width = 1, channels = numHiddenUnits, batchSize = batchSize, seqLength =
    // seqLength
    int outSequenceLength = lastMode ? 1 : getInputTensor()->getSequenceLength();
    resizeOutputTensor(1, 1, isBidirectional ? 2 * hiddenSize : hiddenSize,
                       getInputTensor()->getBatchSize(), outSequenceLength);

    // will need to lift this assumption for image sequences
    assert(getOutputTensor()->getHeight() == 1 && getOutputTensor()->getWidth() == 1);

    m_impl->propagateSize();
#endif
}

void MWRNNLayer::resetState() {
#if defined(MW_TARGET_TYPE_CUDNN) || defined(MW_TARGET_TYPE_ARMNEON) || \
    defined(MW_TARGET_TYPE_MKLDNN)
    static_cast<MWRNNLayerImpl*>(m_impl)->resetState();
#endif
}

void MWRNNLayer::updateState() {
#if defined(MW_TARGET_TYPE_CUDNN) || defined(MW_TARGET_TYPE_ARMNEON) || \
    defined(MW_TARGET_TYPE_MKLDNN)
    static_cast<MWRNNLayerImpl*>(m_impl)->updateState();
#endif
}

int MWRNNLayer::getNumGates() const {
    if (rnnMode == LSTM) {
        return 4;
    } else {
        assert(rnnMode == GRU);
        return 3;
    }
}

StateActEnum strtoStateAct(const std::string& actFunc) {
    if (actFunc == "tanh") {
        return TANH;
    } else {
		assert(actFunc == "softsign");
        return SOFTSIGN;
    }
}

GateActEnum strtoGateAct(const std::string& actFunc) {
    if (actFunc == "sigmoid") {
        return SIGMOID;
    } else {
		assert(actFunc == "hard-sigmoid");
        return HARD_SIGMOID;
    }
}
