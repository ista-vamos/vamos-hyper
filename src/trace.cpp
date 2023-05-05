#include <cassert>

#include <vamos-hyper/pipeline.h>
#include <vamos-hyper/trace.h>

#include <vamos-hyper/debug.h> // DEBUG

namespace vamos {
namespace hyper {

Trace::Trace(TracesPipeline &TP) : _id(TP.newTraceID()), TP(TP) {}

void Trace::updateTP() { TP.updated(this); }

bool TraceConsumer::ended() { return trace->ended(); }

bool TraceConsumer::has(size_t num) {
  assert(trace);
  return trace->has(_pos + num);
}

Event *TraceConsumer::get(size_t idx) {
  assert(trace);
  return trace->get(_pos + idx);
}

void TraceConsumer::consume(size_t n) {
  assert(trace);

  if (_pos == 0) {
    ++trace->_consumers_read_count;
  }

  _absolute_pos += n;
  _pos += n;

  if (trace->_consumers_read_count >= trace->_consumers.size()) {
    size_t min = ~((size_t)0);
    for (auto &tc : trace->_consumers) {
      if (tc->trace != nullptr && tc->_pos < min) {
        min = tc->_pos;
      }
    }

    trace->_consumers_read_count = 0;
    for (auto &tc : trace->_consumers) {
      assert(tc->_pos >= min);
      tc->_pos -= min;
      if (tc->_pos > 0) {
        ++trace->_consumers_read_count;
      }
    }

    assert(min > 0);
    trace->consume(min);
  }
}

TraceConsumer::~TraceConsumer() {
#ifndef NDEBUG
  trace = reinterpret_cast<Trace *>(0xdeadbeef);
  _pos = 0xdeadbeef;
#endif
}

void TraceConsumer::dump() const {
  dbg("TraceConsumer(trace=" + std::to_string(trace->getID()) +
      ", _idx=" + std::to_string(_idx) + ")");
}

} // namespace hyper
} // namespace vamos
