#include <cassert>

#include "monitor_od.h"
#include "od_events.h"

void MString::append(const MString::Letter &l) {
  if (_data.empty()) {
    assert(l.start != MString::Letter::BOT);
    _data.push_back(l);
    return;
  }

  auto &last = _data.back();
  if (last.end == MString::Letter::BOT) {
    assert(last.end != MString::Letter::BOT);
    assert(l.start != MString::Letter::BOT);
    last.start = l.start;
  } else {
    assert(last.start != MString::Letter::BOT);
    assert(l.start != MString::Letter::BOT);
    _data.push_back(l);
  }
}


#ifdef DBG
#include <iomanip>
#include <iostream>

static const char *color_green = "\033[0;32m";
static const char *color_red = "\033[0;31m";
static const char *color_reset = "\033[0m";

std::ostream &operator<<(std::ostream &s, const TraceEvent &ev) {
  s << "TraceEvent(" << color_green << std::setw(7) << std::left
    << kindToStr((Kind)ev.kind()) << color_reset << ", " << color_red
    << std::setw(2) << std::right << ev.id()
    << color_reset
    // all data are the same, it doesn't matter how we access them
    << ", addr=" << ev.data.InputL.addr << ", value=" << ev.data.InputL.value
    << ")";

  return s;
}

std::ostream &operator<<(std::ostream &s, const MString &ev) {
  for (auto& l : ev) {
    s << "(";
    if (l.start == MString::Letter::BOT) s << "⊥"; else s << l.start;
                               s << ", ";
    if (l.end == MString::Letter::BOT) s << "⊥"; else s << l.end;
                               s << ")";
  }
  return s;
}

#endif
