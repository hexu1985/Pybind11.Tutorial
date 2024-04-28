#!/usr/bin/env python3

import sys
sys.path.append('./build')

from example2 import *

d = Dog()
print(call_go(d))     # u'woof! woof! woof! '

class Cat(Animal):
    def go(self, n_times):
        return "meow! " * n_times

c = Cat()
print(call_go(c))   # u'meow! meow! meow! '

class Dachshund(Dog):
    def __init__(self, name):
        Dog.__init__(self)  # Without this, a TypeError is raised.
        self.name = name

    def bark(self):
        return "yap!"

