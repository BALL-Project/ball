"""
	Wrapper script to import both BALLCore and VIEW modules
"""

from BALLCore import *

try:
	from VIEW import *
except Exception as err:
	print("WARNING: Could not import VIEW module:\n" + err.args[0])
