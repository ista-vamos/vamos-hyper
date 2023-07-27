#ifndef OD_CFGS_H_
#define OD_CFGS_H_

#include <cassert>
#include <vamos-buffers/cpp/event.h>

#include "monitor.h"
#include "events.h"

// #define DEBUG

enum class PEStepResult { None = 1, Accept = 2, Reject = 3 };

std::ostream &operator<<(std::ostream &s, const PEStepResult r);

class Workbag;

struct PE1 : public PrefixExpression {
  PEStepResult step(const Event *ev, size_t pos) {
    const auto *e = static_cast<const TraceEvent *>(ev);

    switch ((Kind)e->get_kind()) {
    case Kind::InputL:
    case Kind::OutputL:
#ifndef NDEBUG
      state = 1;
#endif
      match_pos = pos;
      // M.append(MString::Letter(pos, pos));
      return PEStepResult::Accept;
    default:
      assert(state == 0);
      return PEStepResult::None;
    }
  }
};

struct PE2 : public PrefixExpression {
  PEStepResult step(const Event *ev, size_t pos) {
    const auto *e = static_cast<const TraceEvent *>(ev);

    switch ((Kind)e->get_kind()) {
    case Kind::OutputL:
    case Kind::End:
#ifndef NDEBUG
      state = 1;
#endif
      //M.append(MString::Letter(pos, pos));
      match_pos = pos;
      return PEStepResult::Accept;
    default:
      assert(state == 0);
      return PEStepResult::None;
    }
  }
};

struct PE3 : public PrefixExpression {
  PEStepResult step(const Event *ev, size_t pos) {
    const auto *e = static_cast<const TraceEvent *>(ev);

    switch ((Kind)e->get_kind()) {
    case Kind::InputL:
    case Kind::End:
#ifndef NDEBUG
      state = 1;
#endif
      match_pos = pos;
      //M.append(MString::Letter(pos, pos));
      return PEStepResult::Accept;
    default:
      assert(state == 0);
      return PEStepResult::None;
    }
  }
};

struct mPE_1 {
  PE1 _exprs[2];
  bool _accepted[2] = {false, false};

  bool accepted(size_t idx) const { return _accepted[idx]; }
  bool accepted() const { return _accepted[0] && _accepted[1]; }

  PEStepResult step(size_t idx, const Event *ev, size_t pos) {
    assert(idx < 2);
    auto res = _exprs[idx].step(ev, pos);
    if (res == PEStepResult::Accept)
      _accepted[idx] = true;
    return res;
  }

  template <typename TraceT> bool cond(TraceT *t1, TraceT *t2) const {
    return *static_cast<TraceEvent *>(t1->get(_exprs[0].match_pos))
            == *static_cast<TraceEvent *>(t2->get(_exprs[1].match_pos));
  }
};

struct mPE_2 {
  PE2 _exprs[2];
  bool _accepted[2] = {false, false};

  bool accepted(size_t idx) const { return _accepted[idx]; }
  bool accepted() const { return _accepted[0] && _accepted[1]; }

  PEStepResult step(size_t idx, const Event *ev, size_t pos) {
    assert(idx < 2);
    auto res = _exprs[idx].step(ev, pos);
    if (res == PEStepResult::Accept)
      _accepted[idx] = true;
    return res;
  }

  template <typename TraceT> bool cond(TraceT *t1, TraceT *t2) const {
    return *static_cast<TraceEvent *>(t1->get(_exprs[0].match_pos))
            != *static_cast<TraceEvent *>(t2->get(_exprs[1].match_pos));
  }
};

struct mPE_3 {

  PE3 _exprs[2];
  bool _accepted[2] = {false, false};

  bool accepted(size_t idx) const { return _accepted[idx]; }
  bool accepted() const { return _accepted[0] && _accepted[1]; }

  PEStepResult step(size_t idx, const Event *ev, size_t pos) {
    assert(idx < 2);
    auto res = _exprs[idx].step(ev, pos);
    if (res == PEStepResult::Accept)
      _accepted[idx] = true;
    return res;
  }

  template <typename TraceT> bool cond(TraceT *t1, TraceT *t2) const {
    return *static_cast<TraceEvent *>(t1->get(_exprs[0].match_pos))
            != *static_cast<TraceEvent *>(t2->get(_exprs[1].match_pos));
  }
};






class ConfigurationBase {};

template <typename TraceTy>
class Configuration : public ConfigurationBase {

protected:
  bool _failed{false};
  size_t positions[2] = {0};
  TraceTy * traces[2];

public:
  Configuration() {}
  // Configuration& operator=(const Configuration&) = default;
  Configuration(TraceTy *tr[2]) {
    traces[0] = tr[0];
    traces[1] = tr[1];
  }

  Configuration(TraceTy *tr0, TraceTy *tr1) {
    traces[0] = tr0;
    traces[1] = tr1;
  }

  TraceTy *trace(size_t idx) { return traces[idx]; }
  const TraceTy *trace(size_t idx) const { return traces[idx]; }

  bool failed() const { return _failed; }
};

template <typename MpeTy>
class CfgTemplate : public Configuration<Trace<TraceEvent>> {

protected:
  MpeTy mPE{};

public:
  bool canProceed(size_t idx) const {
    return !mPE.accepted(idx) && trace(idx)->size() > positions[idx];
  }

  void queueNextConfigurations(Workbag &) {
      /* no next configurations by default*/
  }

  PEStepResult step(size_t idx) {
    assert(canProceed(idx) && "Step on invalid PE");
    assert(!_failed);

    const Event *ev = trace(idx)->get(positions[idx]);
    assert(ev && "No event");
    auto res = mPE.step(idx, ev, positions[idx]);

#ifdef DEBUG
#ifdef DEBUG_CFGS
    std::cout << "(𝜏" << idx << ") t" << trace(idx)->id()
              << "[" << positions[idx] << "]"
              << "@" << *static_cast<const TraceEvent *>(ev) << ", "
              << positions[idx] << " => " << res << "\n";
#endif
#endif

    ++positions[idx];

    switch (res) {
    case PEStepResult::Accept:
      if (mPE.accepted()) {
        // std::cout << "mPE matched prefixes\n";
        if (mPE.cond(trace(0), trace(1))) {
          // std::cout << "Condition SAT!\n";
          return PEStepResult::Accept;
        } else {
          // std::cout << "Condition UNSAT!\n";
          _failed = true;
          return PEStepResult::Reject;
        }
      }
      return PEStepResult::None;
    case PEStepResult::Reject:
      _failed = true;
      // fall-through
    default:
      return res;
    }
  }

  CfgTemplate() {}
  // CfgTemplate& operator=(const CfgTemplate&) = default;
  CfgTemplate(Trace<TraceEvent> *traces[2])
      : Configuration(traces) {}

  CfgTemplate(Trace<TraceEvent> *t0, Trace<TraceEvent> *t1)
      : Configuration(t0, t1) {}

  CfgTemplate(Trace<TraceEvent> *traces[2],
              const size_t pos[2])
      : Configuration(traces) {
    positions[0] = pos[0];
    positions[1] = pos[1];
  }
};

struct Cfg_1 : public CfgTemplate<mPE_1> {
#ifdef DEBUG
#ifdef DEBUG_CFGS
  static size_t __id;
  size_t _id;

#define INIT_ID (_id = ++__id)

  std::string name() const {
      return "cfg_1#" + std::to_string(_id);
  }
#endif
#else
#define INIT_ID
#endif

  Cfg_1(){ INIT_ID; };
  // Cfg_1& operator=(const Cfg_1&) = default;
  Cfg_1(Trace<TraceEvent> *traces[2])
      : CfgTemplate(traces) { INIT_ID; }

  Cfg_1(Trace<TraceEvent> *t0, Trace<TraceEvent> *t1)
      : CfgTemplate(t0, t1) { INIT_ID; }

  Cfg_1(Trace<TraceEvent> *traces[2], const size_t pos[2])
      :  CfgTemplate(traces, pos) { INIT_ID; }

  void queueNextConfigurations(Workbag &);

};

struct Cfg_2 : public CfgTemplate<mPE_2> {
#ifdef DEBUG
#ifdef DEBUG_CFGS
  static size_t __id;
  size_t _id;

#define INIT_ID (_id = ++__id)

  std::string name() const {
      return "cfg_2#" + std::to_string(_id);
  }
#endif
#else
#define INIT_ID
#endif

  Cfg_2(){ INIT_ID; };
  Cfg_2(Trace<TraceEvent> *traces[2])
      : CfgTemplate(traces) { INIT_ID; }

  Cfg_2(Trace<TraceEvent> *t0, Trace<TraceEvent> *t1)
      : CfgTemplate(t0, t1) { INIT_ID; }
  Cfg_2(Trace<TraceEvent> *traces[2], const size_t pos[2])
      : CfgTemplate(traces, pos) { INIT_ID; }
};


struct Cfg_3 : public CfgTemplate<mPE_3> {
#ifdef DEBUG
#ifdef DEBUG_CFGS
  static size_t __id;
  size_t _id;

#define INIT_ID (_id = ++__id)

  std::string name() const {
      return "cfg_3#" + std::to_string(_id);
  }
#endif
#else
#define INIT_ID
#endif
  Cfg_3(){ INIT_ID; }
  Cfg_3(Trace<TraceEvent> *traces[2])
      : CfgTemplate(traces) { INIT_ID; }

  Cfg_3(Trace<TraceEvent> *t0, Trace<TraceEvent> *t1)
      : CfgTemplate(t0, t1) { INIT_ID; }
  Cfg_3(Trace<TraceEvent> *traces[2], const size_t pos[2])
      : CfgTemplate(traces, pos) { INIT_ID; }
};

#endif
