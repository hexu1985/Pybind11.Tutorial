#!/usr/bin/env python3

import sys
sys.path.append('./build')

import example
v = example.add(i=1, j=2)
assert v == 3
print(v)

