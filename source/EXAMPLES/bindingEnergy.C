// $Id: bindingEnergy.C,v 1.1 2000/01/06 21:34:16 oliver Exp $

#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/ENERGY/atomicContactEnergy.h>
#include <BALL/FORMAT/PDBFile.h>

using namespace BALL;

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		Log.error() << argv[0] << " <pdb file A> <pdb file B>" << endl;
		return 1;
	}

	// open the first PDB file
	PDBFile pdb(argv[1]);
	if (pdb.bad())
	{
		// if the file could not be opened, print error message and exit
		Log.error() << "cannot open PBD file " << argv[1] << endl;
		return 2;
	}

	// read the contents of the file A into a system
	System A;
	pdb >> A;
	pdb.close();

	// open the first PDB file
	pdb.open(argv[2]);
	if (pdb.bad())
	{
		// if the file could not be opened, print error message and exit
		Log.error() << "cannot open PBD file " << argv[2] << endl;
		return 2;
	}

	// read the contents of the file B into a system
	System B;
	pdb >> B;
	pdb.close();

	// normalize the names and build all bonds
	Log.info() << "normalizing names and building bonds..." << endl;
	FragmentDB db;
	A.apply(*db.normalizeNames);
	A.apply(*db.buildBonds);
	B.apply(*db.normalizeNames);
	B.apply(*db.buildBonds);

	// calculate the atomic contact energies of A and B
	float ACE_A = calculateACE(A);
	float ACE_B = calculateACE(B);
	
	// calculate the electrostatic energies of A and B
	AmberFF amber;
	amber.setup(A);
	amber.updateEnergy();
	float ES_A = amber.getESEnergy();
	amber.setup(B);
	amber.updateEnergy();
	float ES_B = amber.getESEnergy();

	// finally, join the to systems into a single system
	A.splice(B);
	float ACE_AB = calculateACE(A);
	amber.setup(A);
	amber.updateEnergy();
	float ES_AB = amber.getESEnergy();
	
	// print the resulting energies
	cout << "change in atomic contact energy on binding:   " 
       << (ACE_AB - ACE_A - ACE_B) << " kJ/mol" << endl;
	cout << "change in electrostatic energy on binding:    " 
       << (ES_AB - ES_A - ES_B) << " kJ/mol" << endl;
	cout << "total binding fre energy:                     "
			 << (ACE_AB - ACE_A - ACE_B) + (ES_AB - ES_A - ES_B) << " kJ/mol" << endl;

	// done
	return 0;
}
			

	
	
