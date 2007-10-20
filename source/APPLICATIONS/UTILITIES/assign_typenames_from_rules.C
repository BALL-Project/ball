// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: assign_typenames_from_rules.C,v 1.4.28.1 2007/08/07 18:26:17 oliver Exp $
//

#include <BALL/common.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/MOLMEC/COMMON/typenameRuleProcessor.h>
#include <BALL/STRUCTURE/fragmentDB.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	if (argc != 4)
	{
		Log.info() << "Need 3 arguments: <INIFile> <input.hin> <output.hin>" << endl;	
		Log.info() << "Note: exemplary rules can be found in BALL/data/Amber/amber_types.rul." << endl;

		return(1);
	}

	Path path;
	String filename = path.find(argv[1]);
	if (filename == "")
	{
		Log.error() << "Could not find rule file " << argv[1] << std::endl;
		return(1);
	}

	Log << "Reading the typename rules..." << endl;
	INIFile typename_rules(filename);
	typename_rules.read();
	TypenameRuleProcessor proc(typename_rules, "TypenameRules");
	Log << "  done." << endl;


	HINFile infile(argv[2]);	
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

	Log << "Applying TypenameRuleProcessor..." << endl;
	system.apply(proc);
	Log << "  done." << endl;

	HINFile outfile(argv[3], std::ios::out);
	if (!outfile)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << argv[3] << " for output." << endl;
		return 2;
	}
	outfile << system;
	outfile.close();	
	Log << "Outputfile " << argv[3] << " written." << endl;

}

