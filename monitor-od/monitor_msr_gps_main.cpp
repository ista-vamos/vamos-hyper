#include <cassert>
#include <iostream>

#include "src-gps/monitor.h"

int monitor(Inputs &inputs);

int main(int /*argc*/, char *argv[]) {
  Inputs inputs(argv + 1);
  return monitor(inputs);
}
