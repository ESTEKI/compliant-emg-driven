#include "stdafx.h"
#include "MWSequenceInputLayerImpl.hpp"
#include "MWSequenceInputLayer.hpp"
#include "MWTargetNetworkImpl.hpp"
#include "MWTensorBase.hpp"
#include "MWCNNLayer.hpp"
#include <cassert>
#include <stdio.h>
 MWSequenceInputLayerImpl::MWSequenceInputLayerImpl(MWCNNLayer* layer, 
MWTargetNetworkImpl* ntwk_impl) : MWCNNLayerImpl(layer, ntwk_impl) { } 
MWSequenceInputLayerImpl::~MWSequenceInputLayerImpl() { } void 
MWSequenceInputLayerImpl::propagateSize() { MWSequenceInputLayer* seqInputLayer 
= static_cast<MWSequenceInputLayer*>(getLayer()); bool isImageInput = 
seqInputLayer->isImageInput; MWTensorBase* opTensor = 
getLayer()->getOutputTensor(0); dnnl::memory::desc layerMemoryDescriptor; 
if(!isImageInput){ dnnl::memory::dims layerMemoryDimensions = { 
opTensor->getSequenceLength() , opTensor->getBatchSize(), 
opTensor->getChannels()}; layerMemoryDescriptor = dnnl::memory::desc( 
layerMemoryDimensions, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::tnc); }else{ dnnl::memory::dims layerMemoryDimensions 
= { opTensor->getSequenceLength() * opTensor->getBatchSize(), 
opTensor->getChannels(), opTensor->getHeight(), opTensor->getWidth()}; 
layerMemoryDescriptor = dnnl::memory::desc( layerMemoryDimensions, 
dnnl::memory::data_type::f32, dnnl::memory::format_tag::nchw); } 
setLayerMemory( std::make_shared<dnnl::memory>(layerMemoryDescriptor, 
*euppfEoiaoCTcVgRPVhA->eng)); }
#if MW_SEQIP_TAP
 void MWSequenceInputLayerImpl::predict() { reorderedLayerOutputTap(0); }
#endif