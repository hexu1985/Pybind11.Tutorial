#!/usr/bin/env python3

import sys
sys.path.append('./build')

import example
ans = example.the_answer
assert ans == 42
print(ans)

what = example.what
assert what == "World"
print(what)

