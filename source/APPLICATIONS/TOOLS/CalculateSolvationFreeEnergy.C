// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/SOLVATION/poissonBoltzmann.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/STRUCTURE/fragmentDB.h>

#include <BALL/FORMAT/commandlineParser.h>

#include "version.h"

using namespace std;
using namespace BALL;

int main(int argc, char* argv[])
{
	FDPB fdpb;

	// instantiate CommandlineParser object
	CommandlineParser parpars("CalculateSolvationFreeEnergy", "calculate solvation free energy of a protein using AMBER ", VERSION, String(__DATE__), "ForceFields");
	parpars.registerMandatoryInputFile("pdb",  "input pdb file ");
	parpars.registerOptionalDoubleParameter("epsilon_medium", "dielectric constant in medium", fdpb.options.getReal(FDPB::Option::SOLVENT_DC));
	parpars.registerOptionalDoubleParameter("epsilon_vacuum", "dielectric constant in vacuum", 1.0);
	///TODO: do not generate results in stdout, rather, generate an output file!

	// the manual
	String man = String("This tool computes the solvation free energy of a pdb file using the Jackson-Sternberg approach (bonded energy using a force field and a non bonded energy (electrostatics only) by solving the Poisson-Boltzmann equation. Parameters are the dielectric constants for the medium (-epsilon_medium) and the vacuum (-epsilon_vacuum).");

	parpars.setToolManual(man);

	parpars.setSupportedFormats("pdb", "pdb");

	// parse the command line
	parpars.parse(argc, argv);


	PDBFile pdb;
	pdb.open(parpars.get("pdb"), std::ios::in);

	if (!pdb)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << parpars.get("pdb") << " for input." << std::endl;
		exit(2);
	}

	System sys;
	pdb >> sys;
	pdb.close();

	// normalize the names and build all bonds
	FragmentDB db("");
	sys.apply(db.normalize_names);
	sys.apply(db.build_bonds);

	// TODO: Ask ResidueChecker if everything is ok! see tool CalculateEnergy

	// create an AMBER force field without non-bonded interactions
	AmberFF FF(sys);

	// calculate the total energy
	float total_energy = FF.updateEnergy();
	//Log << FF.getResults() << std::endl;
	//Log << "   total energy using force field evaluation: " << total_energy << " kJ/mol" << std::endl;

	//Log << "removing non bonded energy terms ..." << std::endl;
	FF.removeComponent("Amber NonBonded");

	// calculate the internal energy (neglecting internal VdW interactions)
	float internal_energy = FF.updateEnergy();
	//Log << FF.getResults() << std::endl;
	Log << "  internal energy: " << internal_energy << " kJ/mol" << std::endl;

	// assign atom radii
	AssignRadiusProcessor radius_processor("radii/PARSE.siz");
	sys.apply(radius_processor);

	// calculate the electrostatic part of the solvation energy	
	//FDPB fdpb;

	float dielectric_const = fdpb.options.getReal(FDPB::Option::SOLVENT_DC);
	if (parpars.has("epsilon_medium"))
		dielectric_const = parpars.get("epsilon_medium").toFloat();
	fdpb.options[FDPB::Option::SOLVENT_DC] = dielectric_const;
	Log << "... using dielectric constant in medium: " << fdpb.options[FDPB::Option::SOLVENT_DC].toFloat() << std::endl;

	fdpb.setup(sys);
	fdpb.solve();

	float solvent_energy = fdpb.getEnergy();

	dielectric_const = 1.0;
	if (parpars.has("epsilon_vacuum"))
		dielectric_const = parpars.get("epsilon_vacuum").toFloat();
	fdpb.options[FDPB::Option::SOLVENT_DC] = dielectric_const;
	Log << "... using dielectric constant in vacuum: " << fdpb.options[FDPB::Option::SOLVENT_DC].toFloat() << std::endl;

	fdpb.setup(sys);
	fdpb.solve();

	float vacuum_energy = fdpb.getEnergy();
	Log << "\n  electrostatic solvation free energy: "	<< solvent_energy - vacuum_energy << std::endl;

	Log << "\n  total energy using a combination of force field and FDPB evaluation: "
		  << internal_energy - vacuum_energy + solvent_energy << " kJ/mol" << std::endl;

	return 0;
}
