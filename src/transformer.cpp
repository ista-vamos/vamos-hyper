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

TraceTransformer::~TraceTransformer() {
    for (auto *consumer : inputs) {
      consumer->destroy();
    }

    for (auto *consumer : outputs) {
      consumer->destroy();
    }
}

bool TraceTransformer::hasOutputOn(size_t idx) {
  assert(outputs.size() > idx);
  return outputs[idx]->has();
}

Event *TraceTransformer::acquireOutputOn(size_t idx) {
  assert(outputs.size() > idx);
  assert(outputs[idx]->has() && "Acquiring output without having it");
  return outputs[idx]->get();
}

void TraceTransformer::consumeOutputOn(size_t idx) {
  assert(outputs.size() > idx);
  return outputs[idx]->consume();
}

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
