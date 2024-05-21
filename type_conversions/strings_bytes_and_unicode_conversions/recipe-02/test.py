#!/usr/bin/env python3

import sys
sys.path.append('./build')

import example

print(isinstance(example.std_string_return(), str))
