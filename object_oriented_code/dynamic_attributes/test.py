#!/usr/bin/env python3

import sys
sys.path.append('./build')

import example
p = example.Pet()
p.name = "Charly"
print(p.name)
p.age = 2
print(p.age)
