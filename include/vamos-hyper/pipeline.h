#ifndef VAMOS_HYPER_PIPELINE_H
#define VAMOS_HYPER_PIPELINE_H

#include <algorithm>
#include <cstdint>
#include <map>
#include <set>
#include <vector>

namespace vamos {
namespace hyper {

class Trace;
class Transformer;

class TracesPipeline {
    std::set<Trace *> _updated;
    std::set<Transformer *> _transformers;
    // std::map<Trace *, std::vector<Transformer *>> _trace_to_trans;

    uint64_t last_new_trace_id{0};

   public:
    uint64_t newTraceID() { return ++last_new_trace_id; }

    void addTransformer(Transformer *T) { _transformers.insert(T); }

    /**
     * Register that a trace was updated with new events, so that we know
     * we should re-evaluate it in the next iteration.
     **/
    void updated(Trace *t) { _updated.insert(t); }

    bool step();
    void run();
};

}  // namespace hyper
}  // namespace vamos

#endif  // VAMOS_HYPER_TRACE_H
