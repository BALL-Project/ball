// $Id: assign_typenames_from_rules.C,v 1.1 2003/03/05 16:17:29 anker Exp $

#include <BALL/common.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/MOLMEC/COMMON/typenameRuleProcessor.h>

using namespace BALL;

int main(int argc, char** argv)
{
	Path path;
	String filename = path.find(argv[1]);
	if (filename == "")
	{
		Log.warn() << "Could not find rule file " << argv[1] << std::endl;
		return(1);
	}
	INIFile typename_rules(filename);
	typename_rules.read();
	TypenameRuleProcessor proc(typename_rules, "TypenameRules");

	HINFile infile(argv[2]);
	System system;
	infile >> system;
	infile.close();

	system.apply(proc);

	HINFile outfile(argv[3], std::ios::out);
	outfile << system;
	outfile.close();
}

