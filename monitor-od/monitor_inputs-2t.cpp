#include <cassert>
#include <cstdlib>
#include <ctime>

#include "src/monitor.h"
#include "src/events.h"

int x;

bool InputStream::hasEvent() const {
  const size_t &pos = reinterpret_cast<const size_t &>(data[1]);
  const size_t len = reinterpret_cast<const size_t>(data[2]);
  return pos < len;
}

bool InputStream::isDone() const {
  const size_t &pos = reinterpret_cast<const size_t &>(data[1]);
  const size_t len = reinterpret_cast<const size_t>(data[2]);
  return pos >= len;
}

Event *InputStream::getEvent() {
  assert(hasEvent() && "getEvent() when there is no event");

  size_t &pos = reinterpret_cast<size_t &>(data[1]);
  size_t start = reinterpret_cast<size_t>(data[1]);

  ++pos;

  static Event_InputL  I(0, 0, 0);
  static Event_OutputL O(0, 0, 0);
  static Event_Write   W(0, 0, 0);

  if (pos >= start) {
      if (pos % (2*DIST) == 0) {
        O = Event_OutputL(pos, &x, 1);
        return &O;
      } else if (pos % DIST == 0) {
        I = Event_InputL(pos, &x, 1);
        return &I;
      }
  }

  W = Event_Write(pos, &x, pos);
  return &W;
}

Inputs::Inputs() {
  size_t &returned = reinterpret_cast<size_t &>(data[0]);
  returned = 0;
}

bool Inputs::done() const {
  const size_t &returned = reinterpret_cast<const size_t &>(data[0]);
  return returned >= NUM_STREAMS;
}

InputStream *Inputs::getNewInputStream() {
  size_t &returned = reinterpret_cast<size_t &>(data[0]);
  if (returned >= NUM_STREAMS)
    return nullptr;

  auto *stream = new InputStream(_streams.size());
  _streams.emplace_back(stream);

  static size_t stream_ty = 0;

  //*reinterpret_cast<TraceEvent **>(&stream->data[0]) = streams[returned % 2];
  *reinterpret_cast<size_t *>(&stream->data[1]) = 0;
  *reinterpret_cast<size_t *>(&stream->data[2]) = STREAM_LEN;
  *reinterpret_cast<size_t *>(&stream->data[3]) = (stream_ty++ % 2 == 0) ? 0 : SPOS;

  ++returned;

  return stream;
}
