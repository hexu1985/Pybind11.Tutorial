#!/usr/bin/env python3

import sys
sys.path.append('./build')

import example
v = example.add(1, 2)
assert v == 3
print(v)

v = example.add(1.5, 2.4)
assert abs(v - 3.9) < 0.001
print(v)

v = example.add(1, 2.4)
assert abs(v - 3.4) < 0.001
print(v)

