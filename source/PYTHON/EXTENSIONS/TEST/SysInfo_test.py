#! python
from ClassTest import *

START_TEST('SysInfo', '$Id: SysInfo_test.py,v 1.2 2005/12/23 17:03:02 amoll Exp $')

CHECK('SysInfo.getFreeMemory()')
try:
	TEST_EQUAL((SysInfo.getFreeMemory() > 0.0), true)
	#STATUS(" free mem: " + str(SysInfo.getFreeMemory() / 1024) + " kiB")
except:
	traceback.print_exc()
	FAIL()
RESULT()

CHECK('SysInfo.getAvailabelMemory()')
#
#
RESULT()

END_TEST()
