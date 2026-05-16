"""End-to-end exercise of the autowrap-generated SimpleVec wrapper."""
import sys
from simple_vec_autowrap import SimpleVec

print("PASS  imported SimpleVec")

# Constructors
v = SimpleVec()
print(f"PASS  default ctor: {v.repr()}")

v = SimpleVec(1.0, 2.0, 3.0)
print(f"PASS  3-arg ctor : {v.repr()}")

v2 = SimpleVec(v)
print(f"PASS  copy ctor  : {v2.repr()}")

# Getters / setters
assert v.getX() == 1.0
assert v.getY() == 2.0
assert v.getZ() == 3.0
v.setX(10.0)
assert v.getX() == 10.0
print("PASS  getters/setters")

# Methods
v = SimpleVec(3.0, 4.0, 0.0)
assert abs(v.length() - 5.0) < 1e-9
print(f"PASS  length: {v.length()}")

n = v.normalize()
assert abs(n.length() - 1.0) < 1e-9
print(f"PASS  normalize: {n.repr()}")

# Operators
a = SimpleVec(1.0, 2.0, 3.0)
b = SimpleVec(10.0, 20.0, 30.0)

c = a + b
print(f"PASS  a + b = {c.repr()}")
assert c.getX() == 11.0

c = b - a
print(f"PASS  b - a = {c.repr()}")

assert a == SimpleVec(1.0, 2.0, 3.0)
assert a != b
print("PASS  == / !=")

# Exception propagation
zero = SimpleVec(0.0, 0.0, 0.0)
try:
    zero.normalize()
    print("FAIL  normalize on zero should raise")
    sys.exit(1)
except (RuntimeError, ValueError) as e:
    print(f"PASS  exception propagation (normalize zero): {e!r}")

try:
    # PITFALL #6: autowrap doesn't auto-coerce int→double for ctor overload
    # resolution. Must pass floats explicitly when the C++ signature takes double.
    SimpleVec(1.0, 2.0, 3.0).divideBy(0.0)
    print("FAIL  divideBy(0) should raise")
    sys.exit(1)
except (RuntimeError, ValueError) as e:
    print(f"PASS  exception propagation (divideBy 0): {e!r}")

print()
print("=" * 60)
print("ALL POC1 TESTS PASSED")
print("=" * 60)
