#!/usr/bin/env python3

import sys
sys.path.append('./build')

from example1 import *

d = Dog()
call_go(d)     # u'woof! woof! woof! '

class Cat(Animal):
    def go(self, n_times):
        return "meow! " * n_times

c = Cat()
call_go(c)   # u'meow! meow! meow! '


