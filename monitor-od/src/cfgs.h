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

/*
template <typename TraceT, typename MStringT>
bool match_eq(TraceT *t1, const MStringT &m1, TraceT *t2, const MStringT &m2) {
  assert(!m1.empty() && !m2.empty());

  // std::cout << "match_eq: " << m1 << ", " << m2 << "\n";

  auto pos1 = m1[0].start;
  auto pos2 = m2[0].start;
#ifndef NDEBUG
  const auto Bot = MString::Letter::BOT;
#endif
  size_t m1i = 0;
  size_t m2i = 0;

  while (true) {
    assert(pos1 != Bot);
    assert(pos2 != Bot);
    if (*static_cast<TraceEvent *>(t1->get(pos1)) !=
        *static_cast<TraceEvent *>(t2->get(pos2)))
      return false;

    if (pos1 == m1[m1i].end) {
      ++m1i;
      if (m1.size() == m1i) { // no more positions in m1
        if (pos2 == m2[m2i].end && m2.size() == m2i + 1) {
          // m2 ended as well
          return true;
        } else {
          return false;
        }
      }
      pos1 = m1[m1i].start;
    }
    if (pos2 == m2[m2i].end) {
      ++m2i;
      if (m2.size() == m2i) { // no more positions in m2
        if (pos1 == m1[m1i].end && m1.size() == m1i + 1) {
          // m1 ended as well
          return true;
        } else {
          return false;
        }
      }
      pos2 = m2[m2i].start;
    }
  }

  assert(false && "Unreachable");
  abort();
}
*/

struct PE1 : public PrefixExpression {
  PEStepResult step(const Event *ev, size_t pos) {
    const auto *e = static_cast<const TraceEvent *>(ev);

    switch ((Kind)e->kind()) {
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

    switch ((Kind)e->kind()) {
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

    switch ((Kind)e->kind()) {
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

template <typename TraceTy, size_t K>
class Configuration : public ConfigurationBase {

protected:
  bool _failed{false};
  size_t positions[K] = {0};
  std::array<TraceTy *, K> traces;

public:
  Configuration() {}
  // Configuration& operator=(const Configuration&) = default;
  Configuration(const std::array<TraceTy *, K> &tr) : traces(tr) {}

  TraceTy *trace(size_t idx) { return traces[idx]; }
  const TraceTy *trace(size_t idx) const { return traces[idx]; }

  bool failed() const { return _failed; }
};

template <typename MpeTy>
class CfgTemplate : public Configuration<Trace<TraceEvent>, 2> {

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
  CfgTemplate(const std::array<Trace<TraceEvent> *, 2> &traces)
      : Configuration(traces) {}

  CfgTemplate(const std::array<Trace<TraceEvent> *, 2> &traces,
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
  Cfg_1(const std::array<Trace<TraceEvent> *, 2> &traces)
      : CfgTemplate(traces) { INIT_ID; }

  Cfg_1(const std::array<Trace<TraceEvent> *, 2> &traces, const size_t pos[2])
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
  Cfg_2(const std::array<Trace<TraceEvent> *, 2> &traces)
      : CfgTemplate(traces) { INIT_ID; }

  Cfg_2(const std::array<Trace<TraceEvent> *, 2> &traces, const size_t pos[2])
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
  Cfg_3(const std::array<Trace<TraceEvent> *, 2> &traces)
      : CfgTemplate(traces) { INIT_ID; }

  Cfg_3(const std::array<Trace<TraceEvent> *, 2> &traces, const size_t pos[2])
      : CfgTemplate(traces, pos) { INIT_ID; }
};

#endif
