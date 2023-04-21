#ifndef VAMOS_HYPER_PIPELINE_H
#define VAMOS_HYPER_PIPELINE_H

#include <algorithm>
#include <set>
#include <vector>

namespace vamos {
namespace hyper {

class Trace;

class TracesPipeline {
  std::set<Trace *> _updated;
  std::set<Trace *> _starting_traces;
  std::vector<Trace *> _traces;

  uint64_t last_new_trace_id{0};

public:
  uint64_t newTraceID() { return ++last_new_trace_id; }

  void addTrace(Trace *t) {
    // don't expect this happen too often, so use vector
    if (std::find(_traces.begin(), _traces.end(), t) == _traces.end()) {
      _traces.push_back(t);
    }
  }

  void addStart(Trace *t) {
    addTrace(t);
    _starting_traces.insert(t);
  }

  /**
   * Register that a trace was updated with new events, so that we know
   * we should re-evaluate it in the next iteration.
   **/
  void updated(Trace *t) { _updated.insert(t); }
};

} // namespace hyper
} // namespace vamos

#endif // VAMOS_HYPER_TRACE_H
