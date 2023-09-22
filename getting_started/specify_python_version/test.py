#!/usr/bin/env python3.8

import sys
sys.path.append('./build')

import example
s = example.greet()
assert s == "hello, world!"
print(s)

