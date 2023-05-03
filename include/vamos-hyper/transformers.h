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

  bool inputsEnded() {
      for (auto *tc : inputs) {
          if (!tc->ended())
              return false;
      }
      return true;
  }

protected:
  std::vector<TraceConsumer *> inputs;
  std::vector<std::unique_ptr<Trace>> _output_traces;
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

  bool ended() {
      for (auto &t : _output_traces) {
          if (!t->ended())
              return false;
      }
      return true;
  }

  bool hasOutputOn(size_t idx);
  Event *acquireOutputOn(size_t idx);
  void consumeOutputOn(size_t idx);

  size_t positionOn(size_t idx) const;
  size_t absolutePositionOn(size_t idx) const;

  StepResult step() {
      StepResult result = step_impl();

      switch (result) {
      case StepResult::Failed:
      case StepResult::Succeeded:
          for (auto &t : _output_traces) {
              t->setTerminated();
          }
          break;
      case StepResult::Progress:
          if (inputsEnded()) {
              for (auto &t : _output_traces) {
                  t->setFinished();
              }
          }
      default: break;
      }

      return result;
  }

  virtual StepResult step_impl() = 0;
  virtual StepResult last_step_impl() { return StepResult::None; }
};

class HyperTraceTransformer {

public:
};

} // namespace hyper
} // namespace vamos

#endif // VAMOS_HYPER_TRACE_H
