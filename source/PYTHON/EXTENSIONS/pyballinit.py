false=0
true=1

import sys
import traceback
import os
if os.environ.has_key("BALL_PY_LIBPATH"):
	sys.path.append(os.environ["BALL_PY_LIBPATH"])
if os.environ.has_key("LD_LIBRARY_PATH"):
	sys.path.append(os.environ["LD_LIBRARY_PATH"])

from BALL import *
print "BALL", VersionInfo.getVersion()
