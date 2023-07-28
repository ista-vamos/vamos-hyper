#!/usr/bin/env python3

from tempfile import mkdtemp
from subprocess import Popen, PIPE, DEVNULL, run as runcmd
from os.path import dirname, realpath, basename, join
from os import listdir, access, X_OK
from sys import argv
from multiprocessing import Pool

bindir = f"{dirname(realpath(__file__))}/"
binary = join(bindir, "monitor-rvhyper")

def run_one(arg):
    traces_dir, traces_num, trace_len, bits = arg
    cmd = ["/bin/time", binary]

    files = []
    for fl in listdir(traces_dir):
        if fl.endswith(".tr"):
            files.append(fl)
            if len(files) == traces_num:
                break

    cmd += files
    #print(cmd)
    p = Popen(cmd, stderr=PIPE, stdout=PIPE, cwd=traces_dir)
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

    print("monitor-rvhyper", traces_num, trace_len, bits, wbg_size, cpu_time, wall_time, mem)
    #return (n, l, wbg_size, cpu_time, wall_time, mem)

traces_num = [500, 1000, 1500, 2000, 2500, 3000]
traces_num = [5, 10, 15]

def get_params(traces_dir, trace_len, bits):
    for N in traces_num:
        yield traces_dir, N, trace_len, bits

def run(trace_len, bits):
    proc_num=None
    print(f"\033[1;34mRunning trace_len={trace_len}, bits={bits}\033[0m")
    if len(argv) == 3:
        proc_num=int(argv[2])

    # generate all traces and then always take just some of them
    traces_dir = mkdtemp(prefix="/tmp/")
    runcmd(["python", f"{bindir}/gen-traces.py",
            str(traces_num[-1]), str(trace_len), str(bits), f"force-od,outdir={traces_dir}"],
           stderr=DEVNULL, stdout=DEVNULL, check=True)

    with Pool(processes=proc_num) as pool:
        result = pool.map(run_one, get_params(traces_dir, trace_len, bits))

for trace_len in (100, 300, 500, 700, 1000):
    for bits in (2,4,8,16,32,64):
        run(trace_len, bits)
