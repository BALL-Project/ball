#include <stdio.h>
#include <BALL/NMR/assignShiftProcessor.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/NMRStarFile.h>

using namespace BALL;
using namespace std;

int main()
{
	PDBFile f;
	f.open("parvulin.pdb");

	System system;
	f >> system;
	FragmentDB frag_db;
	system.apply(frag_db.normalize_names);
	system.apply(frag_db.build_bonds);
	ResidueChecker rc(frag_db);
	system.apply(rc);
	
	PDBFile out("parvulin_out.pdb", File::OUT);
	out << system;
	out.close();
	return 0;

	//NMRStarFile rs("data/parvulin.str");
	NMRStarFile rs("data/bmr4789.str");
	cout << "Size: " << rs.getData()[0]->atomData.size() << endl;
	cout << "number of atoms: " << rs.getNumberOfAtoms() << endl;
	/*
	for (int i = 0;  i < rs.getData()[0]->atomData.size(); i++)
	{
		cout << *(rs.getData()[0]->atomData[i]);
	}
	*/
	

	Log.info() << "System: #atoms = " << system.countAtoms() << endl;
	AssignShiftProcessor asp(rs.getData()[0]->atomData);
	system.apply(asp);

	int numberOfShiftAtoms = 0;
	AtomIterator atom_it = system.beginAtom();
	for (; +atom_it; ++atom_it)
	{
		if (atom_it->hasProperty(ShiftModule::PROPERTY__SHIFT))
		{
			numberOfShiftAtoms++;
		}
		else
		{
			Log.info() << atom_it->getFullName() << " " << atom_it->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat() << endl;
		}
	}
 	cout << "numberOfShiftAtoms " << numberOfShiftAtoms << endl;
}


/*
	for (int i = 0; i < system.countAtoms(); i++)
	{
		cout << i << " " <<system.getAtom(i)->getName() << endl;
	}*/

/*
	cout << system.countAtoms() << " atome" << endl << endl;
	int numberOfAA = 0;
	Fragment* fp = 0;
	for (int i = 0; i < system.countAtoms(); i++)
	{
		if (system.getAtom(i)->getFragment() != fp)
		{
			numberOfAA++;
			fp = system.getAtom(i)->getFragment();
		}
 		cout << i << " " <<system.getAtom(i)->getFragment()->getName()
			<< "" << numberOfAA
			<< ":" << system.getAtom(i)->getName()<< endl;
	}*/
/*
	cout << system.countMolecules() << " molekuele" << endl << endl;

	cout << system.countFragments() << " fragmente" << endl << endl;
	for (Position posfragment = 1; posfragment <= system.countFragments(); posfragment++)
	{
		for (Position posatom = 0; 
				 posatom < system.getMolecule(0)->getFragment(posfragment)->countAtoms();
				 posatom++)
		{
			cout << system.getMolecule(0)->getName()
			<< posfragment
			<< ":" << system.getMolecule(0)->getAtom(posatom)->getName() << endl;
		}
	}*/
