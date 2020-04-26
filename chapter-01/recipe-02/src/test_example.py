import os
import sys
sys.path.append(os.getenv('EXAMPLE_MODULE_PATH'))

import example

v = example.add(1, 2)
assert v == 3
print(v)

