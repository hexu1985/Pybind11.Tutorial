#!/usr/bin/env python3

import sys
sys.path.append('./build')

import example
v = example.add(1, 2)
assert v == 3
print(v)

