#include <stdio.h>
#include <BALL/NMR/assignShiftProcessor.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/NMRStarFile.h>
#include <BALL/SYSTEM/path.h>

using namespace BALL;
using namespace std;

int main()
{
	PDBFile f;
	f.open("parvulin.pdb");
	Path path;

	System system;
	f >> system;
	FragmentDB frag_db(path.find("fragments/Fragments.db"));
	system.apply(frag_db.normalize_names);
	system.apply(frag_db.build_bonds);

	/*
	ResidueChecker rc(frag_db);
	system.apply(rc);
	
	PDBFile out("parvulin_out.pdb", File::OUT);
	out << system;
	out.close();
	return 0;*/

	NMRStarFile rs("data/bmr4789.str");
	cout << "Size: " << rs.getData()[0]->atomData.size() << endl;
	cout << "number of atoms: " << rs.getNumberOfAtoms() << endl;

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
		//	Log.info() << atom_it->getFullName() << " " << atom_it->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat() << endl;
		}
	}
 	cout << "numberOfShiftAtoms " << numberOfShiftAtoms << endl;
}
