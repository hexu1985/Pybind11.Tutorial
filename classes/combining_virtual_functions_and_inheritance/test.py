#!/usr/bin/env python3

import sys
sys.path.append('./build')

from example import *

d = Dog()
print(d.go(3))

class ShihTzu(Dog):
    def bark(self):
        return "yip!"

s = ShihTzu()
print(s.go(4))
