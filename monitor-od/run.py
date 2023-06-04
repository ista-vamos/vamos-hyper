#!/usr/bin/env python3

from subprocess import Popen, PIPE
from os.path import dirname, realpath
from sys import argv

bindir = dirname(realpath(__file__))

NUM_STREAMS = range(500, 2001, 500)
STREAM_LEN = range(500, 10001, 500)

def run(binary):
    for n in NUM_STREAMS:
        for l in STREAM_LEN:
            cmd = ["/bin/time", f"{bindir}/{binary}-{n}-{l}"]
            p = Popen(cmd, stderr=PIPE, stdout=PIPE)
            out, err = p.communicate()
            assert p.returncode == 0, p
            assert err is not None, cmd
            assert out is not None, cmd

            # Max workbag size: 7391
            #Traces #: 500
            #1.73user 0.03system 0:01.76elapsed 99%CPU (0avgtext+0avgdata 137604maxresident)k
            #0inputs+0outputs (0major+34642minor)pagefaults 0swaps
            wbg_size=None
            cpu_time=None
            wall_time=None
            mem=None
            for line in out.splitlines():
                line = line.strip()
                if line.startswith(b"Max workbag"):
                    wbg_size = int(line.split()[3])

            for line in err.splitlines():
                if b"elapsed" in line:
                    parts = line.split()
                    assert b"user" in parts[0]
                    assert b"elapsed" in parts[2]
                    assert b"maxresident" in parts[5]
                    cpu_time = float(parts[0][:-4])
                    wall_time = float(parts[2][2:-7])
                    mem = int(parts[5][:-13])/1024

            print(binary, n, l, wbg_size, cpu_time, wall_time, mem)

run(argv[1])
