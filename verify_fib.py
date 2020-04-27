#!/usr/bin/env python
import sys

f1 = 0
f2 = 1
i = 2

with open('fib.data') as f:
    for l in f:
        f = f1 + f2
        assert f == int(l.lstrip('0')[:-1]) , "Bad fib({})".format(i)
        f1 = f2
        f2 = f
        i = i + 1

print "Good fib"
