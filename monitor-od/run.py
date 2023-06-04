#!/usr/bin/env python3

from subprocess import Popen, PIPE
from os.path import dirname, realpath
from os import listdir
from sys import argv
from multiprocessing import Pool

bindir = dirname(realpath(__file__))

def run_one(arg):
    binary, n, l = arg
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
    #return (n, l, wbg_size, cpu_time, wall_time, mem)

def get_params(binary):
    binary += "-"
    l = len(binary)
    for fl in listdir(bindir):
        if fl.startswith(binary):
            parts = fl[l:].split("-")
            yield (int(parts[0]), int(parts[1]))

binary = argv[1]
args=((binary, n, l) for (n, l) in get_params(binary))
proc_num=None
if len(argv) == 3:
    proc_num=int(argv[2])

with Pool(processes=proc_num) as pool:
    result = pool.map(run_one, args)

