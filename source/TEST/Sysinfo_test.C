// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Sysinfo_test.C,v 1.1 2005/01/25 11:26:30 amoll Exp $
//

#include <BALL/CONCEPT/classTest.h>
///////////////////////////

#include <BALL/SYSTEM/sysinfo.h>

///////////////////////////

START_TEST(SysInfo, "$Id: Sysinfo_test.C,v 1.1 2005/01/25 11:26:30 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace BALL::SysInfo;

///  insert tests for each member function here         
///
	
CHECK(getFreeMemory())
TEST_NOT_EQUAL(getFreeMemory(), 0)
	Log.error() <<getFreeMemory() << std::endl;
RESULT		

CHECK(getTotalMemory())
TEST_NOT_EQUAL(getTotalMemory(), 0)
	Log.error() <<getTotalMemory() << std::endl;
RESULT		

CHECK(getBufferdMemory())
TEST_NOT_EQUAL(getBufferdMemory(), 0)
	Log.error() <<getBufferdMemory() << std::endl;
RESULT		

CHECK(getUptime())
TEST_NOT_EQUAL(getUptime(), 0)
	Log.error() <<getUptime() << std::endl;
RESULT		


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
