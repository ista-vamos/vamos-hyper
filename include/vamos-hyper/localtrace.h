#ifndef VAMOS_HYPER_LOCAL_TRACE_H
#define VAMOS_HYPER_LOCAL_TRACE_H

#include <vamos-buffers/cpp/ring_buffer.h>
#include <vamos-hyper/trace.h>

#include <cstring>
#include <memory>

namespace vamos {
namespace hyper {

template <typename BufferElemTy, uint64_t BufferCapacity = 1024>
class LocalTrace : public Trace {
    using RingBufTy = RingBuffer<BufferCapacity, sizeof(BufferElemTy)>;
    RingBufTy rb;
    RingBufferReader<RingBufTy> reader;
    RingBufferWriter<RingBufTy> writer;

    std::unique_ptr<BufferElemTy[]> data;

   protected:
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

   public:
    LocalTrace(TracesPipeline &TP)
        : Trace(TP),
          reader(rb),
          writer(rb),
          data(new BufferElemTy[BufferCapacity]) {}

    Event *get_impl(size_t idx = 0) override {
        assert(idx == 0 && "Not implemented");
        size_t n;
        size_t off = reader.read_off(n);
        if (n > idx) {
            return &data[off];
        }
        return nullptr;
    }

    void consume_impl(size_t n) override { reader.consume(n); }

    // const Event *get(size_t idx = 0) const override;
    size_t unreadNum_impl() override {
        return reader.available();
        /*
        size_t n;
        reader.read_off(n);
        return n;
        */
    }
};

}  // namespace hyper
}  // namespace vamos

#endif
