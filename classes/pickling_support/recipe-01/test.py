#!/usr/bin/env python3

import sys
import pickle
sys.path.append('./build')

from example import *

p = Pickleable("test_value")
print("p: {}".format(p))
p.setExtra(15)
data = pickle.dumps(p, 2)
print("data: {}".format(data))

p2 = pickle.loads(data)
print("p2: {}".format(p2))
