// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Skeleton_bench.C,v 1.2.2.1 2003/01/07 13:20:27 anker Exp $
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

// insert includes here

///////////////////////////

using namespace BALL;

// args: benchmark name (without '"'), reference time in seconds, CVS ID string
START_BENCHMARK(BenchName, 10.0, "$Id: Skeleton_bench.C,v 1.2.2.1 2003/01/07 13:20:27 anker Exp $")


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

// args: section name, scaling factor for section timing
START_SECTION(SectionName, 1.0)

	START_TIMER
		....
	STOP_TIMER

END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
