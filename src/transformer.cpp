#include <vamos-hyper/pipeline.h>
#include <vamos-hyper/trace.h>
#include <vamos-hyper/transformers.h>

namespace vamos {
namespace hyper {

TransformerID Transformer::next_transformer_id = 0;

Transformer::Transformer(TracesPipeline &TP)
    : _id(++Transformer::next_transformer_id), TP(TP) {
  TP.addTransformer(this);
}

TraceTransformer::~TraceTransformer() {}

size_t TraceTransformer::positionOn(size_t idx) const {
  assert(inputs.size() > idx);
  return inputs[idx]->pos();
}

size_t TraceTransformer::absolutePositionOn(size_t idx) const {
  assert(inputs.size() > idx);
  return inputs[idx]->absolute_pos();
}

} // namespace hyper
} // namespace vamos
