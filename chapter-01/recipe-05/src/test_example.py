import os
import sys
sys.path.append(os.getenv('EXAMPLE_MODULE_PATH'))

import example

ans = example.the_answer
assert ans == 42
print(ans)

what = example.what
assert what == "World"
print(what)

