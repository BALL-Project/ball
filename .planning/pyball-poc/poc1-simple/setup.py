"""
POC1 build: run autowrap to generate the .pyx, then compile via Cython.
Invoke:  python setup.py build_ext --inplace
"""
from pathlib import Path
import sys
from setuptools import setup, Extension
from Cython.Build import cythonize
import autowrap
import autowrap.Main

HERE = Path(__file__).parent
PYX = HERE / "simple_vec_autowrap.pyx"

# PITFALL #5: autowrap-generated .pyx includes "autowrap_tools.hpp" which
# lives in the autowrap package data dir. Must add it to include_dirs.
AUTOWRAP_DATA = Path(autowrap.__file__).parent / "data_files" / "autowrap"

# Step 1: run autowrap to generate the .pyx
print("== autowrap.Main.run ==")
autowrap.Main.run(
    pxds=[str(HERE / "SimpleVec.pxd")],
    addons=[],
    converters=[],
    out=str(PYX),
)
print(f"  generated {PYX}  (size: {PYX.stat().st_size} bytes)")

# Step 2: cythonize + compile
ext = Extension(
    name="simple_vec_autowrap",
    sources=[str(PYX), str(HERE / "simple_vec.cpp")],
    language="c++",
    extra_compile_args=["-std=c++17"],
    include_dirs=[str(HERE), str(AUTOWRAP_DATA)],
)

setup(
    name="poc1_simple",
    ext_modules=cythonize([ext], language_level="3"),
)
