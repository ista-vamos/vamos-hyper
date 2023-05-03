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

struct TTransformer : public TraceTransformer {
  TTransformer(TracesPipeline &TP,
               const std::initializer_list<Trace*>& in) : TraceTransformer(TP, in) {
    assert(in.size() == 2);

    // create outputs
    _output_traces.reserve(4);
    for (int i = 0; i < 4; ++i) {
      auto *trace = new LocalTrace<TEvent>(TP);
      _output_traces.emplace_back(trace);
      outputs.push_back(&trace->createConsumer());
    }
  }

  StepResult step_impl() override {
    assert(inputs.size() == 2);
    StepResult result = StepResult::None;

    for (int i = 0; i < 2; ++i) {
      if (auto *e = inputs[i]->get()) {
          _output_traces[2*i+0]->push(*e);
          _output_traces[2*i+1]->push(*e);

          inputs[i]->consume();
          result = StepResult::Progress;
      }
    }
    return result;
  }

  StepResult last_step_impl() override { return StepResult::None; }
};

int main() {
  TracesPipeline TP;

  Trace *trace = new LocalTrace<TEvent>(TP);
  Trace *trace2 = new LocalTrace<TEvent>(TP);
  for (int x = 1; x < 10; ++x) {
    trace->push(Event_A(x, x));
  }
  trace->setFinished();

  for (int x = 10; x < 20; ++x) {
    trace2->push(Event_B(x, x + 0.5));
  }
  trace2->setFinished();

  TraceTransformer *T = new TTransformer(TP, {trace, trace2});

  size_t n[4] = {0};
  while (!T->ended()) {
    T->step();
    for (int i = 0; i < 4; ++i) {
      if (T->hasOutputOn(i)) {
        Event *e = T->acquireOutputOn(i);
        assert(e && "No event event when hasOutputOn() == true");
        std::cout << "out " << i << "[" << n[i] << "]: ";

        if (e->kind() == (vms_kind)Kind::A) {
          std::cout << static_cast<Event_A *>(e)->x() << "\n";
        } else if (e->kind() == (vms_kind)Kind::B) {
          std::cout << static_cast<Event_B *>(e)->x() << "\n";
        }
        ++n[i];
        T->consumeOutputOn(i);
      }
    }
  }
  /*
  while (Event *e = trace->get()) {
    if (e->kind() == (vms_kind)Kind::A) {
      std::cout << static_cast<Event_A *>(e)->x() << "\n";
    } else if (e->kind() == (vms_kind)Kind::B) {
      std::cout << static_cast<Event_B *>(e)->x() << "\n";
    }
    trace->consume();
  }
  assert(!trace->has() && "Trace still has some events");
  */

  delete T;
  delete trace;
  delete trace2;
}
