// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: proteinMapper.C,v 1.10.14.2 2007/08/06 16:22:07 oliver Exp $
//
// Compute an optimal mapping of two proteins.
//
// Author:
//   Oliver Kohlbacher
//

#include <BALL/SYSTEM/timer.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/MATHS/matrix44.h>

#include <iostream>
#include <map>

using namespace std;
using namespace BALL;

int main(int argc, char** argv) 
{

	// default values
	double upper = 8.0;
	double lower = 4.0;
	double tolerance = 0.6;

	// print usage information
	if ((argc < 4) || (argc > 7)) 
	{
		cout << argv[0] << " <filenameA> <filenameB> <outfilename> [<upper> [<lower> [<tolerance>]]]" << endl;
		cout << "Maps the backbone of two proteins (Calpha atoms) onto each other." << endl
				 << "Calpha atoms are recognized by their name (CA)." << endl << endl
				 << "Optional parameters:" << endl
				 << "  <upper> (double):" << endl
				 << "     upper bound for triangle side length use by the mapper" << endl
				 << "  <lower> (double):" << endl
				 << "     lower bound for triangle side length use by the mapper" << endl
				 << "  <tolerance> (double):" << endl
				 << "     maximum distance between two (mapped) Calpha atoms in Angstrom" << endl << endl
				 << "Defaults for optional parameters:" << endl
				 << "    upper     = " << upper << endl
				 << "    lower     = " << lower << endl
				 << "    tolerance = " << tolerance << endl
				 << endl;

		return 1;
	}

	// set the optional parameters
	if (argc > 4)
	{
		upper = atof(argv[4]);	
	}

	if (argc > 5)
	{
		lower = atof(argv[5]);
	}

	if (argc > 6)
	{
		tolerance = atof(argv[6]);
	}

	map<String, Position> type_map;
	type_map["ALA"] = 0;
	type_map["GLY"] = 0;
	type_map["VAL"] = 0;
	type_map["LEU"] = 0;
	type_map["ILE"] = 0;
	type_map["SER"] = 0;
	type_map["CYS"] = 0;
	type_map["THR"] = 0;
	type_map["MET"] = 0;
	type_map["PHE"] = 0;
	type_map["TYR"] = 0;
	type_map["TRP"] = 0;
	type_map["PRO"] = 0;
	type_map["HIS"] = 0;
	type_map["LYS"] = 0;
	type_map["ARG"] = 0;
	type_map["ASP"] = 0;
	type_map["GLU"] = 0;
	type_map["ASN"] = 0;
	type_map["GLN"] = 0;


	// define two proteins
	Protein		protein1;
	Protein		protein2;

	// and a PDB file to read and write the proteins
	PDBFile	pdb_file;

	// read the proteins
	pdb_file.open(argv[1]);
	cout << "reading " << argv[1] << "... " << flush;
	pdb_file >> protein1;
	cout << "read " << protein1.countAtoms() << " atoms" << endl;
	pdb_file.close();


	pdb_file.open(argv[2]);
	cout << "reading " << argv[2] << "... " << flush;
	pdb_file >> protein2;
	cout << "read " << protein2.countAtoms() << " atoms" << endl;
	pdb_file.close();

	// map the two proteins
	Matrix4x4				T;
	StructureMapper	mapper;
	Size						no_ca;
	double					rmsd;
	
	cout << "mapping " << argv[1] << " onto " << argv[2] << " (this may take a while)..." << endl;

	Timer	t;
	t.start();

	T = mapper.mapProteins(protein1, protein2, type_map, no_ca, rmsd, upper, lower, tolerance);

	t.stop();

	if (no_ca < 1) 
	{
		cout << "Sorry - couldn't map the files (no CA atoms?)" << endl;
		return 1;
	}

	cout << "mapped " << no_ca << " CA atoms." << endl;
	cout << "RMSD (CA only): " << rmsd << " A" << endl;
	cout << endl << "Transformation: " << endl;
	cout << T;


	cout << "Time to map the proteins: " << t.getClockTime() << "s" << endl;
	cout << "transforming " << argv[1] << " and writing to " << argv[3] << "..." << endl;

	mapper.setTransformation(T);
	protein1.apply(mapper);

	PDBFile pdb_file2(argv[3], ios::out);
	pdb_file2 << protein1;
	pdb_file2.close();

	cout << "done." << endl << endl;

	return 0;
}
