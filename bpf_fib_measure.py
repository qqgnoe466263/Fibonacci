#!/usr/bin/env python
from bcc import BPF

prog = """
#include <uapi/linux/ptrace.h>
#include <linux/fs.h>

BPF_HASH(start, u64, u64);

int probe_handler(struct pt_regs *ctx, long long k)
{
    u64 ts = bpf_ktime_get_ns();
    u64 pid = bpf_get_current_pid_tgid();

    start.update(&pid, &ts);

    return 0;
}

int ret_handler(struct pt_regs *ctx) 
{
    u64 ts = bpf_ktime_get_ns();
    u64 pid = bpf_get_current_pid_tgid();
    u64 *tsp = start.lookup(&pid);

    if (tsp != 0) {
        bpf_trace_printk(", %llu \\n", ts - *tsp);
        start.delete(&pid);
    }
    return 0;
}

"""

b = BPF(text=prog)
b.attach_uprobe(name="./fib", sym="fib", fn_name="probe_handler")
b.attach_uretprobe(name="./fib", sym="fib", fn_name="ret_handler")

i = 2
f = open('fib.time', 'w')

while 1:
    try:
        res = b.trace_fields()
    except ValueError:
        continue

    with open('fib.time', 'a') as fp:
        fp.write('{}{}\n'.format(i, res[5].decode("UTF-8")))

    fp.close()
    print '{}{}'.format(i, res[5].decode("UTF-8"))
    i = i + 1





