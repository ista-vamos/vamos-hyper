#ifndef VAMOS_HYPER_LOCAL_TRACE_H
#define VAMOS_HYPER_LOCAL_TRACE_H

#include <cstring>
#include <memory>

#include <vamos-buffers/cpp/ring_buffer.h>

namespace vamos {
namespace hyper {

template <typename BufferElemTy, uint64_t BufferCapacity = 1024>
class LocalTrace : public Trace {
  using RingBufTy = RingBuffer<BufferCapacity, sizeof(BufferElemTy)>;
  RingBufTy rb;
  RingBufferReader<RingBufTy> reader;
  RingBufferWriter<RingBufTy> writer;

  std::unique_ptr<BufferElemTy[]> data;

public:
  LocalTrace(TracesPipeline &TP)
      : Trace(TP), reader(rb), writer(rb),
        data(new BufferElemTy[BufferCapacity]) {}

  bool push_impl(const Event &event) override {
    size_t n;
    size_t off = writer.write_off(n);
    if (n > 0) {
      data[off] = static_cast<const BufferElemTy &>(event);
      writer.write_finish(1);
      return true;
    }

    return false;
  }

  Event *get(size_t idx = 0) override {
    assert(idx == 0 && "Not implemented");
    size_t n;
    size_t off = reader.read_off(n);
    if (n > idx) {
      return &data[off];
    }
    return nullptr;
  }

  void consume(size_t n) override { reader.consume(n); }

  // const Event *get(size_t idx = 0) const override;
  bool has(uint64_t num = 1) override {
    size_t n;
    reader.read_off(n);
    return n >= num;
  }
};

} // namespace hyper
} // namespace vamos

#endif
