// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Sysinfo_test.C,v 1.11 2005/01/27 15:29:55 oliver Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/sysinfo.h>
#include <new>
///////////////////////////

START_TEST(SysInfo, "$Id: Sysinfo_test.C,v 1.11 2005/01/27 15:29:55 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace BALL::SysInfo;

	
CHECK(getFreeMemory())
	TEST_EQUAL(getFreeMemory() > 0.0, true)
RESULT		


CHECK(getAvailableMemory())
	TEST_EQUAL(getAvailableMemory() > 0.0, true)
RESULT		


CHECK(getTotalMemory())
	TEST_EQUAL(getTotalMemory() > 0, true)
	STATUS(" total mem: " << (Size)(getTotalMemory() / 1024) << " kiB")
RESULT		


CHECK(getBufferedMemory())
	LongIndex buffered = getBufferedMemory();
	STATUS(" buffered mem: " << (Size)(getBufferedMemory() / 1024) << " kiB")
RESULT		


CHECK(getFreeSwapSpace())
	TEST_EQUAL(getFreeSwapSpace() >= 0, true)
	STATUS(" swap space: " << (Size)(getFreeSwapSpace() / 1024) << " kiB")
RESULT		


CHECK(getUptime())
	LongIndex uptime = getUptime();
	TEST_EQUAL(uptime > 0 || uptime == -1, true)
	STATUS(" uptime: " << getUptime())
RESULT		


CHECK(getNumberOfProcessors())
	TEST_EQUAL(getNumberOfProcessors() >= 1, true)
	STATUS(" # of processors: " << getNumberOfProcessors())
RESULT		

CHECK([Extra checking for bad_alloc being thrown])
	LongIndex i1 = getAvailableMemory();
 	i1 = i1 * 5000;
	char* c = 0;
	TEST_EXCEPTION(std::bad_alloc, c = new char[i1]);
	if (c != 0)
	{	
		delete [] c;
	}
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
