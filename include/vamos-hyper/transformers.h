#ifndef VAMOS_HYPER_TRANSFORMERS_H
#define VAMOS_HYPER_TRANSFORMERS_H

#include <vamos-buffers/cpp/event.h>
#include <vamos-hyper/localtrace.h>
#include <vamos-hyper/trace.h>

#include <cassert>
#include <cstdint>
#include <vector>

namespace vamos {
namespace hyper {

class Trace;
class TracesPipeline;

enum class StepResult {
    Waiting,    // waiting for an event
    NoEvent,    // have no event and not waiting for one
    Progress,   // produced an output
    Failed,     // transformer failed, terminate it
    Succeeded,  // transformer succeeded, terminate it
    Ended       // transformer just ended, terminate it
};

using TransformerID = uint64_t;

class Transformer {
    static TransformerID next_transformer_id;
    TransformerID _id;

   protected:
    TracesPipeline &TP;

   public:
    Transformer(TracesPipeline &TP);

    virtual StepResult step() = 0;
    virtual StepResult last_step_impl() { return StepResult::NoEvent; }
    virtual bool ended() = 0;
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
    std::vector<std::unique_ptr<Trace>> outputs;

    TraceConsumer &input(size_t idx) {
        assert(idx < inputs.size());
        return *inputs[idx];
    }

    Trace &output(size_t idx) {
        assert(idx < outputs.size());
        return *outputs[idx];
    }

   public:
    TraceTransformer(TracesPipeline &TP) : Transformer(TP) {}
    TraceTransformer(TracesPipeline &TP,
                     const std::initializer_list<Trace *> &in)
        : Transformer(TP) {
        setInputs(in);
    }
    TraceTransformer(TracesPipeline &TP, const TraceTransformer &inT)
        : Transformer(TP) {
        setInputs(inT.outputs);
    }

    virtual ~TraceTransformer();

    void setInputs(const std::initializer_list<Trace *> &in) {
        assert(inputs.empty());
        for (auto *itrace : in) {
            inputs.push_back(itrace->createConsumer());
        }
    }

    void setInputs(const std::vector<std::unique_ptr<Trace>> &in) {
        assert(inputs.empty());
        for (auto &itrace : in) {
            inputs.push_back(itrace->createConsumer());
        }
    }

    template <typename EventTy>
    void addOutput() {
        auto *trace = new LocalTrace<EventTy>(TP);
        outputs.emplace_back(trace);
    }

    template <typename EventTy>
    void addOutputs(size_t num) {
        for (size_t n = 0; n < num; ++n) {
            addOutput<EventTy>();
        }
    }

    template <>
    void addOutputs<void>(size_t) {}

    Trace *getOutputTrace(size_t idx) { return outputs[idx].get(); }

    bool ended() override {
        for (auto &t : outputs) {
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

    StepResult step() override {
        StepResult result = step_impl();

        switch (result) {
            case StepResult::Failed:
            case StepResult::Succeeded:
            case StepResult::Ended:
                for (auto &t : outputs) {
                    t->setTerminated();
                }
                break;
            case StepResult::NoEvent:
                if (inputsEnded()) {
                    for (auto &t : outputs) {
                        t->setFinished();
                    }
                    return StepResult::Ended;
                }
            case StepResult::Progress:
            default:
                break;
        }

        return result;
    }

    virtual StepResult step_impl() = 0;
};

// TraceTransformer with N inputs and M outputs (fixed numbers)
template <size_t inNum, size_t outNum, typename OutEventTy>
class TraceTransformerNM : public TraceTransformer {
    void createOutputs() {
        // FIXME: for this class, where the number of inputs and outputs
        // is fixed, we could use an array instead of vector
        if (!std::is_same<OutEventTy, void>::value && outNum > 0) {
            outputs.reserve(outNum);
            outputs.reserve(outNum);

            addOutputs<OutEventTy>(outNum);
        }
    }

   public:
    TraceTransformerNM(TracesPipeline &TP,
                       const std::initializer_list<Trace *> &ins)
        : TraceTransformer(TP, ins) {
        assert(ins.size() == inNum);
        assert(inputs.size() == inNum);
        createOutputs();
    }
    TraceTransformerNM(TracesPipeline &TP, const TraceTransformer &ins)
        : TraceTransformer(TP, ins) {
        // assert(ins.size() == inNum);
        assert(inputs.size() == inNum);
        createOutputs();
    }
};

class HyperTraceTransformer {
   public:
};

}  // namespace hyper
}  // namespace vamos

#endif  // VAMOS_HYPER_TRACE_H
