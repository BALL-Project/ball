// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDB_bench.C,v 1.2.2.3 2003/02/05 15:45:30 anker Exp $

#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(PDBFile, 1.0, "$Id: PDB_bench.C,v 1.2.2.3 2003/02/05 15:45:30 anker Exp $")

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
