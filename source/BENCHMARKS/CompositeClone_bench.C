// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: CompositeClone_bench.C,v 1.4 2002/12/18 16:00:32 sturm Exp $
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/CONCEPT/composite.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(CompositeClone, 1.0, "$Id: CompositeClone_bench.C,v 1.4 2002/12/18 16:00:32 sturm Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

Composite S;
Composite* M = new Composite;
S.appendChild(*M);
for (Position i = 0; i < 200; i++)
{
	M->appendChild(*new Composite);
}



START_SECTION(Cloning, 0.5)

	for (int count = 0; count < 500; count++)
	{
		Composite S2;
		START_TIMER
			S2 = *static_cast<Composite*>(S.create(true));
		STOP_TIMER
		S2.clear();
	}

END_SECTION

START_SECTION(Creation, 0.5)

	for (int count = 0; count < 500; count++)
	{
		for (int i = 0; i < 2000; i++)
		{
			START_TIMER
			Composite* a = new Composite;
			STOP_TIMER
			delete a;
		}
	}
END_SECTION

START_SECTION(Assignment, 0.5)

	for (int count = 0; count < 500; count++)
	{
		Composite atom;
		for (int i = 0; i < 2000; i++)
		{
			Composite* a = new Composite;
			START_TIMER
				*a = atom;
			STOP_TIMER
			delete a;
		}
	}
END_SECTION

START_SECTION(Cloning (create), 0.5)

	for (int count = 0; count < 500; count++)
	{
		Composite atom;
		for (int i = 0; i < 2000; i++)
		{
			Composite* a;
			START_TIMER
				a = (Composite*)atom.create();
			STOP_TIMER
			delete a;
		}
	}
END_SECTION

START_SECTION(Insertion, 0.5)

	for (int count = 0; count < 500; count++)
	{
		for (int i = 0; i < 2000; i++)
		{
			Composite M;
			Composite* a = new Composite;
			START_TIMER
				M.appendChild(*a);
			STOP_TIMER
		}
	}

END_SECTION

START_SECTION(Deleting, 0.5)

	for (int count = 0; count < 500; count++)
	{
		Composite S2;
		S2 = *static_cast<Composite*>(S.create(true));
		START_TIMER
			S2.destroy();
		STOP_TIMER
	}

END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
