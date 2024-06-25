#!/usr/bin/env python3

import sys
sys.path.append('./build')

import example

count = 1

def square(i):
    return i * i

print(example.func_arg(square))

square_plus_1 = example.func_ret(square)
print(square_plus_1(4))

plus_1 = example.func_cpp()
print(plus_1(number=43))
