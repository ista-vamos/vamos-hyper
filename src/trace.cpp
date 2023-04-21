#include <vamos-hyper/pipeline.h>
#include <vamos-hyper/trace.h>

namespace vamos {
namespace hyper {

Trace::Trace(TracesPipeline &TP) : _id(TP.newTraceID()), TP(TP) {}

void Trace::updateTP() { TP.updated(this); }

} // namespace hyper
} // namespace vamos
