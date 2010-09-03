// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDB_bench.C,v 1.7 2005/02/15 19:18:41 oliver Exp $
//
#include <BALLBenchmarkConfig.h>
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(PDBFile, 1.0, "$Id: PDB_bench.C,v 1.7 2005/02/15 19:18:41 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

START_SECTION(Reading, 0.2)
	for (Position count = 0; count < 500; count++)
	{
		PDBFile infile(BALL_BENCHMARK_DATA_PATH(AmberFF_bench.pdb));
		infile.options.setBool(PDBFile::Option::STORE_SKIPPED_RECORDS, false);
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
