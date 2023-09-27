#!/usr/bin/env python3

import sys
sys.path.append('./build')

import example
p = example.pet_store()
print(type(p))
print(p.bark())
