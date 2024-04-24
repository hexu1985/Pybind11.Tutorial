#!/usr/bin/env python3

import sys
sys.path.append('./build')

from animals import Dog, Cat, bark, meow
print(bark(Dog()))
print(meow(Cat()))
print(bark(None))
print(meow(None))

