#include <BALL/MOLMEC/CHARMM/charmm.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/defaultProcessors.h>

using namespace BALL;
using namespace std;

int main()
{
	PDBFile f("test.pdb");
	System S;
	f >> S;
	f.close();	
	cout << "read " << S.countAtoms() << " atoms." << endl;

	FragmentDB frag_db;
	S.apply(*frag_db.normalizeNames);
	S.apply(*frag_db.buildBonds);

	CharmmFF FF;

	FF.removeComponent("CHARMM NonBonded");
	FF.removeComponent("CHARMM Bend");
	FF.removeComponent("CHARMM Stretch");
	FF.removeComponent("CHARMM Torsion");
//	FF.removeComponent("CHARMM ImproperTorsion");

	// FF.options[CharmmFF::Option::USE_EEF1] = "false";
	FF.setup(S);
	
	FF.updateEnergy();
	FF.updateForces();
	cout << "total energy: " << FF.getEnergy() / 4.184 << " kcal/mol" << endl;
	cout << "  - bend      " << FF.getBendEnergy() / 4.184 << " kcal/mol" << endl;
	cout << "  - stretch   " << FF.getStretchEnergy() / 4.184 << " kcal/mol" << endl;
	cout << "  - torsion   " << FF.getProperTorsionEnergy() / 4.184 << " kcal/mol" << endl;
	cout << "  - impropers " << FF.getImproperTorsionEnergy() / 4.184 << " kcal/mol" << endl;
	cout << "  - ES        " << FF.getESEnergy() / 4.184 << " kcal/mol" << endl;
	cout << "  - VdW       " << FF.getVdWEnergy() / 4.184 << " kcal/mol" << endl;
	cout << "  - solvation " << FF.getSolvationEnergy() / 4.184 << " kcal/mol" << endl;

	HINFile hin("test.hin", ios::out);
	hin << S;
	hin.close();

	const float factor = Constants::NA * 1e-13 / -4.184;
	//const float factor = 1.0;
	AtomIterator it = S.beginAtom();
	for (; +it; ++it)
	{	
		cout << it->getFullName() << "    " << it->getCharge() << "  " << it->getForce() * factor << endl;
	}

/*
	ClearChargeProcessor charges;
	S.apply(charges);
	FF.updateForces();
	it = S.beginAtom();
	for (; +it; ++it)
	{	
		cout << it->getFullName() << "    " << it->getCharge() << "  " << it->getForce() * factor << endl;
	}
*/

	return 0;
}
