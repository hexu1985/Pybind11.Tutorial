#!/usr/bin/env python3

import sys
sys.path.append('./build')

import example

print(isinstance(example.asymmetry(b"have some bytes"), str))

print(example.asymmetry(b"\xba\xd0\xba\xd0"))  # invalid utf-8 as bytes
