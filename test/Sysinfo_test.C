// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/sysinfo.h>
#include <new>
#include <utility>
///////////////////////////

START_TEST(SysInfo)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace BALL::SysInfo;

	
CHECK(getFreeMemory())
	TEST_EQUAL(getFreeMemory() > 0.0, true)
	STATUS(" free mem: " << getFreeMemory() / 1024 << " kiB")
RESULT		


CHECK(getAvailableMemory())
	TEST_EQUAL(getAvailableMemory() > 0.0, true)
	STATUS(" av. mem: " << getFreeMemory() / 1024 << " kiB")
RESULT		


CHECK(getTotalMemory())
	TEST_EQUAL(getTotalMemory() > 0, true)
	STATUS(" total mem: " << (Size)(getTotalMemory() / 1024) << " kiB")
RESULT		


CHECK(getBufferedMemory())
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

// NOTE: I've commented out the following two tests! I don't think that we should
//       something as extreme during tests! That's terrible!
/*
CHECK([Extra] Allocs reduce av. mem)
	LongIndex i1 = getFreeMemory() + getFreeSwapSpace();
	STATUS(" mem before: " << i1 / 1024 / 1024 << " MiB")	
	char* ptr = 0;
	try
	{
		ptr = new char[i1 / 4];
		for (LongIndex idx = 0; idx < (i1 / 4); ptr[idx] =  'A', idx += 512);
		STATUS(" Allocated " << (i1 / 4) / 1024 / 1024 << " MiB")
	}
	catch (std::bad_alloc& e)
	{
		STATUS("Threw bad_alloc!");
	}
	LongIndex i2 = getFreeMemory() + getFreeSwapSpace();
	if (ptr != 0)
	{	
		delete [] ptr;
		ptr = 0;
	}
	STATUS(" mem after: " << i2 / 1024 / 1024<< " MiB")
	STATUS(" mem diff: " << (i1 - i2) / 1024 / 1024 << " MiB")
	TEST_EQUAL((i1 - i2) > 0, true)
RESULT

CHECK([Extra] new throws std::bad_alloc if out of memory)
	LongIndex i1 = getTotalMemory() + getFreeSwapSpace();
	STATUS(" mem free: " << i1 / 1024 / 1024 << " MiB")	
	// Try to allocate more memory than the available amount,
  // but restrict yourself to an amount slightly below 2GiB.
	// Most new [] implementations won't handle more than that...
 	i1 = std::min((LongIndex)2000 * 1024 * 1024, i1 * 4);
	STATUS(" trying to allocate: " << i1 / 1024 / 1024 << " MiB")	
	char* c = 0;
	try
	{
		c = new char[i1];
	}
	catch (std::bad_alloc& e)
	{
		STATUS("Caught exception: " << e.what())
	}
	STATUS(" ptr = " << (void*)c)
	if (c != 0)
	{	
		delete [] c;
	}
RESULT
*/

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
