// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Sysinfo_test.C,v 1.5 2005/01/25 16:03:55 amoll Exp $
//

#include <BALL/CONCEPT/classTest.h>
///////////////////////////

#include <BALL/SYSTEM/sysinfo.h>

///////////////////////////

START_TEST(SysInfo, "$Id: Sysinfo_test.C,v 1.5 2005/01/25 16:03:55 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace BALL::SysInfo;

	
CHECK(getFreeMemory())
	TEST_EQUAL(getFreeMemory() > 0.0, true)
// 	Log.error() <<getFre eMemory() << std::endl;
RESULT		

CHECK(getAvailableMemory())
	TEST_EQUAL(getAvailableMemory() > 0.0, true)
//  	Log.error() <<getAvailableMemory() << std::endl;
RESULT		


CHECK(getTotalMemory())
	TEST_EQUAL(getTotalMemory() > 0, true)
	TEST_EQUAL(getTotalMemory(), getTotalMemory())
// 	Log.error() <<getTotalMemory() << std::endl;
RESULT		

CHECK(getBufferdMemory())
	long bufferd = getBufferdMemory();
	TEST_EQUAL(bufferd > 0 || bufferd == -1, true)
// 	Log.error() <<getBufferdMemory() << std::endl;
RESULT		

CHECK(getFreeSwapSpace())
	TEST_EQUAL(getFreeSwapSpace() >= 0, true)
// 	Log.error() <<getFreeSwapSpace() << std::endl;
RESULT		


CHECK(getUptime())
	float uptime = getUptime();
	TEST_EQUAL(uptime > 0 || uptime == -1, true)
// 	Log.error() <<getUptime() << std::endl;
RESULT		

CHECK(getNumberOfProcessors())
	TEST_EQUAL(getNumberOfProcessors() >= 1, true)
// 	Log.error() <<getNumberOfProcessors() << std::endl;
RESULT		

/*
CHECK(Extra1)
	TEST_EQUAL(getFreeMemory() > 0, true)
	Index i1 = getFreeMemory() + getBufferdMemory();
	Log.error() << std::endl;
	Log.error() << i1 << std::endl;
	double* d = new double[100000000];
	sleep(1);
	Index i2 = getFreeMemory() + getBufferdMemory();
	Log.error() <<i2 << std::endl;
	TEST_EQUAL(i1 > i2, true);
	delete[] d;
RESULT		
*/

CHECK(Extra1)
	Index i1 = getAvailableMemory();
	i1 *= 0.9;
	char* c = new char[i1];
	delete[] c;
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
