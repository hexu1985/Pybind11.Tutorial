#!/usr/bin/env python3

import sys
sys.path.append('./build')

import cats, dogs, frogs  # No error because of the added py::module_local()

mycat, mydog = cats.Cat("Fluffy"), dogs.Dog("Rover")
print((cats.pet_name(mycat), dogs.pet_name(mydog)))
print((cats.pet_name(mydog), dogs.pet_name(mycat), frogs.pet_name(mycat)))


