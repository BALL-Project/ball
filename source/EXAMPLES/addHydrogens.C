#include <BALL/FORMAT/PDBFile.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/COMMON/assignTypes.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/STRUCTURE/fragmentDB.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	Log.setPrefix(cout, "[%T] ");
	Log.setPrefix(cerr, "[%T] ERROR: ");

	// issue a usage hint if called without parameters
	if (argc != 3)
	{
		Log << argv[0] << " <PDB infile> <PDB outfile>" << endl;
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
	
	// create a system and read the contents of the PDB file
	System S;
	file >> S;
	file.close();

	// print the number of atoms read from the file
	Log << "read " << S.countAtoms() << " atoms." << endl;


	// now we open a fragment database
	Log << "reading fragment DB..." << endl;
	FragmentDB fragment_db;

	// and normalize the atom names, i.e. we convert different
	// naming standards to the PDB naming scheme - just in case!
	Log << "normalizing names..." << endl;
	S.apply(fragment_db.normalize_names);

	// now we create the bonds between the atoms (PDB files hardly
  // ever contain a complete set of CONECT records)																							
	Log << "building bonds..." << endl;
	S.apply(fragment_db.build_bonds);

	// now we add any missing hydrogens to the residues
	// the data on the hydrogen positions stems from the
	// fragment database. However the hydrogen positions 
	// created in this way are only good estimates
	Log << "adding hydrogens..." << endl;
	S.apply(fragment_db.add_hydrogens);	
	Log << "added " << fragment_db.add_hydrogens.getNumberOfInsertedHydrogens() 
			<< " hydrogen atoms" << endl;

	// now we check whether the model we built is consistent
	// The ResidueChecker checks for charges, bond lengths,
	// and missing atoms
	Log << "checking the built model..." << endl;
	ResidueChecker checker(fragment_db);
	S.apply(checker);
	
	// now we create an AMBER force field 
	Log << "setting up force field..." << endl;
	AmberFF FF;

	// we then select all hydrogens (element(H))
	// using a specialized processor (Selector)
	S.deselect();
	Selector selector("element(H)");
	S.apply(selector);
	FF.setup(S);
	
	// just for curiosity: check how many atoms we are going
	// to optimize
	Log << "optimizing " << FF.getNumberOfMovableAtoms() << " out of " << S.countAtoms() << " atoms" << endl;
	
	// now we create a minimizer object that uses a conjugate 
	// gradient algorithm to optimize the atom positions
	ConjugateGradientMinimizer minimizer;

	// we only want to optimize the positions of the selected
	// atoms (hydrogens) - everything else remains in place!
	FF.setUseSelection(true);

	// calculate the total energy of the system
	float initial_energy = FF.updateEnergy();

	// initialize the minimizer and perform (up to)
	// 1000 optimization steps
	minimizer.setup(FF);
	minimizer.setEnergyOutputFrequency(1);
	minimizer.minimize(1000);

	// calculate the terminal energy and print it
	float terminal_energy = FF.getEnergy();

	Log << "energy before/after minimization: " << initial_energy << "/" << terminal_energy << " kJ/mol" << endl;

	// write the optimized structure to a file whose
	// name is given as the second command line argument
	Log << "writing PBD file " << argv[2] << endl;
	file.open(argv[2], ios::out);
	file << S;
	file.close();

	// done.
	return 0;
}

	
	
	
	
