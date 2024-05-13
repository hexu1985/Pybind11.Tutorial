#!/usr/bin/env python3

import sys
sys.path.append('./build')

from example import Pet
p = Pet("Lucy", Pet.Cat)
print(p.type)
print(int(p.type))
print(Pet.Kind.__members__)

