#!/usr/bin/env python3

import sys
sys.path.append('./build')

import example
p = example.pet_store2()
print(type(p))
print(p.bark())
