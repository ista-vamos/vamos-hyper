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


TRACE_NUM = int(sys.argv[1])
TRACE_LEN = int(sys.argv[2])
BITS = int(sys.argv[3])

maxnum = (2**BITS)

dummy = IOEvent(0, 0)

def gen_rand_event():
    if randint(0, TRACE_LEN) <= (TRACE_LEN / 10):
        n_in, n_out = randint(0, maxnum), randint(0, maxnum)
        return IOEvent(n_in, n_out)
    else:
        return dummy

seed() # initialize random numbers

for trnum in range(0, TRACE_NUM):
    t1 = open(f"{trnum}-1.tr", "w")
    t2 = open(f"{trnum}-2.tr", "w")

    for n in range(0, TRACE_LEN):
        e1 = gen_rand_event()

        if randint(0, 100) <= 0: # 1% of cases gen a different output to t2
            e2 = gen_rand_event()
            e2.n_in = e1.n_in
        else:
            e2 = e1

        print(e1.encode(), file=t1)
        print(e2.encode(), file=t2)

    t1.close()
    t2.close()
