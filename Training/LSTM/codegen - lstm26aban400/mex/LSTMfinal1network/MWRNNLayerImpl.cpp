#include <cassert>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include "MWRNNLayerImpl.hpp"
#include "MWRNNLayer.hpp"
#include "MWCNNLayerImpl.hpp"
#include "MWCNNLayer.hpp"
#include "MWTensorBase.hpp"
#include "MWTensor.hpp"
#include "MWTargetNetworkImpl.hpp"
#include "MWMkldnnUtils.hpp"
#include "dnnl.hpp"
#include <memory>
 MWRNNLayerImpl::MWRNNLayerImpl(MWCNNLayer* layer, MWTargetNetworkImpl* 
ntwk_impl, int hljcfGWsvZXJZNrImpJB, int UpnEytIWGokwbTFkBcSx, bool 
cQBKlCKXxecGPJrXBXdk, const char* m_weights_file, const char* m_bias_file, const 
char* m_initial_hidden_state_file, const char* m_initial_cell_state_file) : 
MWCNNLayerImpl(layer, ntwk_impl) , 
fxxCPKTclxXPxrdMAkwi(hljcfGWsvZXJZNrImpJB) , 
gzSTokDHvkXefhiGDcWL(UpnEytIWGokwbTFkBcSx) , 
bUVPfnrJhLfHzOLUUrKk(cQBKlCKXxecGPJrXBXdk) , NldNILHvuQqQPSAHXxdT(-1) { MWRNNLayer* 
rnnLayer = static_cast<MWRNNLayer*>(getLayer()); RNNMode puSFZkRJmyuFPfQRswDK = 
rnnLayer->rnnMode; int gTcJMwtYuwiqqUmqvKhT = rnnLayer->getNumGates(); int 
hDaNSVZAofAENeIAiWEw = rnnLayer->numLayers; bool atVCyzqXZAZxwlkRLBRA = 
rnnLayer->isBidirectional; int NtWaRGCHLeTapjWdEHHS = 
atVCyzqXZAZxwlkRLBRA ? 2 : 1; loadWeights(m_weights_file); 
loadBias(m_bias_file); loadInitialHiddenState(m_initial_hidden_state_file); if 
(puSFZkRJmyuFPfQRswDK == LSTM) { loadInitialCellState(m_initial_cell_state_file); 
} else { assert(NtWaRGCHLeTapjWdEHHS == 1); assert(hDaNSVZAofAENeIAiWEw == 
1); } dnnl::memory::dims ipWgtsDims = {hDaNSVZAofAENeIAiWEw, 
NtWaRGCHLeTapjWdEHHS, fxxCPKTclxXPxrdMAkwi, gTcJMwtYuwiqqUmqvKhT, 
gzSTokDHvkXefhiGDcWL}; dnnl::memory::dims recWgtsDims = 
{hDaNSVZAofAENeIAiWEw, NtWaRGCHLeTapjWdEHHS, gzSTokDHvkXefhiGDcWL, 
gTcJMwtYuwiqqUmqvKhT, gzSTokDHvkXefhiGDcWL}; dnnl::memory::dims 
biasDims; if (puSFZkRJmyuFPfQRswDK == LSTM) { biasDims = {hDaNSVZAofAENeIAiWEw, 
NtWaRGCHLeTapjWdEHHS, gTcJMwtYuwiqqUmqvKhT, gzSTokDHvkXefhiGDcWL}; } 
else { biasDims = {hDaNSVZAofAENeIAiWEw, NtWaRGCHLeTapjWdEHHS, 4, 
gzSTokDHvkXefhiGDcWL}; } auto ipWeights_md = dnnl::memory::desc( 
{ipWgtsDims, dnnl::memory::data_type::f32, dnnl::memory::format_tag::ldgoi}); 
ipWeights = std::shared_ptr<dnnl::memory>(new dnnl::memory( ipWeights_md, 
*euppfEoiaoCTcVgRPVhA->eng, ZinudJuZuGitiNTsJpBR.get())); auto recWeights_md 
= dnnl::memory::desc( {recWgtsDims, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::ldgoi}); recWeights = 
std::shared_ptr<dnnl::memory>(new dnnl::memory( recWeights_md, 
*euppfEoiaoCTcVgRPVhA->eng, omxlPZbBePZdWaJOBUUG.get())); auto bias_md = 
dnnl::memory::desc( {biasDims, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::ldgo}); bias = std::shared_ptr<dnnl::memory>( new 
dnnl::memory(bias_md, *euppfEoiaoCTcVgRPVhA->eng, 
ONvcEjLBnVNUdjMKOAwF.get())); rnn_ipWeights_md = 
std::unique_ptr<dnnl::memory::desc>(new dnnl::memory::desc( ipWgtsDims, 
dnnl::memory::data_type::f32, dnnl::memory::format_tag::any)); 
rnn_recWeights_md = std::unique_ptr<dnnl::memory::desc>(new dnnl::memory::desc( 
recWgtsDims, dnnl::memory::data_type::f32, dnnl::memory::format_tag::any)); 
rnn_bias_md = std::unique_ptr<dnnl::memory::desc>(new dnnl::memory::desc( 
biasDims, dnnl::memory::data_type::f32, dnnl::memory::format_tag::ldgo)); 
reorderedIpWeightsDesc = ipWeights->get_desc(); reorderedRecWeightsDesc = 
recWeights->get_desc(); } void MWRNNLayerImpl::propagateSize() { 
clearLayerMapsAndVectors(); MWMkldnnUtils::reorderInputsTo(this); 
allocateInitialStates(); createInputMemdesc(); createOutputMemdesc(); 
MWRNNLayer* rnnLayer = static_cast<MWRNNLayer*>(getLayer()); bool 
atVCyzqXZAZxwlkRLBRA = rnnLayer->isBidirectional; dnnl::rnn_direction 
rnn_direction = atVCyzqXZAZxwlkRLBRA ? 
dnnl::rnn_direction::bidirectional_concat : 
dnnl::rnn_direction::unidirectional_left2right; RNNMode puSFZkRJmyuFPfQRswDK = 
rnnLayer->rnnMode; if (puSFZkRJmyuFPfQRswDK == LSTM) { rnn_d = 
std::unique_ptr<dnnl::lstm_forward::desc>(new dnnl::lstm_forward::desc( 
dnnl::prop_kind::forward_inference, rnn_direction, *rnn_src_md, 
*rnn_hidStateX_md, *rnn_cellStateX_md, *rnn_ipWeights_md, *rnn_recWeights_md, 
*rnn_bias_md, *rnn_dst_md, *rnn_hidStateY_md, *rnn_cellStateY_md)); rnn_pd = 
std::unique_ptr<dnnl::lstm_forward::primitive_desc>( new 
dnnl::lstm_forward::primitive_desc(*rnn_d, *euppfEoiaoCTcVgRPVhA->eng)); 
MWTensorBase* ipTensor = getLayer()->getInputTensor(0); if 
(rnn_pd->weights_layer_desc() != ipWeights->get_desc()) { if 
(rnn_pd->weights_layer_desc() != reorderedIpWeightsDesc) {  rnn_ipWgt_memory = 
ipWeights; rnn_ipWgt_memory = 
std::make_shared<dnnl::memory>(rnn_pd->weights_layer_desc(), 
*euppfEoiaoCTcVgRPVhA->eng); auto rnn_reorder_ipWgt = 
std::unique_ptr<dnnl::reorder>( new dnnl::reorder(*ipWeights, 
*rnn_ipWgt_memory)); pipeline_weights_memory_args.push_back( {{DNNL_ARG_FROM, 
*ipWeights}, {DNNL_ARG_TO, *rnn_ipWgt_memory}}); 
pipeline_weights.push_back(*rnn_reorder_ipWgt); reorderedIpWeightsDesc = 
rnn_pd->weights_layer_desc(); } } else { rnn_ipWgt_memory = ipWeights; } if 
(rnn_pd->weights_iter_desc() != recWeights->get_desc()) { if 
(rnn_pd->weights_iter_desc() != reorderedRecWeightsDesc) {  rnn_recWgt_memory = 
recWeights; rnn_recWgt_memory = 
std::make_shared<dnnl::memory>(rnn_pd->weights_iter_desc(), 
*euppfEoiaoCTcVgRPVhA->eng); auto rnn_reorder_recWgt = 
std::unique_ptr<dnnl::reorder>( new dnnl::reorder(*recWeights, 
*rnn_recWgt_memory)); pipeline_weights_memory_args.push_back( {{DNNL_ARG_FROM, 
*recWeights}, {DNNL_ARG_TO, *rnn_recWgt_memory}}); 
pipeline_weights.push_back(*rnn_reorder_recWgt); reorderedRecWeightsDesc = 
rnn_pd->weights_iter_desc(); } } else { rnn_recWgt_memory = recWeights; } if 
(pipeline_weights.size() > 0) { auto s = 
dnnl::stream(*euppfEoiaoCTcVgRPVhA->eng); assert(pipeline_weights.size() == 
pipeline_weights_memory_args.size()); for (int i = 0; i < 
pipeline_weights.size(); i++) { pipeline_weights[i].execute(s, 
pipeline_weights_memory_args[i]); } } if (bUVPfnrJhLfHzOLUUrKk && 
(ipTensor->getSequenceLength() > 1)) { MWTensorBase* opTensor = 
rnnLayer->getOutputTensor(0); dnnl::memory::dims layerMemoryDimensions = { 
opTensor->getSequenceLength(), opTensor->getBatchSize(), 
opTensor->getChannels()}; auto layerMemoryDescriptor = dnnl::memory::desc( 
layerMemoryDimensions, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::tnc);  setLayerMemory( 
std::make_shared<dnnl::memory>(layerMemoryDescriptor, 
*euppfEoiaoCTcVgRPVhA->eng)); auto dst_md = dnnl::memory::desc( 
{ipTensor->getSequenceLength(), opTensor->getBatchSize(), 
opTensor->getChannels()}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::tnc);  rnn_dst_memory = 
std::shared_ptr<dnnl::memory>( new dnnl::memory(dst_md, 
*euppfEoiaoCTcVgRPVhA->eng)); } else { 
setLayerMemory(std::make_shared<dnnl::memory>(rnn_pd->dst_layer_desc(), 
*euppfEoiaoCTcVgRPVhA->eng)); rnn_dst_memory = getLayerMemory(); } rnn = 
std::unique_ptr<dnnl::lstm_forward>(new dnnl::lstm_forward(*rnn_pd)); 
pipeline.push_back(*rnn); pipeline_memory_args.push_back({{DNNL_ARG_SRC_LAYER, 
*getReorderedIpMemory()}, {DNNL_ARG_SRC_ITER, *hx_memory}, 
{DNNL_ARG_SRC_ITER_C, *cx_memory}, {DNNL_ARG_WEIGHTS_LAYER, *rnn_ipWgt_memory}, 
{DNNL_ARG_WEIGHTS_ITER, *rnn_recWgt_memory}, {DNNL_ARG_BIAS, *bias}, 
{DNNL_ARG_DST_LAYER, *rnn_dst_memory}, {DNNL_ARG_DST_ITER, *hy_memory}, 
{DNNL_ARG_DST_ITER_C, *cy_memory}}); } else { lbr_gru_d = 
std::unique_ptr<dnnl::lbr_gru_forward::desc>(new dnnl::lbr_gru_forward::desc( 
dnnl::prop_kind::forward_inference, rnn_direction, *rnn_src_md, 
*rnn_hidStateX_md, *rnn_ipWeights_md, *rnn_recWeights_md, *rnn_bias_md, 
*rnn_dst_md, *rnn_hidStateY_md)); lbr_gru_pd = 
std::unique_ptr<dnnl::lbr_gru_forward::primitive_desc>( new 
dnnl::lbr_gru_forward::primitive_desc(*lbr_gru_d, *euppfEoiaoCTcVgRPVhA->eng)); 
MWTensorBase* ipTensor = getLayer()->getInputTensor(0); if 
(lbr_gru_pd->weights_layer_desc() != ipWeights->get_desc()) { if 
(lbr_gru_pd->weights_layer_desc() != reorderedIpWeightsDesc) {  
rnn_ipWgt_memory = ipWeights; rnn_ipWgt_memory = 
std::make_shared<dnnl::memory>( lbr_gru_pd->weights_layer_desc(), 
*euppfEoiaoCTcVgRPVhA->eng); auto rnn_reorder_ipWgt = 
std::unique_ptr<dnnl::reorder>( new dnnl::reorder(*ipWeights, 
*rnn_ipWgt_memory)); pipeline_weights_memory_args.push_back( {{DNNL_ARG_FROM, 
*ipWeights}, {DNNL_ARG_TO, *rnn_ipWgt_memory}}); 
pipeline_weights.push_back(*rnn_reorder_ipWgt); reorderedIpWeightsDesc = 
lbr_gru_pd->weights_layer_desc(); } } else { rnn_ipWgt_memory = ipWeights; } if 
(lbr_gru_pd->weights_iter_desc() != recWeights->get_desc()) { if 
(lbr_gru_pd->weights_iter_desc() != reorderedRecWeightsDesc) {  
rnn_recWgt_memory = recWeights; rnn_recWgt_memory = 
std::make_shared<dnnl::memory>( lbr_gru_pd->weights_iter_desc(), 
*euppfEoiaoCTcVgRPVhA->eng); auto rnn_reorder_recWgt = 
std::unique_ptr<dnnl::reorder>( new dnnl::reorder(*recWeights, 
*rnn_recWgt_memory)); pipeline_weights_memory_args.push_back( {{DNNL_ARG_FROM, 
*recWeights}, {DNNL_ARG_TO, *rnn_recWgt_memory}}); 
pipeline_weights.push_back(*rnn_reorder_recWgt); reorderedRecWeightsDesc = 
lbr_gru_pd->weights_iter_desc(); } } else { rnn_recWgt_memory = recWeights; } 
if (pipeline_weights.size() > 0) { auto s = 
dnnl::stream(*euppfEoiaoCTcVgRPVhA->eng); assert(pipeline_weights.size() == 
pipeline_weights_memory_args.size()); for (int i = 0; i < 
pipeline_weights.size(); i++) { pipeline_weights[i].execute(s, 
pipeline_weights_memory_args[i]); } } if (bUVPfnrJhLfHzOLUUrKk && 
(ipTensor->getSequenceLength() > 1)) { MWTensorBase* opTensor = 
rnnLayer->getOutputTensor(0); dnnl::memory::dims layerMemoryDimensions = { 
opTensor->getSequenceLength(), opTensor->getBatchSize(), 
opTensor->getChannels()}; auto layerMemoryDescriptor = dnnl::memory::desc( 
layerMemoryDimensions, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::tnc);  setLayerMemory( 
std::make_shared<dnnl::memory>(layerMemoryDescriptor, 
*euppfEoiaoCTcVgRPVhA->eng)); auto dst_md = dnnl::memory::desc( 
{ipTensor->getSequenceLength(), opTensor->getBatchSize(), 
opTensor->getChannels()}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::tnc);  rnn_dst_memory = 
std::shared_ptr<dnnl::memory>( new dnnl::memory(dst_md, 
*euppfEoiaoCTcVgRPVhA->eng)); } else { 
setLayerMemory(std::make_shared<dnnl::memory>(lbr_gru_pd->dst_layer_desc(), 
*euppfEoiaoCTcVgRPVhA->eng)); rnn_dst_memory = getLayerMemory(); } lbr_gru = 
std::unique_ptr<dnnl::lbr_gru_forward>(new dnnl::lbr_gru_forward(*lbr_gru_pd)); 
pipeline.push_back(*lbr_gru); 
pipeline_memory_args.push_back({{DNNL_ARG_SRC_LAYER, *getReorderedIpMemory()}, 
{DNNL_ARG_SRC_ITER, *hx_memory}, {DNNL_ARG_WEIGHTS_LAYER, *rnn_ipWgt_memory}, 
{DNNL_ARG_WEIGHTS_ITER, *rnn_recWgt_memory}, {DNNL_ARG_BIAS, *bias}, 
{DNNL_ARG_DST_LAYER, *rnn_dst_memory}, {DNNL_ARG_DST_ITER, *hy_memory}}); } 
return; } void MWRNNLayerImpl::allocateInitialStates() { MWRNNLayer* rnnLayer = 
static_cast<MWRNNLayer*>(getLayer()); MWTensorBase* ipTensor = 
rnnLayer->getInputTensor(0); int hDaNSVZAofAENeIAiWEw = rnnLayer->numLayers; 
bool atVCyzqXZAZxwlkRLBRA = rnnLayer->isBidirectional; int 
NtWaRGCHLeTapjWdEHHS = atVCyzqXZAZxwlkRLBRA ? 2 : 1; int batchSize = 
ipTensor->getBatchSize(); dnnl::memory::dims stateDims = 
{hDaNSVZAofAENeIAiWEw, NtWaRGCHLeTapjWdEHHS, batchSize, 
gzSTokDHvkXefhiGDcWL}; auto hx_md = dnnl::memory::desc( {stateDims, 
dnnl::memory::data_type::f32, dnnl::memory::format_tag::ldnc}); hx_memory = 
std::shared_ptr<dnnl::memory>(new dnnl::memory(hx_md, 
*euppfEoiaoCTcVgRPVhA->eng)); auto hy_md = dnnl::memory::desc( {stateDims, 
dnnl::memory::data_type::f32, dnnl::memory::format_tag::ldnc}); hy_memory = 
std::shared_ptr<dnnl::memory>(new dnnl::memory(hy_md, 
*euppfEoiaoCTcVgRPVhA->eng)); RNNMode puSFZkRJmyuFPfQRswDK = rnnLayer->rnnMode; if 
(puSFZkRJmyuFPfQRswDK == LSTM) { auto cx_md = dnnl::memory::desc( {stateDims, 
dnnl::memory::data_type::f32, dnnl::memory::format_tag::ldnc}); cx_memory = 
std::shared_ptr<dnnl::memory>(new dnnl::memory(cx_md, 
*euppfEoiaoCTcVgRPVhA->eng)); auto cy_md = dnnl::memory::desc( {stateDims, 
dnnl::memory::data_type::f32, dnnl::memory::format_tag::ldnc}); cy_memory = 
std::shared_ptr<dnnl::memory>(new dnnl::memory(cy_md, 
*euppfEoiaoCTcVgRPVhA->eng)); } } void MWRNNLayerImpl::allocate() { MWRNNLayer* 
rnnLayer = static_cast<MWRNNLayer*>(getLayer()); MWTensorBase* ipTensorBase = 
rnnLayer->getInputTensor(0); MWTensor<float>* ipTensor = 
static_cast<MWTensor<float>*>(ipTensorBase); int hDaNSVZAofAENeIAiWEw = 
rnnLayer->numLayers; bool atVCyzqXZAZxwlkRLBRA = 
rnnLayer->isBidirectional; int NtWaRGCHLeTapjWdEHHS = 
atVCyzqXZAZxwlkRLBRA ? 2 : 1; int batchSize = ipTensor->getBatchSize(); 
RNNMode puSFZkRJmyuFPfQRswDK = rnnLayer->rnnMode; if (NldNILHvuQqQPSAHXxdT != 
batchSize) { int stateSize = NtWaRGCHLeTapjWdEHHS * hDaNSVZAofAENeIAiWEw * 
batchSize * gzSTokDHvkXefhiGDcWL; UKtMXCCqdjeyaVHabkxg = 
std::shared_ptr<float>(new float[stateSize]); TxNFOfYScyqGlEFFxbAv = 
std::shared_ptr<float>(new float[stateSize]); UEESbUvbMihFnquvuFij = 
std::shared_ptr<float>(new float[stateSize]); if (puSFZkRJmyuFPfQRswDK == LSTM) { 
QjgQHaUACFNSteMrRtRj = std::shared_ptr<float>(new float[stateSize]); 
QVgVGfoCXYiYXzPhvVPX = std::shared_ptr<float>(new float[stateSize]); 
QhTesEEIHwhNmHSeYbRR = std::shared_ptr<float>(new float[stateSize]); 
prepareInitialState(QwUuNuQNtlPXrIwRNiSZ.get(), 
QjgQHaUACFNSteMrRtRj.get()); } 
prepareInitialState(UdmcwaUkepxfZrpdpcAN.get(), 
UKtMXCCqdjeyaVHabkxg.get()); resetState(); NldNILHvuQqQPSAHXxdT = batchSize; } 
else { setStatePrimitives(); } } void MWRNNLayerImpl::resetState() { 
MWRNNLayer* rnnLayer = static_cast<MWRNNLayer*>(getLayer()); RNNMode 
puSFZkRJmyuFPfQRswDK = rnnLayer->rnnMode; UzaGmBLFEwmwaFXebUma = 
UKtMXCCqdjeyaVHabkxg.get(); VCbcPxtPsBLTrHYdEvqn = TxNFOfYScyqGlEFFxbAv.get(); if 
(puSFZkRJmyuFPfQRswDK == LSTM) { RAtlBpdedvgxUsgDTsch = QjgQHaUACFNSteMrRtRj.get(); 
REXdEoRjxuQJkqgIDihy = QVgVGfoCXYiYXzPhvVPX.get(); } setStatePrimitives(); } void 
MWRNNLayerImpl::setStatePrimitives() { MWRNNLayer* rnnLayer = 
static_cast<MWRNNLayer*>(getLayer()); RNNMode puSFZkRJmyuFPfQRswDK = 
rnnLayer->rnnMode; if (hx_memory != nullptr) { assert(hy_memory != nullptr); 
hx_memory->set_data_handle(UzaGmBLFEwmwaFXebUma); 
hy_memory->set_data_handle(VCbcPxtPsBLTrHYdEvqn); if (puSFZkRJmyuFPfQRswDK == LSTM) { 
assert(cx_memory != nullptr); assert(cy_memory != nullptr); 
cx_memory->set_data_handle(RAtlBpdedvgxUsgDTsch); 
cy_memory->set_data_handle(REXdEoRjxuQJkqgIDihy); } } } void 
MWRNNLayerImpl::updateState() { MWRNNLayer* rnnLayer = 
static_cast<MWRNNLayer*>(getLayer()); RNNMode puSFZkRJmyuFPfQRswDK = 
rnnLayer->rnnMode; if (UzaGmBLFEwmwaFXebUma == UKtMXCCqdjeyaVHabkxg.get()) { 
assert(VCbcPxtPsBLTrHYdEvqn == TxNFOfYScyqGlEFFxbAv.get()); UzaGmBLFEwmwaFXebUma = 
VCbcPxtPsBLTrHYdEvqn; VCbcPxtPsBLTrHYdEvqn = UEESbUvbMihFnquvuFij.get(); if 
(puSFZkRJmyuFPfQRswDK == LSTM) { assert(RAtlBpdedvgxUsgDTsch == 
QjgQHaUACFNSteMrRtRj.get()); assert(REXdEoRjxuQJkqgIDihy == 
QVgVGfoCXYiYXzPhvVPX.get()); RAtlBpdedvgxUsgDTsch = REXdEoRjxuQJkqgIDihy; REXdEoRjxuQJkqgIDihy = 
QhTesEEIHwhNmHSeYbRR.get(); } } else { float* tmp; tmp = UzaGmBLFEwmwaFXebUma; 
UzaGmBLFEwmwaFXebUma = VCbcPxtPsBLTrHYdEvqn; VCbcPxtPsBLTrHYdEvqn = tmp; if (puSFZkRJmyuFPfQRswDK == 
LSTM) { tmp = RAtlBpdedvgxUsgDTsch; RAtlBpdedvgxUsgDTsch = REXdEoRjxuQJkqgIDihy; REXdEoRjxuQJkqgIDihy = 
tmp; } } if (rnnLayer->isBidirectional) { resetBackwardStates(); } 
setStatePrimitives(); } void MWRNNLayerImpl::resetBackwardStates() { 
assert(UzaGmBLFEwmwaFXebUma != UKtMXCCqdjeyaVHabkxg.get()); assert(RAtlBpdedvgxUsgDTsch != 
QjgQHaUACFNSteMrRtRj.get()); MWRNNLayer* rnnLayer = 
static_cast<MWRNNLayer*>(getLayer()); assert(rnnLayer->isBidirectional); 
RNNMode puSFZkRJmyuFPfQRswDK = rnnLayer->rnnMode; assert(puSFZkRJmyuFPfQRswDK == 
LSTM); int hDaNSVZAofAENeIAiWEw = rnnLayer->numLayers; int batchSize = 
rnnLayer->getOutputTensor(0)->getBatchSize(); int forwardPassOffset = 
gzSTokDHvkXefhiGDcWL * batchSize; int biLSTMLayerOffset = 2 * 
gzSTokDHvkXefhiGDcWL * batchSize; for (int i = 0; i < 
hDaNSVZAofAENeIAiWEw; ++i) { memcpy(UzaGmBLFEwmwaFXebUma + (long 
int)(forwardPassOffset + i * biLSTMLayerOffset), 
(float*)UKtMXCCqdjeyaVHabkxg.get() + (long int)(forwardPassOffset + i * 
biLSTMLayerOffset), sizeof(float) * batchSize * gzSTokDHvkXefhiGDcWL); 
memcpy(RAtlBpdedvgxUsgDTsch + (long int)(forwardPassOffset + i * biLSTMLayerOffset), 
(float*)QjgQHaUACFNSteMrRtRj.get() + (long int)(forwardPassOffset + i * 
biLSTMLayerOffset), sizeof(float) * batchSize * gzSTokDHvkXefhiGDcWL); } 
} void MWRNNLayerImpl::predict() { auto s = 
dnnl::stream(*euppfEoiaoCTcVgRPVhA->eng); assert(pipeline.size() == 
pipeline_memory_args.size()); for (int i = 0; i < pipeline.size(); i++) { 
pipeline[i].execute(s, pipeline_memory_args[i]); } MWRNNLayer* rnnLayer = 
static_cast<MWRNNLayer*>(getLayer()); MWTensorBase* opTensorBase = 
rnnLayer->getOutputTensor(0); MWTensorBase* ipTensorBase = 
rnnLayer->getInputTensor(0); MWTensor<float>* opTensor = 
static_cast<MWTensor<float>*>(opTensorBase); MWTensor<float>* ipTensor = 
static_cast<MWTensor<float>*>(ipTensorBase); if (bUVPfnrJhLfHzOLUUrKk && 
(ipTensor->getSequenceLength() > 1)) { bool atVCyzqXZAZxwlkRLBRA = 
rnnLayer->isBidirectional; if (atVCyzqXZAZxwlkRLBRA) { long int 
offsetToLastTimestep_forward = opTensor->getChannels() * 
opTensor->getBatchSize() * (ipTensor->getSequenceLength() - 1); long int 
offsetToLastTimestep_backward = gzSTokDHvkXefhiGDcWL; float* layerOp = 
(float*)rnn_dst_memory->get_data_handle(); for (int i = 0; i < 
opTensor->getBatchSize(); i++) { memcpy(opTensor->getData() + i * 
opTensor->getChannels(), layerOp + offsetToLastTimestep_forward + i * 
opTensor->getChannels(), sizeof(float) * gzSTokDHvkXefhiGDcWL); } for 
(int i = 0; i < opTensor->getBatchSize(); i++) { memcpy( opTensor->getData() + 
i * opTensor->getChannels() + gzSTokDHvkXefhiGDcWL, layerOp + 
offsetToLastTimestep_backward + i * opTensor->getChannels(), sizeof(float) * 
gzSTokDHvkXefhiGDcWL); } } else { long int offsetToLastTimestep = 
opTensor->getChannels() * opTensor->getBatchSize() * 
(ipTensor->getSequenceLength() - 1); float* layerOp = 
(float*)rnn_dst_memory->get_data_handle(); memcpy(opTensor->getData(), layerOp 
+ offsetToLastTimestep, opTensor->getBatchSize() * opTensor->getChannels() * 
sizeof(float)); } }
#if MW_LSTM_TAP
 reorderedLayerOutputTap(0);
#endif
 return; } void MWRNNLayerImpl::createInputMemdesc() { MWRNNLayer* rnnLayer = 
static_cast<MWRNNLayer*>(getLayer()); MWTensorBase* ipTensor = 
rnnLayer->getInputTensor(0); int hDaNSVZAofAENeIAiWEw = rnnLayer->numLayers; 
bool atVCyzqXZAZxwlkRLBRA = rnnLayer->isBidirectional; int 
NtWaRGCHLeTapjWdEHHS = atVCyzqXZAZxwlkRLBRA ? 2 : 1; int n = 
ipTensor->getBatchSize(); int c = ipTensor->getChannels(); int t = 
ipTensor->getSequenceLength(); rnn_src_md = 
std::unique_ptr<dnnl::memory::desc>(new dnnl::memory::desc( {t, n, c}, 
dnnl::memory::data_type::f32, dnnl::memory::format_tag::tnc)); rnn_hidStateX_md 
= std::unique_ptr<dnnl::memory::desc>(new dnnl::memory::desc( 
{hDaNSVZAofAENeIAiWEw, NtWaRGCHLeTapjWdEHHS, n, 
gzSTokDHvkXefhiGDcWL}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::ldnc)); RNNMode puSFZkRJmyuFPfQRswDK = 
rnnLayer->rnnMode; if (puSFZkRJmyuFPfQRswDK == LSTM) { rnn_cellStateX_md = 
std::unique_ptr<dnnl::memory::desc>(new dnnl::memory::desc( 
{hDaNSVZAofAENeIAiWEw, NtWaRGCHLeTapjWdEHHS, n, 
gzSTokDHvkXefhiGDcWL}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::ldnc)); } } void 
MWRNNLayerImpl::createOutputMemdesc() { MWRNNLayer* rnnLayer = 
static_cast<MWRNNLayer*>(getLayer()); MWTensorBase* ipTensor = 
rnnLayer->getInputTensor(0); MWTensorBase* opTensor = 
rnnLayer->getOutputTensor(0); int hDaNSVZAofAENeIAiWEw = rnnLayer->numLayers; 
bool atVCyzqXZAZxwlkRLBRA = rnnLayer->isBidirectional; int 
NtWaRGCHLeTapjWdEHHS = atVCyzqXZAZxwlkRLBRA ? 2 : 1; int n = 
ipTensor->getBatchSize(); int t = ipTensor->getSequenceLength(); rnn_dst_md = 
std::unique_ptr<dnnl::memory::desc>( new dnnl::memory::desc({t, n, 
opTensor->getChannels()}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::tnc)); rnn_hidStateY_md = 
std::unique_ptr<dnnl::memory::desc>(new dnnl::memory::desc( 
{hDaNSVZAofAENeIAiWEw, NtWaRGCHLeTapjWdEHHS, n, 
gzSTokDHvkXefhiGDcWL}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::ldnc)); RNNMode puSFZkRJmyuFPfQRswDK = 
rnnLayer->rnnMode; if (puSFZkRJmyuFPfQRswDK == LSTM) { rnn_cellStateY_md = 
std::unique_ptr<dnnl::memory::desc>(new dnnl::memory::desc( 
{hDaNSVZAofAENeIAiWEw, NtWaRGCHLeTapjWdEHHS, n, 
gzSTokDHvkXefhiGDcWL}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::ldnc)); } } void MWRNNLayerImpl::loadWeights(const 
char* SGsAudmgjmvcUXzzrUtf) { FILE* SUleyRyvAggTFnSdxLru = 
MWCNNLayer::openBinaryFile(SGsAudmgjmvcUXzzrUtf); if (SUleyRyvAggTFnSdxLru == nullptr) 
{ std::cerr << "Unable to open the weights file" << std::endl; } MWRNNLayer* 
rnnLayer = static_cast<MWRNNLayer*>(getLayer()); int hDaNSVZAofAENeIAiWEw = 
rnnLayer->numLayers; bool atVCyzqXZAZxwlkRLBRA = 
rnnLayer->isBidirectional; int NtWaRGCHLeTapjWdEHHS = 
atVCyzqXZAZxwlkRLBRA ? 2 : 1; int gTcJMwtYuwiqqUmqvKhT = 
rnnLayer->getNumGates(); int Iw = gTcJMwtYuwiqqUmqvKhT * 
fxxCPKTclxXPxrdMAkwi * gzSTokDHvkXefhiGDcWL; int Rw = 
gTcJMwtYuwiqqUmqvKhT * gzSTokDHvkXefhiGDcWL * 
gzSTokDHvkXefhiGDcWL; int eachgateIwSize = fxxCPKTclxXPxrdMAkwi * 
gzSTokDHvkXefhiGDcWL; int eachgateRwSize = gzSTokDHvkXefhiGDcWL * 
gzSTokDHvkXefhiGDcWL; int aLsOwwcceEmRSYzllBNs = 
NtWaRGCHLeTapjWdEHHS * hDaNSVZAofAENeIAiWEw * Iw; int 
pckLLTEdVPoCZLRwyDnM = NtWaRGCHLeTapjWdEHHS * hDaNSVZAofAENeIAiWEw * Rw; 
ZinudJuZuGitiNTsJpBR = std::shared_ptr<float>(new 
float[aLsOwwcceEmRSYzllBNs]); omxlPZbBePZdWaJOBUUG = 
std::shared_ptr<float>(new float[pckLLTEdVPoCZLRwyDnM]); auto 
yCdIUfwoZFngCRRRkCTg = (float*)calloc(aLsOwwcceEmRSYzllBNs + 
pckLLTEdVPoCZLRwyDnM, sizeof(float)); 
MWCNNLayer::call_fread(yCdIUfwoZFngCRRRkCTg, sizeof(float), 
aLsOwwcceEmRSYzllBNs + pckLLTEdVPoCZLRwyDnM, SUleyRyvAggTFnSdxLru, 
SGsAudmgjmvcUXzzrUtf); RNNMode puSFZkRJmyuFPfQRswDK = rnnLayer->rnnMode; if 
(puSFZkRJmyuFPfQRswDK == LSTM) { for (int i = 0; i < (hDaNSVZAofAENeIAiWEw * 
NtWaRGCHLeTapjWdEHHS); i++) { memcpy(ZinudJuZuGitiNTsJpBR.get() + i * 
Iw, yCdIUfwoZFngCRRRkCTg + i * (Iw + Rw), Iw * sizeof(float)); 
memcpy(omxlPZbBePZdWaJOBUUG.get() + i * Rw, yCdIUfwoZFngCRRRkCTg + i 
* (Iw + Rw) + Iw, Rw * sizeof(float)); } } else { float* ipwghts_buffer = 
(float*)calloc(aLsOwwcceEmRSYzllBNs, sizeof(float)); float* recWgts_buffer = 
(float*)calloc(pckLLTEdVPoCZLRwyDnM, sizeof(float)); for (int i = 0; i < 
(hDaNSVZAofAENeIAiWEw); i++) { memcpy(ipwghts_buffer + i * Iw, 
yCdIUfwoZFngCRRRkCTg + i * (Iw + Rw), Iw * sizeof(float)); 
memcpy(recWgts_buffer + i * Rw, yCdIUfwoZFngCRRRkCTg + i * (Iw + Rw) + Iw, 
Rw * sizeof(float)); } for (int k = 0; k < (hDaNSVZAofAENeIAiWEw); k++) { for 
(int i = 0; i < (gTcJMwtYuwiqqUmqvKhT); i++) { if (i == 0) { 
memcpy(ZinudJuZuGitiNTsJpBR.get() + eachgateIwSize + gTcJMwtYuwiqqUmqvKhT * 
k * eachgateIwSize, ipwghts_buffer + gTcJMwtYuwiqqUmqvKhT * k * eachgateIwSize, 
eachgateIwSize * sizeof(float)); memcpy(omxlPZbBePZdWaJOBUUG.get() + 
eachgateRwSize + gTcJMwtYuwiqqUmqvKhT * k * eachgateRwSize, recWgts_buffer + 
gTcJMwtYuwiqqUmqvKhT * k * eachgateRwSize, eachgateRwSize * sizeof(float)); } 
else if (i == 1) { memcpy( ZinudJuZuGitiNTsJpBR.get() + 
gTcJMwtYuwiqqUmqvKhT * k * eachgateIwSize, ipwghts_buffer + eachgateIwSize + 
gTcJMwtYuwiqqUmqvKhT * k * eachgateIwSize, eachgateIwSize * sizeof(float)); 
memcpy( omxlPZbBePZdWaJOBUUG.get() + gTcJMwtYuwiqqUmqvKhT * k * 
eachgateRwSize, recWgts_buffer + eachgateRwSize + gTcJMwtYuwiqqUmqvKhT * k * 
eachgateRwSize, eachgateRwSize * sizeof(float)); } else { 
memcpy(ZinudJuZuGitiNTsJpBR.get() + i * eachgateIwSize + 
gTcJMwtYuwiqqUmqvKhT * k * eachgateIwSize, ipwghts_buffer + i * 
(eachgateIwSize) + gTcJMwtYuwiqqUmqvKhT * k * eachgateIwSize, eachgateIwSize * 
sizeof(float)); memcpy(omxlPZbBePZdWaJOBUUG.get() + i * eachgateRwSize + 
gTcJMwtYuwiqqUmqvKhT * k * eachgateRwSize, recWgts_buffer + i * 
(eachgateRwSize) + gTcJMwtYuwiqqUmqvKhT * k * eachgateRwSize, eachgateRwSize * 
sizeof(float)); } } } free(ipwghts_buffer); free(recWgts_buffer); } 
free(yCdIUfwoZFngCRRRkCTg); fclose(SUleyRyvAggTFnSdxLru); } void 
MWRNNLayerImpl::loadBias(const char* SGsAudmgjmvcUXzzrUtf) { MWRNNLayer* 
rnnLayer = static_cast<MWRNNLayer*>(getLayer()); int hDaNSVZAofAENeIAiWEw = 
rnnLayer->numLayers; bool atVCyzqXZAZxwlkRLBRA = 
rnnLayer->isBidirectional; int NtWaRGCHLeTapjWdEHHS = 
atVCyzqXZAZxwlkRLBRA ? 2 : 1; int gTcJMwtYuwiqqUmqvKhT = 
rnnLayer->getNumGates(); auto biasMode = rnnLayer->rnnBiasMode == 
SINGLE_INPUT_BIAS ? 0 : 1; RNNMode puSFZkRJmyuFPfQRswDK = rnnLayer->rnnMode; if 
(puSFZkRJmyuFPfQRswDK == LSTM) { int OumvfgWXDdmsQaciHMHx = 
NtWaRGCHLeTapjWdEHHS * hDaNSVZAofAENeIAiWEw * gTcJMwtYuwiqqUmqvKhT * 
gzSTokDHvkXefhiGDcWL; ONvcEjLBnVNUdjMKOAwF = 
std::shared_ptr<float>(new float[OumvfgWXDdmsQaciHMHx]); FILE* SUleyRyvAggTFnSdxLru = 
MWCNNLayer::openBinaryFile(SGsAudmgjmvcUXzzrUtf); if (SUleyRyvAggTFnSdxLru == nullptr) 
{ std::cerr << "Unable to open Bias file" << std::endl; } 
MWCNNLayer::call_fread(ONvcEjLBnVNUdjMKOAwF.get(), sizeof(float), 
OumvfgWXDdmsQaciHMHx, SUleyRyvAggTFnSdxLru, SGsAudmgjmvcUXzzrUtf); 
fclose(SUleyRyvAggTFnSdxLru); } else { int PmFfARVzoHVAYkfpuvqK = 
hDaNSVZAofAENeIAiWEw * 4 * gzSTokDHvkXefhiGDcWL; int 
OwenhowBxTAXHXmJpIKd = hDaNSVZAofAENeIAiWEw * 3 * (biasMode + 1) * 
gzSTokDHvkXefhiGDcWL; auto biasGateSize = gzSTokDHvkXefhiGDcWL; 
ONvcEjLBnVNUdjMKOAwF = std::shared_ptr<float>(new 
float[PmFfARVzoHVAYkfpuvqK]); FILE* SUleyRyvAggTFnSdxLru = 
MWCNNLayer::openBinaryFile(SGsAudmgjmvcUXzzrUtf); if (SUleyRyvAggTFnSdxLru == nullptr) 
{ std::cerr << "Unable to open Bias file" << std::endl; } auto OKaRVOctKLlnIyGmjRNW 
= (float*)calloc(PmFfARVzoHVAYkfpuvqK, sizeof(float)); if (biasMode == 
1) { auto OiVqrkNdXioJhALWMMvm = (float*)calloc(OwenhowBxTAXHXmJpIKd, 
sizeof(float)); for (int i = 0; i < hDaNSVZAofAENeIAiWEw; i++) { 
MWCNNLayer::call_fread(OiVqrkNdXioJhALWMMvm + i * OwenhowBxTAXHXmJpIKd 
/ hDaNSVZAofAENeIAiWEw, sizeof(float), OwenhowBxTAXHXmJpIKd / 
hDaNSVZAofAENeIAiWEw, SUleyRyvAggTFnSdxLru, SGsAudmgjmvcUXzzrUtf); 
fseek(SUleyRyvAggTFnSdxLru, sizeof(float) * i * OwenhowBxTAXHXmJpIKd / 
hDaNSVZAofAENeIAiWEw, SEEK_SET); } for (int k = 0; k < hDaNSVZAofAENeIAiWEw; 
k++) { for (int i = 0; i < 2; i++) { for (int j = 0; j < biasGateSize; j++) { 
OKaRVOctKLlnIyGmjRNW[k * 4 * biasGateSize + i * biasGateSize + j] = 
OiVqrkNdXioJhALWMMvm[k * 6 * biasGateSize + i * biasGateSize + j] + 
OiVqrkNdXioJhALWMMvm[k * 6 * biasGateSize + (i + 3) * biasGateSize + j]; } } 
} for (int k = 0; k < hDaNSVZAofAENeIAiWEw; k++) { for (int j = 0; j < 
biasGateSize; j++) { OKaRVOctKLlnIyGmjRNW[k * 4 * biasGateSize + 2 * biasGateSize + 
j] = OiVqrkNdXioJhALWMMvm[k * 6 * biasGateSize + 2 * biasGateSize + j]; 
OKaRVOctKLlnIyGmjRNW[k * 4 * biasGateSize + 3 * biasGateSize + j] = 
OiVqrkNdXioJhALWMMvm[k * 6 * biasGateSize + 5 * biasGateSize + j]; } } 
free(OiVqrkNdXioJhALWMMvm); } else { for (int i = 0; i < 
hDaNSVZAofAENeIAiWEw; i++) { MWCNNLayer::call_fread( OKaRVOctKLlnIyGmjRNW + i * 
PmFfARVzoHVAYkfpuvqK / hDaNSVZAofAENeIAiWEw, sizeof(float), 
OwenhowBxTAXHXmJpIKd / hDaNSVZAofAENeIAiWEw, SUleyRyvAggTFnSdxLru, 
SGsAudmgjmvcUXzzrUtf); fseek(SUleyRyvAggTFnSdxLru, sizeof(float) * i * 
OwenhowBxTAXHXmJpIKd / hDaNSVZAofAENeIAiWEw, SEEK_SET); } } for (int k 
= 0; k < hDaNSVZAofAENeIAiWEw; k++) { for (int i = 0; i < 
(gTcJMwtYuwiqqUmqvKhT + 1); i++) { if (i == 0) { 
memcpy(ONvcEjLBnVNUdjMKOAwF.get() + k * 4 * biasGateSize, OKaRVOctKLlnIyGmjRNW + 
biasGateSize + k * 4 * biasGateSize, biasGateSize * sizeof(float)); } else if 
(i == 1) { memcpy(ONvcEjLBnVNUdjMKOAwF.get() + biasGateSize + k * 4 * 
biasGateSize, OKaRVOctKLlnIyGmjRNW + k * 4 * biasGateSize, biasGateSize * 
sizeof(float)); } else { memcpy(ONvcEjLBnVNUdjMKOAwF.get() + i * biasGateSize 
+ k * 4 * biasGateSize, OKaRVOctKLlnIyGmjRNW + i * biasGateSize + k * 4 * 
biasGateSize, biasGateSize * sizeof(float)); } } } free(OKaRVOctKLlnIyGmjRNW); 
fclose(SUleyRyvAggTFnSdxLru); } return; } void 
MWRNNLayerImpl::loadInitialHiddenState(const char* SGsAudmgjmvcUXzzrUtf) { 
MWRNNLayer* rnnLayer = static_cast<MWRNNLayer*>(getLayer()); int 
hDaNSVZAofAENeIAiWEw = rnnLayer->numLayers; bool atVCyzqXZAZxwlkRLBRA = 
rnnLayer->isBidirectional; int NtWaRGCHLeTapjWdEHHS = 
atVCyzqXZAZxwlkRLBRA ? 2 : 1; int vFNECEAeLZsYsUxvlgqL = 
NtWaRGCHLeTapjWdEHHS * hDaNSVZAofAENeIAiWEw * gzSTokDHvkXefhiGDcWL; 
FILE* SUleyRyvAggTFnSdxLru = MWCNNLayer::openBinaryFile(SGsAudmgjmvcUXzzrUtf); if 
(SUleyRyvAggTFnSdxLru == nullptr) { std::cerr << 
"Unable to open initial hidden state file" << std::endl; } 
UdmcwaUkepxfZrpdpcAN = std::shared_ptr<float>(new 
float[vFNECEAeLZsYsUxvlgqL]); 
MWCNNLayer::call_fread(UdmcwaUkepxfZrpdpcAN.get(), sizeof(float), 
vFNECEAeLZsYsUxvlgqL, SUleyRyvAggTFnSdxLru, SGsAudmgjmvcUXzzrUtf); 
fclose(SUleyRyvAggTFnSdxLru); } void MWRNNLayerImpl::loadInitialCellState(const char* 
SGsAudmgjmvcUXzzrUtf) { MWRNNLayer* rnnLayer = 
static_cast<MWRNNLayer*>(getLayer()); int hDaNSVZAofAENeIAiWEw = 
rnnLayer->numLayers; bool atVCyzqXZAZxwlkRLBRA = 
rnnLayer->isBidirectional; int NtWaRGCHLeTapjWdEHHS = 
atVCyzqXZAZxwlkRLBRA ? 2 : 1; RNNMode puSFZkRJmyuFPfQRswDK = 
rnnLayer->rnnMode; assert(puSFZkRJmyuFPfQRswDK == LSTM); int 
vFNECEAeLZsYsUxvlgqL = NtWaRGCHLeTapjWdEHHS * 
hDaNSVZAofAENeIAiWEw * gzSTokDHvkXefhiGDcWL; FILE* SUleyRyvAggTFnSdxLru = 
MWCNNLayer::openBinaryFile(SGsAudmgjmvcUXzzrUtf); if (SUleyRyvAggTFnSdxLru == nullptr) 
{ std::cerr << "Unable to open initial cell state file" << std::endl; } 
QwUuNuQNtlPXrIwRNiSZ = std::shared_ptr<float>(new 
float[vFNECEAeLZsYsUxvlgqL]); 
MWCNNLayer::call_fread(QwUuNuQNtlPXrIwRNiSZ.get(), sizeof(float), 
vFNECEAeLZsYsUxvlgqL, SUleyRyvAggTFnSdxLru, SGsAudmgjmvcUXzzrUtf); 
fclose(SUleyRyvAggTFnSdxLru); } void MWRNNLayerImpl::prepareInitialState(float* src, 
float* dest) { MWRNNLayer* rnnLayer = static_cast<MWRNNLayer*>(getLayer()); int 
hDaNSVZAofAENeIAiWEw = rnnLayer->numLayers; bool atVCyzqXZAZxwlkRLBRA = 
rnnLayer->isBidirectional; int NtWaRGCHLeTapjWdEHHS = 
atVCyzqXZAZxwlkRLBRA ? 2 : 1; int batchSize = 
rnnLayer->getOutputTensor(0)->getBatchSize(); for (int j = 0; j < 
(hDaNSVZAofAENeIAiWEw * NtWaRGCHLeTapjWdEHHS); j++) { for (int i = 0; i < 
batchSize; i++) { memcpy(dest + i * gzSTokDHvkXefhiGDcWL + j * 
gzSTokDHvkXefhiGDcWL * batchSize, src + j * gzSTokDHvkXefhiGDcWL, 
gzSTokDHvkXefhiGDcWL * sizeof(float)); } } }