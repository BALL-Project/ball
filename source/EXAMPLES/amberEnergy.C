// $Id: amberEnergy.C,v 1.3 2000/01/07 22:01:57 oliver Exp $

#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/FORMAT/PDBFile.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	if ((argc < 2) || (argc > 4))
	{
		Log.error() << argv[0] << " <pdb file> [<option file>]" << endl;
		return 1;
	}

	// open the PDB file
	PDBFile pdb(argv[1]);
	if (!pdb)
	{
		// if the file could not be opened, print error message and exit
		Log.error() << "cannot open PBD file " << argv[1] << endl;
		return 2;
	}

	// read the contents of the file into a system
	System S;
	pdb >> S;
	pdb.close();

	// normalize the names and build all bonds
	Log.info() << "normalizing names and building bonds..." << endl;
	FragmentDB db;
	S.apply(*db.normalizeNames);
	S.apply(*db.buildBonds);

	// create an AMBER force field and set its options from a file 
	AmberFF amber;
	if (argc > 2)
	{
		amber.options.readOptionFile(argv[2]);
	}

	// setup the force field
	Log.info() << "setting up force field..." << endl;
	amber.setup(S);

	// calculate the total energy and print it
	Log.info() << "total energy: " << amber.updateEnergy() << " kJ/mol   "
						 << "stretch: " << amber.getStretchEnergy() << " kJ/mol   "
						 << "bend: " << amber.getBendEnergy() << " kJ/mol   "
						 << "torsion: " << amber.getTorsionEnergy() << " kJ/mol   "
						 << "ES: " << amber.getESEnergy() << " kJ/mol   "
						 << "vdW: " << amber.getVdWEnergy() << " kJ/mol" 
					   << endl;
	
	// done
	return 0;
}
			

	
	
