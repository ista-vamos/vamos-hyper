#ifndef VAMOS_HYPER_TRANSFORMERS_H
#define VAMOS_HYPER_TRANSFORMERS_H

namespace vamos {
namespace hyper {

enum class StepResult { None, Progress, Failed, Succeeded };

class TraceTransformer {

public:
  virtual StepResult step() = 0;
  virtual StepResult last_step() = 0;
};

class HyperTraceTransformer {

public:
};

} // namespace hyper
} // namespace vamos

#endif // VAMOS_HYPER_TRACE_H
