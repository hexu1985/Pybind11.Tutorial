#!/usr/bin/env python3

import sys
sys.path.append('./build')

from module1 import Pet
from module2 import create_pet

pet1 = Pet("Kitty")
print(pet1.name)
pet2 = create_pet("Doggy")
print(pet2.name)
