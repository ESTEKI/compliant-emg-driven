/* Copyright 2020 The MathWorks, Inc. */

#ifndef _MW_MKLDNN_SEQINPUT_LAYER_IMPL_
#define _MW_MKLDNN_SEQINPUT_LAYER_IMPL_

#include "MWCNNLayerImpl.hpp"

class MWCNNLayer;
class MWTargetNetworkImpl;

/**
 * Codegen class for Sequence Input layer implementation
 */
class MWSequenceInputLayerImpl : public MWCNNLayerImpl {
  public:
    MWSequenceInputLayerImpl(MWCNNLayer*, MWTargetNetworkImpl*);
    ~MWSequenceInputLayerImpl();
#if MW_SEQIP_TAP
    void predict();
#endif

  private:
    void propagateSize();
};

#endif
