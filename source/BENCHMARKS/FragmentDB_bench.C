// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: FragmentDB_bench.C,v 1.5 2004/05/10 11:56:29 amoll Exp $

#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(FragmentDB, 1.0, "$Id: FragmentDB_bench.C,v 1.5 2004/05/10 11:56:29 amoll Exp $")


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

START_SECTION(Creation, 0.50)
	FragmentDB* ptr;
	for (Size i = 0; i < 20; i++)
	{
		START_TIMER
			ptr = new FragmentDB("");
		STOP_TIMER
		delete ptr;
	}

END_SECTION

STATUS("Creating fragment DB")
FragmentDB db("");
STATUS("Readig PDB file")
PDBFile f("data/AmberFF_bench.pdb");
System original;
f >> original;
f.close();

START_SECTION(Name normalization, 0.1)
	for (Size i = 0; i < 50; ++i)
	{
		System S(original);
		START_TIMER
			S.apply(db.normalize_names);
		STOP_TIMER
	}
	
END_SECTION

START_SECTION(Bond building, 0.1)
	for (Size i = 0; i < 50; ++i)
	{
		System S(original);
		START_TIMER
			S.apply(db.build_bonds);
		STOP_TIMER
	}
	
END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
