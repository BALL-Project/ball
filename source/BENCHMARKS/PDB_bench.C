// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDB_bench.C,v 1.5 2005/02/08 17:32:37 oliver Exp $

#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(PDBFile, 1.0, "$Id: PDB_bench.C,v 1.5 2005/02/08 17:32:37 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

START_SECTION(Reading, 1.0)
	for (Position count = 0; count < 100; count++)
	{
		PDBFile infile("data/AmberFF_bench.pdb");
		infile.options.setBool(GenericPDBFile::Option::STORE_SKIPPED_RECORDS, false);
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
