// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: FragmentDB_bench.C,v 1.2.2.1 2003/01/07 13:20:24 anker Exp $

#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/STRUCTURE/fragmentDB.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(FragmentDB, 10.0, "$Id: FragmentDB_bench.C,v 1.2.2.1 2003/01/07 13:20:24 anker Exp $")


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

START_SECTION(Creation, 1.0)

	FragmentDB* ptr;
	for (Size i = 0; i < 5; i++)
	{
		START_TIMER
			ptr = new FragmentDB;
		STOP_TIMER
		delete ptr;
	}

END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
