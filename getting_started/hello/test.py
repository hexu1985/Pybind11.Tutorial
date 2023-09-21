#!/usr/bin/env python3

import os
import sys

sys.path.append('./build')

import hello

s = hello.greet()
assert s == "hello, world"
print(s)

