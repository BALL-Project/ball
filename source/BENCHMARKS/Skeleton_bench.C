// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Skeleton_bench.C,v 1.2.2.2 2003/02/05 15:32:28 anker Exp $
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

// insert includes here

///////////////////////////

using namespace BALL;

// args: benchmark name (without '"'), reference time in seconds, CVS ID string
<<<<<<< Skeleton_bench.C
START_BENCHMARK(BenchName, 10.0, "$Id: Skeleton_bench.C,v 1.2.2.2 2003/02/05 15:32:28 anker Exp $")
=======
START_BENCHMARK(BenchName, 10.0, "$Id: Skeleton_bench.C,v 1.2.2.2 2003/02/05 15:32:28 anker Exp $")
>>>>>>> 1.3


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
