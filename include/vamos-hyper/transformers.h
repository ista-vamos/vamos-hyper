#ifndef VAMOS_HYPER_TRANSFORMERS_H
#define VAMOS_HYPER_TRANSFORMERS_H

#include <cassert>
#include <cstdint>
#include <vector>

#include <vamos-buffers/cpp/event.h>
#include <vamos-hyper/trace.h>
#include <vamos-hyper/localtrace.h>

namespace vamos {
namespace hyper {

class Trace;
class TracesPipeline;

enum class StepResult { None, Progress, Failed, Succeeded };

using TransformerID = uint64_t;

class Transformer {
  static TransformerID next_transformer_id;
  TransformerID _id;

protected:
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
    setInputs(in);
  }

  virtual ~TraceTransformer();

  void setInputs(const std::initializer_list<Trace *> &in) {
    inputs.clear();
    for (auto *itrace : in) {
      inputs.push_back(&itrace->createConsumer());
    }
  }

  template <typename EventTy>
  TraceConsumer& addOutput() {
      auto *trace = new LocalTrace<EventTy>(TP);
      _output_traces.emplace_back(trace);
      outputs.push_back(&trace->createConsumer());

      return *outputs.back();
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

// TraceTransformer with N inputs and M outputs (fixed numbers)
template <size_t inNum, size_t outNum, typename OutEventTy>
class TraceTransformerNM : public TraceTransformer {
public:
    TraceTransformerNM(TracesPipeline &TP,
                       const std::initializer_list<Trace*>& ins)
        : TraceTransformer(TP, ins) {
        assert(ins.size() == inNum);
        assert(inputs.size() == inNum);

        // FIXME: for this class, where the number of inputs and outputs
        // is fixed, we could use an array instead of vector
        _output_traces.reserve(outNum);
        outputs.reserve(outNum);

        for (size_t i = 0; i < outNum; ++i) {
            addOutput<OutEventTy>();
        }
    }
};

class HyperTraceTransformer {

public:
};

} // namespace hyper
} // namespace vamos

#endif // VAMOS_HYPER_TRACE_H
