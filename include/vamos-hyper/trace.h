#ifndef VAMOS_HYPER_TRACE_H
#define VAMOS_HYPER_TRACE_H

#include <cassert>
#include <memory>
#include <vector>

#include <vamos-buffers/cpp/event.h>
#include <vamos-hyper/debug.h>

namespace vamos {
namespace hyper {

using TraceID = uint64_t;

class TracesPipeline;
class Trace;

// Trace with a pointer for reading.
// A trace can be accessed via multiple consumers,
// so every consumer must holds its own reading head.
// When all consumers read at least 'n' elements
// from the trace, only then these 'n' elements can be consumed
// from the trace
class TraceConsumer {
  friend class Trace;

  Trace *trace;
  size_t _pos{0};
  size_t _absolute_pos{0};
  // the index of the trace consumer in the _consumers field of the trace
  const size_t _idx;

public:
  TraceConsumer(Trace *trace, size_t idx) : trace(trace), _idx(idx) {
    assert(_pos == 0);
    dump();
  }

  TraceConsumer(TraceConsumer &&) = default;
  ~TraceConsumer();

  bool ended();

  void destroy() {
    trace = nullptr;
#ifndef NDEBUG
    _pos = -1;
#endif
  }

  bool has(uint64_t num = 1);
  Event *get(size_t idx = 0);
  void consume(size_t n = 1);
  bool hasConsumed() const { return _pos > 0; }

  size_t pos() const { return _pos; }
  size_t absolute_pos() const { return _absolute_pos; }

  void dump() const;
};

class Trace {
  friend class TraceConsumer;

  TraceID _id;

  // the cache for the number of available events to be read
  size_t _unread_num{0};

  // all consumers of this trace
  std::vector<std::unique_ptr<TraceConsumer>> _consumers;
  // the counter of reads by consumers. We use it to avoid
  // checking if we should consume something from the trace
  // after every consumer operation.
  size_t _consumers_read_count{0};

  TraceConsumer *getFreeConsumer() {
    for (auto &con : _consumers) {
      if (con->trace == nullptr)
        return con.get();
    }

    return nullptr;
  }

  bool _finished{false};
  bool _terminated{false};

protected:
  TracesPipeline &TP;

  /* override this method to push a new event into the trace */
  virtual bool push_impl(const Event &event) = 0;
  /* override this method to consume events from the trace */
  virtual void consume_impl(size_t n = 1) = 0;
  /* override this method to read an event from the trace */
  virtual Event *get_impl(size_t idx = 0) = 0;
  /* override this method to get the number of available events to read */
  virtual size_t unreadNum_impl() = 0;

public:
  Trace(TracesPipeline &TP);
  virtual ~Trace() = default;

  TraceConsumer &createConsumer() {
    if (auto *consumer = getFreeConsumer()) {
      assert(consumer->_idx < _consumers.size());
      consumer->trace = this;
      consumer->_pos = 0;
      return *consumer;
    }

    _consumers.emplace_back(new TraceConsumer(this, _consumers.size()));
    return *_consumers.back().get();
  }

  size_t getID() const { return _id; }

  size_t unreadNum() {
      _unread_num = unreadNum_impl();
      return _unread_num;
  }

  // the trace has ended when it was set as `finished` (the last elem
  // was put to the trace) and we have read all the
  bool ended() {
      if (_finished) {
          return _terminated || (_unread_num == 0 && unreadNum() == 0);
      }
      return false;
  }

  // call this after pushing the last element to the trace
  void setFinished() { _finished = true; }
  void setTerminated() {
      _finished = true;
      _terminated = true;
  }

  Event *get(size_t idx = 0) {
      if (_unread_num <= idx) {
          _unread_num = unreadNum();
      }

      Event *e = get_impl(idx);
      assert(e || _unread_num == 0);

      return e;
  }

  void consume(size_t n = 1) {
      assert(_unread_num >= n);
      _unread_num -= n;
      consume_impl(n);
  }

  bool has(size_t num = 1) {
    // we cache the results of unreadNum to avoid
    // extra indirect calls (there can be many of them)
    if (_unread_num >= num)
      return true;
    _unread_num = unreadNum();
    return _unread_num >= num;
  }

  /* size = the real size of the event */
  bool push(const Event &event) {
    updateTP();
    return push_impl(event);
  }

  void updateTP();
};

} // namespace hyper
} // namespace vamos

#endif // VAMOS_HYPER_TRACE_H
