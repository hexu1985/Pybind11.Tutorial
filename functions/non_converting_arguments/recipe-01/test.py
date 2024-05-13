#!/usr/bin/env python3

import sys
sys.path.append('./build')

from example import *
v = floats_preferred(4)
print(v)
v = floats_only(4.0)
print(v)
v = floats_only(4)
