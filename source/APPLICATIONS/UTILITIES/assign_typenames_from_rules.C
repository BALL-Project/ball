// $Id: assign_typenames_from_rules.C,v 1.2 2003/03/28 14:41:40 anker Exp $

#include <BALL/common.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/MOLMEC/COMMON/typenameRuleProcessor.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	if (argc != 4)
	{
		cout << "Need 3 arguments: INIFile input.hin output.hin" << endl;
		return(1);
	}

	Path path;
	String filename = path.find(argv[1]);
	if (filename == "")
	{
		cerr << "Could not find rule file " << argv[1] << std::endl;
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

