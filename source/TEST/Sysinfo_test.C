// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Sysinfo_test.C,v 1.10 2005/01/25 17:26:48 amoll Exp $
//

#include <BALL/CONCEPT/classTest.h>
///////////////////////////
#include <BALL/SYSTEM/sysinfo.h>
///////////////////////////

START_TEST(SysInfo, "$Id: Sysinfo_test.C,v 1.10 2005/01/25 17:26:48 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace BALL::SysInfo;

	
CHECK(getFreeMemory())
	TEST_EQUAL(getFreeMemory() > 0.0, true)
// Log.error() <<getFreeMemory() << std::endl;
RESULT		


CHECK(getAvailableMemory())
	TEST_EQUAL(getAvailableMemory() > 0.0, true)
// Log.error() <<getAvailableMemory() << std::endl;
RESULT		


CHECK(getTotalMemory())
	TEST_EQUAL(getTotalMemory() > 0, true)
	TEST_REAL_EQUAL(getTotalMemory(), getTotalMemory())
//    	Log.error() <<getTotalMemory() << std::endl;
RESULT		


CHECK(getBufferdMemory())
	float bufferd = getBufferdMemory();
	TEST_EQUAL(bufferd > 0 || bufferd == -1, true)
//    	Log.error() <<getBufferdMemory() << std::endl;
RESULT		


CHECK(getFreeSwapSpace())
	TEST_EQUAL(getFreeSwapSpace() >= 0, true)
//    	Log.error() <<getFreeSwapSpace() << std::endl;
RESULT		


CHECK(getUptime())
	float uptime = getUptime();
	TEST_EQUAL(uptime > 0 || uptime == -1, true)
//    	Log.error() <<getUptime() << std::endl;
RESULT		


CHECK(getNumberOfProcessors())
	TEST_EQUAL(getNumberOfProcessors() >= 1, true)
//    	Log.error() <<getNumberOfProcessors() << std::endl;
RESULT		

 // doesnt work under Linux, no idea why: ????????? AM
CHECK(Extra1)
	float i1 = getAvailableMemory();
	char* d = new char[(long)(i1 * 0.9)];
	float i2 = getAvailableMemory();
	TEST_EQUAL(i1 > i2, true);
	delete[] d;
RESULT		


CHECK(Extra2)
	float i1 = getAvailableMemory();
 	i1 *= 0.9;
	char* c = new char[(long)i1];
	delete[] c;
RESULT


CHECK(Extra3)
	float i1 = getAvailableMemory();
 	i1 *= 1.1;
	char* c = 0;
	TEST_EXCEPTION(Exception::OutOfMemory,c = new char[(long)i1]);
	if (c != 0) delete[] c;
RESULT


CHECK(Extra4)
	float i1 = getAvailableMemory() * 0.90;
	char* c = new char[(long)i1];
	i1 = getAvailableMemory() * 0.20;
	char* d = new char[(long)i1];
	delete[] c;
	delete[] d;
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
