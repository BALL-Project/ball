// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: tutorial3.C,v 1.6.26.3 2007/05/25 09:36:29 oliver Exp $
//

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
	// Read a structure from the PDB file
	PDBFile	infile("pdb4pti.ent");
	System S;
	infile >> S;
	infile.close();

	// Add the missing hydrogen atoms in a standard geometry,
	// normalizee the names to be PDB compliant and construct
	// missing bonds.
	FragmentDB db("");
	S.apply(db.normalize_names);
	S.apply(db.add_hydrogens);
	S.apply(db.build_bonds);

	// Make sure we have a reasonable structure.
	// The residue checker will print warnings for
	// numerous common problems like missing atoms,
	// overlaps, etc.
	ResidueChecker rc(db);
	S.apply(rc);

	// Construct an AMBER force field object
	// from our protein, select the added hydrogens
	// only and optimize the hydrogen positions
	// (heavy atoms will remain fixed (similar to 
	// a BELLY run in AMBER).
	AmberFF amber(S);
	Selector hydrogen_selector("element(H)");
	S.apply(hydrogen_selector);

	ConjugateGradientMinimizer cgm(amber);
	cgm.setEnergyOutputFrequency(1);
	cgm.setMaxGradient(1.0);
	std::cout << "Minimizer options:" << std::endl ;
	cgm.options.dump(std::cout);
	cgm.minimize(300);


	// Remove the selection again. We will now
	// perform a full MD run in a water box.
	S.deselect();

	// We call the setup method of AmberFF again
	// to add the missing water for the solvent box.
	amber.options[PeriodicBoundary::Option::PERIODIC_BOX_ENABLED] = "true";
	amber.options[PeriodicBoundary::Option::PERIODIC_BOX_ADD_SOLVENT] = "true";
	amber.setup(S);

	Log.info() << "Creating MDS..." << std::endl;
	CanonicalMD md(amber);
	md.setReferenceTemperature(300.0);
	md.setBathRelaxationTime(0.0001);
	md.setEnergyOutputFrequency(1);
	md.simulateIterations(100);

	std::cout << "Simulation settings:" << std::endl;
	md.options.dump(std::cout);

	// Store the final structure.
	PDBFile final("final.pdb", File::MODE_OUT);
	final << S;
	final.close();

	return 0;
}
