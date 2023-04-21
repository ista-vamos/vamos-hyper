#include <iostream>

#include <vamos-buffers/cpp/event.h>
#include <vamos-hyper/localtrace.h>
#include <vamos-hyper/vamos-hyper.h>

using namespace vamos::hyper;
using vamos::Event;

enum class Kind : vms_kind {
  A,
  B,
};

struct TEvent : Event {
  union {
    struct {
      int x;
    } A;
    struct {
      float x;
    } B;
  } data;

  TEvent() = default;
  TEvent(Kind k, vms_eventid id) : Event((vms_kind)k, id) {}
};

struct Event_A : public TEvent {
  Event_A() = default;
  Event_A(vms_eventid id, int x) : TEvent(Kind::A, id) { data.A.x = x; }

  int x() const { return data.A.x; }
};

struct Event_B : public TEvent {
  Event_B() = default;
  Event_B(vms_eventid id, float x) : TEvent(Kind::B, id) { data.B.x = x; }

  float x() const { return data.B.x; }
};

int main() {
  TracesPipeline TP;

  Trace *trace = new LocalTrace<TEvent>(TP);
  for (int x = 1; x < 10; ++x) {
    trace->push(Event_A(x, x));
  }
  for (int x = 10; x < 20; ++x) {
    trace->push(Event_B(x, x + 0.5));
  }

  while (Event *e = trace->get()) {
    if (e->kind() == (vms_kind)Kind::A) {
      std::cout << static_cast<Event_A *>(e)->x() << "\n";
    } else if (e->kind() == (vms_kind)Kind::B) {
      std::cout << static_cast<Event_B *>(e)->x() << "\n";
    }
    trace->consume();
  }
  assert(!trace->has() && "Trace still has some events");

  delete trace;
}
