#/usr/bin/env python3
from random import randint, seed
import sys

def _encode_n(name, n):
    bitstr = str(bin(n))[2:][::-1]
    ns = [n for n, b in enumerate(bitstr) if b == "1"]
    return ",".join((f"{name}_{n}" for n in ns))

class IOEvent:
    """
    I/O Event
    """
    def __init__(self, n_in, n_out):
        self.n_in = n_in
        self.n_out = n_out

    def encode(self):
        return f"{_encode_n('in', self.n_in)};{_encode_n('out', self.n_out)}"


p = 0.1
p_err = 0.001

if len(sys.argv) != 4:
    print("Need arguments: <number of traces> <length of traces> <bits (number of atomic props.)>", file=sys.stderr)
    print(
f"""
The script will generate 2*<number of traces> of length <length of traces>
Traces will contain I/O events with probability p={p} and a dummy events
with complementary probability 1-p. The I/O event has atomic propositions
in_0, in_1, ... and out_0, out_1, ... that corresponds to BITS.
They are populated from uniform range `(0, 2**BITS-1)`, i.e., together `in`
and `out` give a random unsigned number on `BITS` bits.

The traces come in pairs and every event in the second trace is with
the probability p_err={p_err} generated a new, which means that it can differ
from a corresponding event on the first trace.

Parameters `p` and `p_err` can be set in the script.
""", file=sys.stderr)
    exit(1)


TRACE_NUM = int(sys.argv[1])
TRACE_LEN = int(sys.argv[2])
BITS = int(sys.argv[3])
maxnum = (2**BITS)

def gen_rand_event():
    """
    Generate I/O event with probability `p` and a dummy event with
    probability `1-p`. The I/O event will have random fields
    populated from uniform range `(0, 2**BITS-1)`, i.e., it is a random
    unsigned number on `BITS` bits.
    The dummy event is just all `in` and `out` bits set to 0.
    """
    p_0 = randint(0, TRACE_LEN)/TRACE_LEN
    if p_0 <= p:
        n_in, n_out = randint(0, maxnum), randint(0, maxnum)
        return IOEvent(n_in, n_out)
    else:
        return IOEvent(0, 0)

seed() # initialize random numbers

RESOLUTION = 1000

differ = 0
for trnum in range(0, TRACE_NUM):
    t1 = open(f"{trnum}-1.tr", "w")
    t2 = open(f"{trnum}-2.tr", "w")

    for n in range(0, TRACE_LEN):
        e1 = gen_rand_event()

        # 1% of cases gen a different output to t2
        # in the last event
        if n == TRACE_LEN - 1 and\
            randint(0, RESOLUTION) < (p_err * RESOLUTION):
            e2 = IOEvent(e1.n_in, e1.n_out ^ 0x1)
            differ += 1
        else:
            e2 = e1

        print(e1.encode(), file=t1)
        print(e2.encode(), file=t2)

    t1.close()
    t2.close()

print(f"Generated {trnum+1} pairs of traces", file=sys.stderr)
print(f"Number of traces with diff. last event: {differ}", file=sys.stderr)

if differ == 0:
    print("Did not generate a single different trace, you might want to re-run me!", file=sys.stderr)
    exit(1)
exit(0)
