// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: assign_charges_from_rules.C,v 1.1 2006/05/21 10:18:10 anker Exp $
//

#include <BALL/common.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/MOLMEC/COMMON/chargeRuleProcessor.h>
#include <BALL/STRUCTURE/fragmentDB.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	if (argc != 4)
	{
		cout << "Need 3 arguments: INIFile input.pdb output.hin" << endl;
		return(1);
	}

	Path path;
	String filename = path.find(argv[1]);
	if (filename == "")
	{
		cerr << "Could not find rule file " << argv[1] << std::endl;
		return(1);
	}

	INIFile charge_rules(filename);
	charge_rules.read();
	ChargeRuleProcessor proc(charge_rules, "ChargeRules");

	PDBFile infile(argv[2]);
	System system;
	infile >> system;
	infile.close();

	FragmentDB db("");
	system.apply(db.build_bonds);
	system.apply(db.normalize_names);

	system.apply(proc);

	// PARANOIA
	ResidueConstIterator res_it = system.beginResidue();

	for (; +res_it; ++res_it)
	{
		float charge = 0.0f;
		AtomConstIterator it = res_it->beginAtom();
		for (; +it; ++it)
		{
			float c = it->getCharge();
			if (fabs(c) > 4.0f)
			{
				Log.warn() << "WARNING: atom " << it->getFullName() << ":"
					<< res_it->getID() << " has suspect charge " << c << endl;
			}

			charge += c;
		}

		if (fabs(charge - roundf(charge)) > 0.05)
		{
			Log.warn() << "WARNING: residue " << res_it->getName() << ":" 
				<< res_it->getID() << " has non-integral charge "
				<< charge << endl;
		}
	}

	HINFile outfile(argv[3], std::ios::out);
	outfile << system;
	outfile.close();
}

