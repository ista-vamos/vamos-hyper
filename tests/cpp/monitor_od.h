#ifndef MONITOR_OD_H
#define MONITOR_OD_H

#include <array>
#include <memory>
#include <set>
#include <vector>

#include <vamos-buffers/cpp/event.h>

using vamos::Event;

class TraceBase {
  const size_t _id;
  bool _done{false};

public:
  TraceBase(size_t id) : _id(id) {}
  void setDone() { _done = true; }

  bool done() const { return _done; }
  size_t id() const { return _id; }
};

template <typename EventTy> class Trace : public TraceBase {
  std::vector<EventTy> _events;

public:
  Trace(size_t id) : TraceBase(id) {}

  void append(const EventTy *e) { _events.push_back(*e); }
  void append(const EventTy &e) { _events.push_back(e); };

  Event *get(size_t idx) { return &_events[idx]; }
  const Event *get(size_t idx) const { return &_events[idx]; }

  Event *operator[](size_t idx) { return get(idx); }
  const Event *operator[](size_t idx) const { return get(idx); }

  size_t size() const { return _events.size(); }
};

class Inputs;

///
/// \brief The InputStream class
/// Stream of events that are input to the monitor.
/// This is an opaque object, basically an iterator over incoming events.
/// The incoming events are stored into the trace that is referenced in
/// InputStream.
class InputStream {
protected:
  const size_t _id;
  TraceBase *_trace;
  // this is ugly but efficient, we'll reinterpret cast the data into what we
  // need
  void *data[4];

  friend class Inputs;

public:
  InputStream(size_t id) : _id(id) {}

  size_t id() const { return _id; }
  void setTrace(TraceBase *t) { _trace = t; }

  TraceBase *trace() { return _trace; }
  const TraceBase *trace() const { return _trace; }

  Event *getEvent();
  const Event *getEvent() const;

  bool hasEvent() const;
  bool isDone() const;
};

class Inputs {
  std::vector<std::unique_ptr<InputStream>> _streams;

  // this is ugly but efficient, we'll reinterpret cast the data into what we
  // need
  void *data[4];

public:
  Inputs();

  InputStream *getNewInputStream();
  bool done() const;
};

class MString {
public:
  struct Letter {
    static const size_t BOT = ~static_cast<size_t>(0);

    size_t start;
    size_t end;
    Letter(size_t s, size_t e) : start(s), end(e) {}

    bool operator==(const Letter &rhs) const {
      return start == rhs.start && end == rhs.end;
    }
    bool operator!=(const Letter &rhs) const { return !(*this == rhs); }
  };

  void append(const MString::Letter &l);
  bool empty() const { return _data.empty(); }
  size_t size() const { return _data.size(); }

  bool operator==(const MString &rhs) const { return _data == rhs._data; }
  bool operator!=(const MString &rhs) const { return _data != rhs._data; }

  Letter &operator[](size_t idx) { return _data[idx]; }
  Letter operator[](size_t idx) const { return _data[idx]; }

  auto begin() -> auto { return _data.begin(); }
  auto end() -> auto { return _data.end(); }
  auto begin() const -> auto { return _data.begin(); }
  auto end() const -> auto { return _data.end(); }

private:
  std::vector<Letter> _data;

  friend std::ostream &operator<<(std::ostream &s, const MString &ev);
};

#define DBG
#ifdef DBG
#include <iostream>
std::ostream &operator<<(std::ostream &s, const MString &ev);
#endif

struct PrefixExpression {
  size_t state{0};
  MString M;
};

template <size_t K> struct MultiTracePrefixExpression {
  bool _accepted[K]{false};
  std::array<PrefixExpression, K> _exprs;

  MultiTracePrefixExpression(const std::array<PrefixExpression, K> &PEs)
      : _exprs(PEs) {}

  bool cond() const;
  bool accepted(size_t idx) const { return _accepted[idx]; }
  bool accepted() const {
    for (bool a : _accepted) {
      if (!a) {
        return false;
      }
    }
    return true;
  }
};

#endif // MONITOR_OD_H