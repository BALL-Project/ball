// $Id: bindingEnergy.C,v 1.3 2000/02/16 19:18:32 oliver Exp $

#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/ENERGY/atomicContactEnergy.h>
#include <BALL/ENERGY/coulomb.h>
#include <BALL/FORMAT/PDBFile.h>

using namespace BALL;
using namespace std;

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
	A.apply(db.normalize_names);
	A.apply(db.build_bonds);
	B.apply(db.normalize_names);
	B.apply(db.build_bonds);
	

	// calculate the atomic contact energies of A and B
	float ACE_A = calculateACE(A);
	float ACE_B = calculateACE(B);
	
	// calculate the electrostatic energies of A and B
	AmberFF amber;
	amber.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
	amber.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
	amber.setup(A);
	amber.updateEnergy();
	float ES_A = amber.getESEnergy();
	float C_A = calculateCoulomb(A);
	amber.setup(B);
	amber.updateEnergy();
	float ES_B = amber.getESEnergy();
	float C_B = calculateCoulomb(B);

	// finally, join the to systems into a single system
	cout << "atoms in A:  " << A.countAtoms() << endl;
	cout << "atoms in B:  " << B.countAtoms() << endl;
	A.splice(B);
	cout << "final atoms: " << A.countAtoms() << endl;
	float ACE_AB = calculateACE(A);
	amber.setup(A);
	amber.updateEnergy();
	float ES_AB = amber.getESEnergy();
	float C_AB = calculateCoulomb(A);
	
	// print the resulting energies
	cout << "ES energy of A: " << ES_A << endl;
	cout << "ES energy of B: " << ES_B << endl;
	cout << "ES energy of AB:" << ES_AB << endl;
	cout << "C energy of A: " << C_A << endl;
	cout << "C energy of B: " << C_B << endl;
	cout << "C energy of AB:" << C_AB << endl;
	cout << "change in atomic contact energy on binding:   " 
       << (ACE_AB - ACE_A - ACE_B) << " kJ/mol" << endl;
	cout << "change in electrostatic energy on binding:    " 
       << (ES_AB - ES_A - ES_B) << " kJ/mol" << endl;
	cout << "total binding fre energy:                     "
			 << (ACE_AB - ACE_A - ACE_B) + (ES_AB - ES_A - ES_B) << " kJ/mol" << endl;

	// done
	return 0;
}
			

	
	
