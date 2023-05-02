/* Copyright 2020 The MathWorks, Inc. */

#ifndef _MW_CONV_LAYER_API_IMPL
#define _MW_CONV_LAYER_API_IMPL

#include "MWCNNLayerImpl.hpp"
#include "dnnl.hpp"
#include <memory>

class MWCNNLayer;
class MWTargetNetworkImpl;

// LSTM Layer
class MWRNNLayerImpl : public MWCNNLayerImpl {
  public:
    MWRNNLayerImpl(MWCNNLayer* layer,
                    MWTargetNetworkImpl* ntwk_impl,
                    int hljcfGWsvZXJZNrImpJB,
                    int UpnEytIWGokwbTFkBcSx,
                    bool m_last_mode,
                    const char* zzWugmJRYlNEuAzHMpeQ,
                    const char* OVOphSOolqRQDDoKPwxy,
                    const char* XCLDbxHBtWRStETWIkId,
                    const char* WprSrhAStKGxyXeoxETy);

    ~MWRNNLayerImpl() {
    }

    void predict();
    void propagateSize();
    void resetState();
    void allocate();
    void updateState();

  private:
    int fxxCPKTclxXPxrdMAkwi;
    int gzSTokDHvkXefhiGDcWL;
    bool bUVPfnrJhLfHzOLUUrKk;

    // buffers to hold initial unexpanded states
    std::shared_ptr<float> QwUuNuQNtlPXrIwRNiSZ;
    std::shared_ptr<float> UdmcwaUkepxfZrpdpcAN;

    // buffers to hold batch expanded initial states
    std::shared_ptr<float> QjgQHaUACFNSteMrRtRj;
    std::shared_ptr<float> UKtMXCCqdjeyaVHabkxg;

    float* UzaGmBLFEwmwaFXebUma;
    float* RAtlBpdedvgxUsgDTsch;

    float* VCbcPxtPsBLTrHYdEvqn;
    float* REXdEoRjxuQJkqgIDihy;

    // Buffers to store the input and output states. In subsequence calls to predictAndUpdateState,
    // these buffers will be used in a ping-pong mechanism to avoid extraneous copies.
    std::shared_ptr<float> TxNFOfYScyqGlEFFxbAv;
    std::shared_ptr<float> QVgVGfoCXYiYXzPhvVPX;
    std::shared_ptr<float> UEESbUvbMihFnquvuFij;
    std::shared_ptr<float> QhTesEEIHwhNmHSeYbRR;

    std::shared_ptr<float> ZinudJuZuGitiNTsJpBR;
    std::shared_ptr<float> omxlPZbBePZdWaJOBUUG;
    std::shared_ptr<float> ONvcEjLBnVNUdjMKOAwF;

    // Variable to store previous batch size. This is to enable us to avoid setting performing
    // batchSize-dependent tasks while we do not yet support variable runtime batchSize.
    int NldNILHvuQqQPSAHXxdT;

    void loadWeights(const char*);
    void loadBias(const char*);
    void loadInitialHiddenState(const char*);
    void loadInitialCellState(const char*);
    void prepareInitialState(float*, float*);
    void allocateInitialStates();
    void resetBackwardStates();
    void setStatePrimitives();

    void createInputMemdesc();
    void createOutputMemdesc();

    // LSTM Layer related config params
    std::shared_ptr<dnnl::memory> ipWeights;
    std::shared_ptr<dnnl::memory> recWeights;
    std::shared_ptr<dnnl::memory> bias;
    std::shared_ptr<dnnl::memory> hx_memory;
    std::shared_ptr<dnnl::memory> cx_memory;
    std::shared_ptr<dnnl::memory> hy_memory;
    std::shared_ptr<dnnl::memory> cy_memory;
    std::shared_ptr<dnnl::memory> rnn_dst_memory;

    dnnl::memory::desc reorderedIpWeightsDesc;
    dnnl::memory::desc reorderedRecWeightsDesc;

    /*reorder related settings*/
    std::shared_ptr<dnnl::memory> rnn_ipWgt_memory;
    std::shared_ptr<dnnl::memory> rnn_recWgt_memory;

    std::unique_ptr<dnnl::memory::desc> rnn_src_md;
    std::unique_ptr<dnnl::memory::desc> rnn_ipWeights_md;
    std::unique_ptr<dnnl::memory::desc> rnn_recWeights_md;
    std::unique_ptr<dnnl::memory::desc> rnn_bias_md;
    std::unique_ptr<dnnl::memory::desc> rnn_hidStateX_md;
    std::unique_ptr<dnnl::memory::desc> rnn_hidStateY_md;
    std::unique_ptr<dnnl::memory::desc> rnn_cellStateX_md;
    std::unique_ptr<dnnl::memory::desc> rnn_cellStateY_md;
    std::unique_ptr<dnnl::memory::desc> rnn_dst_md;

    std::unique_ptr<dnnl::lstm_forward::desc> rnn_d;
    std::unique_ptr<dnnl::lstm_forward::primitive_desc> rnn_pd;
    std::unique_ptr<dnnl::lstm_forward::primitive> rnn;

    std::unique_ptr<dnnl::lbr_gru_forward::desc> lbr_gru_d;
    std::unique_ptr<dnnl::lbr_gru_forward::primitive_desc> lbr_gru_pd;
    std::unique_ptr<dnnl::lbr_gru_forward::primitive> lbr_gru;
};

#endif
