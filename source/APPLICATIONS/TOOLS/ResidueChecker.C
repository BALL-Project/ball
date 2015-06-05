// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/STRUCTURE/reconstructFragmentProcessor.h>

#include <BALL/FORMAT/commandlineParser.h>

#include "version.h"

using namespace std;
using namespace BALL;

int main(int argc, char* argv[])
{
	// instantiate CommandlineParser object
	CommandlineParser parpars("ResidueChecker", "check residues to debug a protein structure wrt to PDB conventions", VERSION, String(__DATE__), "ForceFields");
	parpars.registerMandatoryInputFile("pdb",  "input pdb file ");

	// TODO: offer upload of a distinguished fragDB file? 
	// shall we normalize names according to force field conventions??
	parpars.registerFlag("norm_names", "ensures a consistent naming scheme for all atoms, e.g. PDB conventions", false);

	// shall we add bonds 
	parpars.registerFlag("build_bonds", "add missing bonds", false);

	// shall we apply the fragment reconstruction
	parpars.registerFlag("frag_reconstruct", "reconstruct incomplete fragments", false);

	// an now the tests:
	parpars.registerFlag("extra_atoms", "check for extra atoms, i.e. unknown in the reference fragment", false);
	parpars.registerFlag("bond_length", "check for invalid bond length", false);
	// charges
	parpars.registerFlag("int_net_charge", "check if integer charges", false);
	parpars.registerFlag("large_charges", "check for too large charges", false);
	parpars.registerFlag("large_net_charge", "check for too large net charge", false);

	parpars.registerFlag("overlapping_atoms", "check for overlapptin atom positions", false);
	parpars.registerFlag("nan_positions", "check for ill-valued atomic positions", false);
	parpars.registerFlag("elements", "check if atom names reflect the atomic element", false);
	parpars.registerFlag("dublicate_atom_names", "check for dublicated atom names", false);
	parpars.registerFlag("unknown_residues", "check for unknown residues", false);

	//TODO: offer write Logs to a pdf

	// the manual
	String man = String("This tool checks the residues of a pdb file wrt. common inconsistencies such as missing atoms or suspicious distances.");

	parpars.setToolManual(man);

	parpars.setSupportedFormats("pdb", "pdb");

	// parse the command line
	parpars.parse(argc, argv);

	bool norm_names = false;
	if (parpars.has("norm_names"))
	{
		norm_names  = parpars.get("norm_names").toBool();
	}

	bool build_bonds = false;
	if (parpars.has("build_bonds"))
	{
		build_bonds = parpars.get("build_bonds").toBool();
	}

	bool frag_reconstruct = false;
	if (parpars.has("frag_reconstruct"))
	{
		frag_reconstruct = parpars.get("frag_reconstruct").toBool();
	}

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

	bool changes = false;

	// normalize the names and build all bonds
	FragmentDB db("");

	if (norm_names)
	{
		sys.apply(db.normalize_names);
		changes = true;
	}

	if (build_bonds)
	{
		sys.apply(db.build_bonds);
		changes = true;
	}

	if (frag_reconstruct)
	{
		ReconstructFragmentProcessor proc(db);
		sys.apply(proc);
		sys.apply(db.build_bonds);
		changes = true;
	}

	// now specify the ResidueChecker
	ResidueChecker check(db);

	//MISSING_ATOMS
	if (parpars.has("missing_atoms"))
	{
		check.enable(ResidueChecker::MISSING_ATOMS, parpars.get("missing_atoms").toBool());
	}

	//EXTRA_ATOMS
	if (parpars.has("extra_atoms"))
	{
		check.enable(ResidueChecker::EXTRA_ATOMS, parpars.get("extra_atoms").toBool());
	}

	// ELEMENTS, 	
	if (parpars.has("elements"))
	{
		check.enable(ResidueChecker::ELEMENTS, parpars.get("elements").toBool());
	}

	// SUSPECT_BOND_LENGTHS, 
	if (parpars.has("bond_length"))
	{
		check.enable(ResidueChecker::SUSPECT_BOND_LENGTHS, parpars.get("bond_length").toBool());
	}

  // NON_INTEGRAL_NET_CHARGE, //TODO: meaning reasonable or integer ??
	if (parpars.has("net_charge"))
	{
		check.enable(ResidueChecker::NON_INTEGRAL_NET_CHARGE, parpars.get("net_charge").toBool());
	}

	// LARGE_CHARGES, 	
	if (parpars.has("large_charges"))
	{
		check.enable(ResidueChecker::LARGE_CHARGES, parpars.get("large_charges").toBool());
	}

	// LARGE_NET_CHARGE, 
	if (parpars.has("large_net_charge"))
	{
		check.enable(ResidueChecker::LARGE_NET_CHARGE, parpars.get("large_net_charge").toBool());
	}

	// NAN_POSITIONS, 
	if (parpars.has("nan_positions"))
	{
		check.enable(ResidueChecker::NAN_POSITIONS, parpars.get("nan_positions").toBool());
	}

  // OVERLAPPING_ATOMS,
	if (parpars.has("overlapping_atoms"))
	{
		check.enable(ResidueChecker::OVERLAPPING_ATOMS, parpars.get("overlapping_atoms").toBool());
		check.enable(ResidueChecker::STRONGLY_OVERLAPPING_ATOMS, parpars.get("overlapping_atoms").toBool());
	}

	// DUPLICATE_ATOM_NAMES, 
	if (parpars.has("dublicate_atom_names"))
	{
		check.enable(ResidueChecker::DUPLICATE_ATOM_NAMES, parpars.get("dublicate_atom_names").toBool());
	}

	// UNKNOWN_RESIDUES, 
	if (parpars.has("unknown_residues"))
	{
		check.enable(ResidueChecker::UNKNOWN_RESIDUES, parpars.get("unknown_residues").toBool());
	}


	// set specified tests
	sys.apply(check);

	// overwrite
	if (changes)
	{
		pdb.open(parpars.get("pdb"), ios::out);
		pdb << sys;
		pdb.close();
		Log << "changes are written " <<  std::endl;
	}

	return 0;
}
