#ifndef VAMOS_HYPER_DEBUG_H
#define VAMOS_HYPER_DEBUG_H

#include <iostream>

namespace vamos {
namespace hyper {

template <typename MsgT> void dbg(const MsgT &msg) {
  std::cerr << "[vamos-hyper] ";
  std::cerr << msg;
  std::cerr << "\n";
}

} // namespace hyper
} // namespace vamos

#endif
