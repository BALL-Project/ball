// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/common.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>
#include <BALL/STRUCTURE/fragmentDB.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	if (argc != 4)
	{	
		Log.info() << "Need 3 arguments: <INIFile> <input.pdb> <output.hin>" << endl;
		Log.info() << "Note: exemplary rules can be found in BALL/data/solvation/PARSE.rul." << endl;
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
	RadiusRuleProcessor proc(charge_rules, "RadiusRules");

	PDBFile infile(argv[2]);
	if (!infile)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << argv[2] << " for input." << endl;
		return 2;
	}

	System system;
	infile >> system;
	infile.close();

	Log << "Initializing FragmentDB..." << endl;
	FragmentDB db("");
	Log << "  done." << endl;
	
	Log << "Normalizing names..." << endl;
	system.apply(db.normalize_names);
	Log << "  done." << endl;
	
	Log << "Building Bonds..." << endl;
	system.apply(db.build_bonds);
	Log << "  done." << endl;

	Log << "Applying RadiusRuleProcessor..." << endl;
	system.apply(proc);
	Log << "  done." << endl;

	Log << "Checking results..." << endl;
	// PARANOIA
	ResidueConstIterator res_it = system.beginResidue();

	for (; +res_it; ++res_it)
	{
		AtomConstIterator it = res_it->beginAtom();
		for (; +it; ++it)
		{
			float c = it->getCharge();
			if (fabs(c) > 4.0f)
			{
				Log.warn() << "WARNING: atom " << it->getFullName() << ":"
					<< res_it->getID() << " has suspect charge " << c << endl;
			}
		}
	}
	Log << "  done." << endl;

	HINFile outfile(argv[3], std::ios::out);
	outfile << system;
	outfile.close();	
	Log << "Outputfile " << argv[3] << " written." << endl;

}

