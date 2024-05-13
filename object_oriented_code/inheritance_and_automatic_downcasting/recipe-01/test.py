#!/usr/bin/env python3

import sys
sys.path.append('./build')

import example
p = example.Dog("Molly")
print(p.name)
print(p.bark())
