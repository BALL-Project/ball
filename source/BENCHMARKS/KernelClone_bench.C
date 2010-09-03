// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: KernelClone_bench.C,v 1.4 2002/12/21 16:46:18 oliver Exp $
#include <BALLBenchmarkConfig.h>
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(KernelIteration, 1.0, "$Id: KernelClone_bench.C,v 1.4 2002/12/21 16:46:18 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

PDBFile infile(BALL_BENCHMARK_DATA_PATH(AmberFF_bench.pdb));
System S1;
infile >> S1;
infile.close();

System S;
Molecule* M = new Molecule;
S.insert(*M);
for (Position i = 0; i < 700; i++)
{
	M->insert(*new Atom);
}



START_SECTION(Cloning w/o bonds, 1.0)

	for (int count = 0; count < 200; count++)
	{
		System S2;
		START_TIMER
			S2 = *static_cast<System*>(S.create(true));
		STOP_TIMER
		S2.clear();
	}

END_SECTION


AtomIterator it(M->beginAtom());
Atom* last = &*it;
for (++it; +it; ++it)
{
	it->createBond(*last);
	last = &*it;
}

START_SECTION(Cloning, 1.0)

	for (int count = 0; count < 200; count++)
	{
		System S2;
		START_TIMER
			S2 = *static_cast<System*>(S.create(true));
		STOP_TIMER
		S2.clear();
	}

END_SECTION

START_SECTION(Creation, 1.0)

	for (int count = 0; count < 300; count++)
	{
		for (int i = 0; i < 700; i++)
		{
			START_TIMER
			Atom* a = new Atom;
			STOP_TIMER
			delete a;
		}
	}
END_SECTION

START_SECTION(Assignment, 1.0)

	for (int count = 0; count < 200; count++)
	{
		Atom atom;
		for (int i = 0; i < 700; i++)
		{
			Atom* a = new Atom;
			START_TIMER
				*a = atom;
			STOP_TIMER
			delete a;
		}
	}
END_SECTION

START_SECTION(Cloning (create), 1.0)

	for (int count = 0; count < 300; count++)
	{
		Atom atom;
		for (int i = 0; i < 700; i++)
		{
			Atom* a;
			START_TIMER
				a = (Atom*)atom.create();
			STOP_TIMER
			delete a;
		}
	}
END_SECTION

START_SECTION(Insertion, 1.0)

	for (int count = 0; count < 300; count++)
	{
		for (int i = 0; i < 700; i++)
		{
			Molecule M;
			Atom* a = new Atom;
			START_TIMER
				M.insert(*a);
			STOP_TIMER
		}
	}

END_SECTION

AtomIterator ai(S.beginAtom());
for (; +ai; ++ai)
{
	ai->destroyBonds();
}

START_SECTION(Deleting, 1.0)

	for (int count = 0; count < 200; count++)
	{
		System S2;
		S2 = *static_cast<System*>(S.create(true));
		START_TIMER
			S2.destroy();
		STOP_TIMER
	}

END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
