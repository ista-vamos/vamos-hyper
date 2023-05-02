#ifndef VAMOS_HYPER_TRANSFORMERS_H
#define VAMOS_HYPER_TRANSFORMERS_H

#include <cassert>
#include <cstdint>
#include <vector>

#include <vamos-buffers/cpp/event.h>
#include <vamos-hyper/trace.h>

namespace vamos {
namespace hyper {

class Trace;
class TracesPipeline;

enum class StepResult { None, Progress, Failed, Succeeded };

using TransformerID = uint64_t;

class Transformer {
  static TransformerID next_transformer_id;
  TransformerID _id;

  TracesPipeline &TP;

public:
  Transformer(TracesPipeline &TP);
};

class TraceTransformer : public Transformer {
protected:
  std::vector<TraceConsumer *> inputs;
  std::vector<Trace *> _output_traces;
  std::vector<TraceConsumer *> outputs;

public:
  TraceTransformer(TracesPipeline &TP) : Transformer(TP) {}
  TraceTransformer(TracesPipeline &TP, const std::initializer_list<Trace *> &in)
      : Transformer(TP) {
    for (auto *itrace : in) {
      inputs.push_back(&itrace->createConsumer());
    }
  }

  virtual ~TraceTransformer();

  void setInputs(const std::initializer_list<Trace *> &in) {
    inputs.clear();
    for (auto *itrace : in) {
      inputs.push_back(&itrace->createConsumer());
    }
  }

  bool hasOutputOn(size_t idx);
  Event *acquireOutputOn(size_t idx);
  void consumeOutputOn(size_t idx);

  size_t positionOn(size_t idx) const;
  size_t absolutePositionOn(size_t idx) const;

  virtual StepResult step() = 0;
  virtual StepResult last_step() = 0;
};

class HyperTraceTransformer {

public:
};

} // namespace hyper
} // namespace vamos

#endif // VAMOS_HYPER_TRACE_H
