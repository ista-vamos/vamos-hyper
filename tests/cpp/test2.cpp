#include <iostream>

#include <vamos-hyper/vamos-hyper.h>
#include <vamos-hyper/localtrace.h>
#include <vamos-buffers/cpp/event.h>

using namespace vamos::hyper;

struct Event_A : public vamos::Event {
    int x;

    Event_A() = default;
    Event_A(int y) : x(y) {}
};

int main() {
    TracesPipeline TP;

    Trace *trace = new LocalTrace<Event_A>(TP);
    for (int x = 1; x < 1000; ++x) {
        trace->push(Event_A(x));
    }

    while (trace->has()) {
        std::cout << static_cast<Event_A*>(trace->get())->x << "\n";
        trace->consume();
    }

    delete trace;
}
