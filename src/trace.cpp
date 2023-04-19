#include <vamos-hyper/trace.h>
#include <vamos-hyper/pipeline.h>

namespace vamos {
namespace hyper {


Trace::Trace(TracesPipeline& TP): _id(TP.newTraceID()), TP(TP)  {}

bool Trace::push(const Event &event, size_t size) {
    TP.updated(this);
    return push_impl(event, size);
}

} // namespace hyper
} // namespace vamos
