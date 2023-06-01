#include <cassert>

#include "monitor_od.h"
#include "od_events.h"

int x;

#define NUM_STREAMS 1000
#define MAX_NUM_EVS 23
const size_t lens[] = {20, MAX_NUM_EVS};
TraceEvent streams[][MAX_NUM_EVS] = {
    {
        Event_Write(4, &x, 5),
        Event_Write(4, &x, 5),
        Event_InputL(1, &x, 1),
        Event_Write(4, &x, 5),
        Event_Write(4, &x, 5),
        Event_Write(4, &x, 5),
        Event_Write(4, &x, 5),
        Event_Write(4, &x, 5),
        Event_Write(4, &x, 5),
        Event_InputL(1, &x, 1),
        Event_Write(4, &x, 5),
        Event_Write(4, &x, 5),
        Event_InputL(1, &x, 1),
        Event_Write(4, &x, 5),
        Event_OutputL(1, &x, 1),
        Event_Write(4, &x, 5),
        Event_Write(4, &x, 5),
        Event_Write(4, &x, 5),
        Event_Write(4, &x, 5),
        Event_Write(4, &x, 5),
    },
    {
        Event_Write(4, &x, 5),  Event_Write(4, &x, 5),   Event_Write(4, &x, 5),
        Event_Write(4, &x, 5),  Event_Write(4, &x, 5),   Event_Write(4, &x, 5),
        Event_InputL(1, &x, 1), Event_Write(4, &x, 5),   Event_Write(4, &x, 5),
        Event_InputL(1, &x, 1), Event_Write(4, &x, 5),   Event_Write(4, &x, 5),
        Event_Write(4, &x, 5),  Event_Write(4, &x, 5),   Event_Write(4, &x, 5),
        Event_Write(4, &x, 5),  Event_InputL(1, &x, 1),  Event_Write(4, &x, 5),
        Event_Write(4, &x, 5),  Event_Write(4, &x, 5),   Event_Write(4, &x, 5),
        Event_Write(4, &x, 5),  Event_OutputL(1, &x, 1),
    },
};

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

  TraceEvent *events = static_cast<TraceEvent *>(data[0]);
  size_t &pos = reinterpret_cast<size_t &>(data[1]);

  return &events[pos++];
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

  *reinterpret_cast<TraceEvent **>(&stream->data[0]) = streams[returned % 2];
  *reinterpret_cast<size_t *>(&stream->data[1]) = 0;
  *reinterpret_cast<size_t *>(&stream->data[2]) = lens[returned % 2];

  ++returned;

  return stream;
}
