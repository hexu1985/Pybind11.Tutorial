import os
import sys
sys.path.append(os.getenv('EXAMPLE_MODULE_PATH'))

import example

v = example.add(1, 2)
assert v == 3
print(v)

v = example.add(i=2, j=3)
assert v == 5
print(v)

