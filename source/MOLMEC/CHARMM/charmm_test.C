#include <BALL/MOLMEC/CHARMM/charmm.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>

using namespace BALL;
using namespace std;

int main()
{
	PDBFile f("test.pdb");
	System S;
	f >> S;
	f.close();	
	cout << "read " << S.countAtoms() << " atoms." << endl;

	//FragmentDB frag_db;
	//S.apply(*frag_db.buildBonds);

	CharmmFF FF;
	FF.setup(S);
	FF.updateEnergy();
	cout << "total energy: " << FF.getEnergy() << " kJ/mol" << endl;
	cout << "  - bend      " << FF.getBendEnergy() << " kJ/mol" << endl;
	cout << "  - stretch   " << FF.getStretchEnergy() << " kJ/mol" << endl;
	cout << "  - torsion   " << FF.getTorsionEnergy() << " kJ/mol" << endl;
	cout << "  - ES        " << FF.getESEnergy() << " kJ/mol" << endl;
	cout << "  - VdW       " << FF.getVdWEnergy() << " kJ/mol" << endl;
	cout << "  - solvation " << FF.getSolvationEnergy() << " kJ/mol" << endl;

	return 0;
}
