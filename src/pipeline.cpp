#include <cassert>
#include <vamos-hyper/pipeline.h>
#include <vamos-hyper/transformers.h>

namespace vamos {
namespace hyper {

bool TracesPipeline::step() {
  assert(0 && "Not implemented");
  return false;
}

void TracesPipeline::run() {
  bool has_living = true;
  std::vector<Transformer *> dead;
  dead.reserve(4);

  while (has_living) {
    has_living = false;
    dead.clear();

    for (auto *t : _transformers) {
      StepResult result = t->step();
      if (result == StepResult::Ended) {
        dead.push_back(t);
      } else {
        has_living = true;
      }
    }

    for (auto *t : dead) {
      _transformers.erase(t);
    }
  }
}

} // namespace hyper
} // namespace vamos
