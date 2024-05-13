#!/usr/bin/env python3

import sys
sys.path.append('./build')

import example
p = example.Pet("Molly")
print(p.name)
p.name = "Charly"
print(p.name)
