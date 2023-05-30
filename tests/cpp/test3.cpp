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

struct TTransformer : public TraceTransformerNM<2, 4, TEvent> {

  TTransformer(TracesPipeline &TP, const std::initializer_list<Trace *> &in)
      : TraceTransformerNM(TP, in) {}

  StepResult step_impl() override {
    assert(inputs.size() == 2);
    StepResult result = StepResult::NoEvent;

    for (int i = 0; i < 2; ++i) {
      if (auto *e = inputs[i]->get()) {
        outputs[2 * i + 0]->push(*e);
        outputs[2 * i + 1]->push(*e);

        inputs[i]->consume();
        result = StepResult::Progress;
      }
    }
    return result;
  }
};

struct TTransformer2 : public TraceTransformerNM<1, 1, TEvent> {

  TTransformer2(TracesPipeline &TP, const std::initializer_list<Trace *> &in)
      : TraceTransformerNM(TP, in) {}

  StepResult step_impl() override {
    assert(inputs.size() == 1);
    StepResult result = StepResult::NoEvent;

    if (auto *e = inputs[0]->get()) {
      if (e->kind() == (vms_kind)Kind::A) {
        auto *ev = static_cast<Event_A *>(e);
        outputs[0]->push(Event_A{ev->id(), ev->x() % 2});
        inputs[0]->consume();
        result = StepResult::Progress;
      }
    }
    return result;
  }
};

struct OutTransformer : public TraceTransformerNM<4, 0, void> {

  OutTransformer(TracesPipeline &TP, const TraceTransformer &in)
      : TraceTransformerNM(TP, in) {}

  StepResult step_impl() override {
    assert(inputs.size() == 2);
    StepResult result = StepResult::NoEvent;

    size_t n[4] = {0};
    for (int i = 0; i < 4; ++i) {
      if (input(i).has()) {
        Event *e = input(i).get();
        assert(e && "No event event when hasOutputOn() == true");
        std::cout << "out " << i << "[" << n[i] << "]: ";

        if (e->kind() == (vms_kind)Kind::A) {
          std::cout << static_cast<Event_A *>(e)->x() << "\n";
        } else if (e->kind() == (vms_kind)Kind::B) {
          std::cout << static_cast<Event_B *>(e)->x() << "\n";
        }
        ++n[i];
        input(i).consume();
        result = StepResult::Progress;
      }
    }
    return result;
  }
};

Trace *createTrace(TracesPipeline &TP) {
  Trace *trace = new LocalTrace<TEvent>(TP);
  for (int x = 1; x < 10; ++x) {
    trace->push(Event_A(x, x));
    trace->push(Event_B(x, x));
  }
  trace->setFinished();

  return trace;
}

int main() {
  TracesPipeline TP;

  Trace *traces[20];
  for (int i = 0; i < 20; ++i)
    traces[i] = createTrace(TP);

  for (int i = 0; i < 20; ++i) {
    for (int j = 0; j < 20; ++j) {
      if (i == j)
        continue;

      auto *T = new TTransformer(TP, {traces[i], traces[j]});
      new OutTransformer(TP, *T);
    }
  }

  TP.run();
}
