// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDB_bench.C,v 1.3 2002/02/27 12:20:33 sturm Exp $
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(PDBFile, 1.0, "$Id: PDB_bench.C,v 1.3 2002/02/27 12:20:33 sturm Exp $")


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

START_SECTION(Reading, 1.0)

	for (int count = 0; count < 100; count++)
	{
		PDBFile infile("data/AmberFF_bench.pdb");
		System S;
		START_TIMER
		infile >> S;
		STOP_TIMER
		infile.close();
	}
	

END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
