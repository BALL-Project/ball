// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: KernelCreation_bench.C,v 1.3 2002/12/21 16:46:18 oliver Exp $
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(KernelIteration, 1.0, "$Id: KernelCreation_bench.C,v 1.3 2002/12/21 16:46:18 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


Size N = 40000;

vector<Atom*> atoms;
atoms.reserve(N);
START_SECTION(Atom creation, 1.0)

	for (Size i = 0; i < N; i++)
	{
		START_TIMER
			Atom* atom = new Atom;
		STOP_TIMER
		atoms.push_back(atom);
	}
	
END_SECTION

START_SECTION(Atom destruction, 1.0)

	for (Size i = 0; i < N; i++)
	{
		Atom* atom = atoms[i];
		START_TIMER
			delete atom;
		STOP_TIMER
	}
	atoms.clear();
	
END_SECTION


vector<Fragment*> fragments;
fragments.reserve(N);
START_SECTION(Fragment creation, 1.0)

	for (Size i = 0; i < N; i++)
	{
		START_TIMER
			Fragment* fragment = new Fragment;
		STOP_TIMER
		fragments.push_back(fragment);
	}
	
END_SECTION

START_SECTION(Fragment destruction, 1.0)

	for (Size i = 0; i < N; i++)
	{
		START_TIMER
			delete fragments[i];
		STOP_TIMER
	}
	fragments.clear();
	
END_SECTION



vector<Residue*> residues;
residues.reserve(N);
START_SECTION(Residue creation, 1.0)

	for (Size i = 0; i < N; i++)
	{
		START_TIMER
			Residue* residue = new Residue;
		STOP_TIMER
		residues.push_back(residue);
	}
	
END_SECTION

START_SECTION(Residue destruction, 1.0)

	for (Size i = 0; i < N; i++)
	{
		START_TIMER
			delete residues[i];
		STOP_TIMER
	}
	residues.clear();
	
END_SECTION



vector<Molecule*> molecules;
molecules.reserve(N);
START_SECTION(Molecule creation, 1.0)

	for (Size i = 0; i < N; i++)
	{
		START_TIMER
			Molecule* molecule = new Molecule;
		STOP_TIMER
		molecules.push_back(molecule);
	}
	
END_SECTION

START_SECTION(Molecule destruction, 1.0)

	for (Size i = 0; i < N; i++)
	{
		START_TIMER
			delete molecules[i];
		STOP_TIMER
	}
	molecules.clear();
	
END_SECTION



vector<System*> systems;
systems.reserve(N);
START_SECTION(System creation, 1.0)

	for (Size i = 0; i < N; i++)
	{
		START_TIMER
			System* system = new System;
		STOP_TIMER
		systems.push_back(system);
	}
	
END_SECTION

START_SECTION(System destruction, 1.0)

	for (Size i = 0; i < N; i++)
	{
		START_TIMER
			delete systems[i];
		STOP_TIMER
	}
	systems.clear();
	
END_SECTION


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
