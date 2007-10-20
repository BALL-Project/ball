// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//============================================================================
// BALL -  Example for a energy evaluations as it was used in Althaus 
// et al. "A branch and cut algorithm for the optimal solution of the 
// side-chain placement problem", 2000
//
// This example reads a PDB file and calculates a bonded energy using a force
// field and a non bonded energy (electrostatics only) by solving the Poisson-
// Boltzmann equation.
//============================================================================


#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/SOLVATION/poissonBoltzmann.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/STRUCTURE/fragmentDB.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{	
	// issue a usage hint if called without parameters
	if (argc != 2)
	{
		Log << argv[0] << " <PDB infile>" << endl;
		return 1;
	}


	// open a PDB file with the name of the first argument
	PDBFile file(argv[1]);
	if (!file)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << argv[1] << " for input." << endl;
		return 2;
	}
	
	System s;
	file >> s;
	
	// normalize the names and build all bonds
	Log.info() << "normalizing names and building bonds..." << endl;
	FragmentDB db("");
	s.apply(db.normalize_names);
	s.apply(db.build_bonds);

	// create an AMBER force field without non-bonded interactions
	AmberFF FF(s);
	
	// calculate the total energy
	float total_energy = FF.updateEnergy();
	Log.info() << FF.getResults() << std::endl;
	Log.info() << "   total energy using force field evaluation: " << total_energy << " kJ/mol" << endl;
	
	Log.info() << "removing non bonded energy terms ..." << endl;
	FF.removeComponent("Amber NonBonded");
	
	// calculate the internal energy (neglecting internal VdW interactions)
	float internal_energy = FF.updateEnergy();
	Log.info() << FF.getResults() << std::endl;
	Log.info() << "   internal energy: " << internal_energy << " kJ/mol" << endl;

	
	// assign atom radii
	AssignRadiusProcessor radius_processor("radii/PARSE.siz");
	s.apply(radius_processor);

	// calculate the electrostatic part of the solvation energy	
	Log.info() << "calculating electrostatic energy terms with FD-Poisson-Boltzmann ..." << endl;

	FDPB fdpb(s);
	fdpb.solve();
	Log.info() << "... using dielectric constant in medium: " << fdpb.options[FDPB::Option::SOLVENT_DC].toFloat() << std::endl;
	float solvent_energy = fdpb.getEnergy();
	
	fdpb.options[FDPB::Option::SOLVENT_DC] = 1.0;
	Log.info() << "... using dielectric constant in vacuum: " << fdpb.options[FDPB::Option::SOLVENT_DC].toFloat() << std::endl;
	fdpb.setup(s);
	fdpb.solve();
	float vacuum_energy = fdpb.getEnergy();
	Log.info() << "\n   electrostatic solvation free energy: " 
		<< solvent_energy - vacuum_energy << endl;
	
	Log.info() << "   total energy using a combination of force field and FDPB evaluation: " << internal_energy - vacuum_energy + solvent_energy << " kJ/mol" << endl;
	return 0;
}
