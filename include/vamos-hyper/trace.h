#ifndef VAMOS_HYPER_TRACE_H
#define VAMOS_HYPER_TRACE_H

#include <vamos-buffers/cpp/event.h>

namespace vamos {
namespace hyper {

using TraceID = uint64_t;

class TracesPipeline;

class Trace {
    TraceID _id;

protected:

    TracesPipeline &TP;

public:
    Trace(TracesPipeline& TP);
    virtual ~Trace() = default;

    /* size = the real size of the event */
    bool push(const Event &event, size_t size);

    /* override this method to push a new event into the trace */
    virtual bool push_impl(const Event &event, size_t size) = 0;

    virtual Event *get(size_t idx = 0) = 0;
    //virtual const Event *get(size_t idx = 0) const = 0;

    virtual void consume(size_t n = 1) = 0;

    virtual bool has(uint64_t num = 1) = 0;
};


// Trace with a pointer where are we
class TraceWithCursor {
    Trace &trace;
    uint64_t pos{0};

public:
    TraceWithCursor(Trace &trace) : trace(trace) {}
    bool has(uint64_t num = 1) const { return trace.has(num); };

};

} // namespace hyper
} // namespace vamos

#endif // VAMOS_HYPER_TRACE_H
