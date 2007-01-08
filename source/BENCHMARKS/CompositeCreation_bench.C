// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: CompositeCreation_bench.C,v 1.6 2004/02/11 18:52:28 oliver Exp $
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/CONCEPT/composite.h>
#include <vector>

///////////////////////////

using namespace BALL;

START_BENCHMARK(CompositeCreation, 1.0, "$Id: CompositeCreation_bench.C,v 1.6 2004/02/11 18:52:28 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

Size N = 2000000;

std::vector<Composite*> composites(N);

START_SECTION(Creation, 1.0)

	Composite* composite;
	for (Size i = 0; i < N; i++)
	{
		START_TIMER
			composite = new Composite;
		STOP_TIMER
		composites[i] = composite;
	}

END_SECTION

START_SECTION(Destruction, 1.0)

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
