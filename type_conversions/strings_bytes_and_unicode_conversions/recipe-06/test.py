#!/usr/bin/env python3

import sys
sys.path.append('./build')

import example

print(example.pass_char("A"))

try:
    print(example.pass_char(0x65))
except Exception as e:
    print("error: ", e)

print(example.pass_char(chr(0x65)))
