// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: readMMFF94TestFile.C,v 1.1.2.2 2005/03/21 16:43:01 amoll Exp $
//
// A small program for adding hydrogens to a PDB file (which usually comes
// without hydrogen information) and minimizing all hydrogens by means of a
// conjugate gradient minimizer.

#include <BALL/common.h>

#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>


using namespace std;
using namespace BALL;

System* readTestFile(String filename)
{
	MOL2File mol2_file(filename);
	System* system = new System;
	mol2_file >> *system;

	filename = filename.getSubstring(0, filename.size() - 4);
	filename += "atoms";

	vector<String> atoms, names, symbols;
	vector<float> charges, fcharges;
	vector<short> types;
	vector<String> fields;

	HashMap<String, Position> name_to_pos;
	Position pos = 0;
	
	LineBasedFile infile(filename);
	while (infile.readLine())
	{
		if (infile.getLine().split(fields) != 6)
		{
			Log.error() << "Error in " << filename << " Not 6 fields in one line " << infile.getLine() << std::endl;
			return 0;
		}

		atoms.push_back(fields[0]);
		types.push_back(fields[2].toUnsignedShort());
		charges.push_back(fields[4].toFloat());
		fcharges.push_back(fields[5].toFloat());

		name_to_pos[fields[0]] = pos;
		pos ++;
	}

	infile.close();

	AtomIterator ait;
	BALL_FOREACH_ATOM(*system, ait)
	{
		if (!name_to_pos.has(ait->getName()))
		{
			Log.error() << "We have no data for the atom " << ait->getName() << " in file " << filename << std::endl;
			continue;
		}

		Position pos = name_to_pos[ait->getName()];
		ait->setType(types[pos]);
		ait->setCharge(charges[pos]);
	}

	return system;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		Log.error() << "Usage: readMMFF94TestFile <location of <test>.mol2>" << std::endl;
		return 1;
	}

	System* system = readTestFile(argv[1]);
	if (system == 0) return -1;

	MMFF94 mmff;
	if (!mmff.setup(*system))
	{
		Log.error() << "Setup failed for " << argv[1] << std::endl;
		return -1;
	}

	mmff.updateEnergy();

	Log.info () << "Energy for " << argv[1] << " : " << mmff.getEnergy() << std::endl;

	delete system;

	return 0;
}
