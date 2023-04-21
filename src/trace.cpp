#include <vamos-hyper/trace.h>
#include <vamos-hyper/pipeline.h>

namespace vamos {
namespace hyper {


Trace::Trace(TracesPipeline& TP): _id(TP.newTraceID()), TP(TP)  {}

void Trace::updateTP() {
    TP.updated(this);
}

} // namespace hyper
} // namespace vamos
