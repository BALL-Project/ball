#include <BALL/FORMAT/PDBFile.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/COMMON/assignTypes.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/STRUCTURE/fragmentDB.h>

using namespace BALL;

int main(int argc, char** argv)
{
	// issue a usage hint if called without parameters
	if (argc != 3)
	{
		cout << argv[0] << " <PDB infile> <PDB outfile>" << endl;
		return 1;
	}


	// open a PDB file with the name of the first argument
	PDBFile file(argv[1]);
	if (!file)
	{
		// if file does not exist: complain and abort
		cerr << "error opening " << argv[1] << " for input." << endl;
		return 2;
	}
	
	// create a system and read the contents of the PDB file
	System S;
	file >> S;
	file.close();

	// print the number of atoms read from the file
	cout << "read " << S.countAtoms() << " atoms." << endl;


	// now we open a fragment database
	cout << "reading fragment DB..." << endl;
	FragmentDB fragment_db;

	// and normalize the atom names, i.e. we convert different
	// naming standards to the PDB naming scheme - just in case!
	cout << "normalizing names..." << endl;
	S.apply(fragment_db.normalize_names);

	// now we add any missing hydrogens to the residues
	// the data on the hydrogen positions stems from the
	// fragment database. However the hydrogen positions 
	// created in this way are only good estimates
	cout << "adding hydrogens..." << endl;
	S.apply(fragment_db.add_hydrogens);

	// now we create the bonds between teh atoms (PDB files hardly
  // contain a complete set of CONECT records)																							
	cout << "building bonds..." << endl;
	S.apply(fragment_db.build_bonds);

	// now we check whether the model we built is consistent
	// The ResidueChecker checks for charges, bond lengths,
	// and missing atoms
	cout << "checking the built model..." << endl;
	ResidueChecker checker(fragment_db);
	S.apply(checker);

	// now we create an AMBER force field 
	cout << "setting up force field..." << endl;
	AmberFF FF;
	FF.setup(S);

	// we then select all hydrogens (element(H))
	// using a specialized processor (Selector)
	Selector selector("element(H)");
	S.apply(selector);
	
	// just for curiosity: we iterate over all 
	// atoms and count the number of selected atoms
	AtomIterator atom_it = S.beginAtom();
	Size counter = 0;
	for (; +atom_it; ++atom_it)
	{
		// if the atom is selected: increment "counter"
		if (atom_it->isSelected())
		{
			counter++;
		}
	}
	cout << "optimizing " << counter << " hydrogen atoms." << endl;

	
	// now we create a minimizer object that uses
	// a conjugate gradient algorithm to optimize the positions of
	// atoms
	ConjugateGradientMinimizer minimizer;

	// we only want to optimize the positions of the selected
	// atoms (hydrogens) - everything else remains in place!
	FF.setUseSelection(true);

	// calculate the total energy of the system
	float initial_energy = FF.updateEnergy();

	// initialize the minimizer and perform (up to)
	// 10000 optimization steps
	minimizer.setup(FF);
	minimizer.minimize(10000);

	// calculate the terminal energy and print it
	float terminal_energy = FF.getEnergy();

	cout << "energy before/after minimization: " << initial_energy << "/" << terminal_energy << " kJ/mol" << endl;
	
	// write the optimized structure to a file whose
	// name is given as the second command line argument
	cout << "writing PBD file " << argv[2] << endl;
	file.open(argv[2], ios::out);
	file << S;
	file.close();

	// done.
	return 0;
}

	
	
	
	
