// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: CompositeCreation_bench.C,v 1.3 2002/12/18 16:00:33 sturm Exp $
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/CONCEPT/composite.h>
#include <vector>

///////////////////////////

using namespace BALL;

START_BENCHMARK(CompositeCreation, 1.0, "$Id: CompositeCreation_bench.C,v 1.3 2002/12/18 16:00:33 sturm Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

Size N = 100000;

std::vector<Composite*> composites;
composites.reserve(100000);

START_SECTION(Creation, 0.5)

	Composite* composite;
	for (Size i = 0; i < N; i++)
	{
		START_TIMER
			composite = new Composite;
		STOP_TIMER
		composites.push_back(composite);
	}

END_SECTION

START_SECTION(Destruction, 0.5)

	for (Size i = 0; i < N; i++)
	{
		composite = composites[i];
		START_TIMER
			delete composite;
		STOP_TIMER
	}
END_SECTION


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
