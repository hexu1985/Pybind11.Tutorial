#!/usr/bin/env python3

import sys
sys.path.append('./build')

class A:
    def __int__(self):
        return 123


from example import print

print(A())
