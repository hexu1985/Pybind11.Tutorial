#!/usr/bin/env python3

import sys
sys.path.append('./build')

import example
v = example.add(i=1, j=2)
assert v == 3
print(v)

v = example.add(i=4)
assert v == 6
print(v)

v = example.add(j=4)
assert v == 5
print(v)
