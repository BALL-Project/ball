// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: tutorial3.C,v 1.4.4.1 2004/05/10 12:04:18 amoll Exp $

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/MDSIMULATION/canonicalMD.h>

using namespace BALL;

int main()
{
	PDBFile	infile("pdb4pti.ent");
	System S;
	infile >> S;
	infile.close();

	FragmentDB db("");
	S.apply(db.normalize_names);

	S.apply(db.add_hydrogens);
	S.apply(db.build_bonds);

	ResidueChecker rc(db);
	S.apply(rc);

	AmberFF amber(S);

	Selector hydrogen_selector("element(H)");
	S.apply(hydrogen_selector);

	ConjugateGradientMinimizer cgm(amber);
	cgm.setEnergyOutputFrequency(1);
	cgm.setMaxGradient(5.0);
	std::cout << "Minimizer options:" << std::endl ;
	cgm.options.dump(std::cout);
	cgm.minimize(100);

	S.deselect();

	amber.options[PeriodicBoundary::Option::PERIODIC_BOX_ENABLED] = "true";
	amber.options[PeriodicBoundary::Option::PERIODIC_BOX_ADD_SOLVENT] = "true";
	amber.setup(S);

	Log.info() << "Creating MDS..." << std::endl;
	CanonicalMD md(amber);
	md.setReferenceTemperature(300);
	md.setEnergyOutputFrequency(1);
	md.simulateIterations(10);

	std::cout << "Simulation settings:" << std::endl;
	md.options.dump(std::cout);

	return 0;
}
