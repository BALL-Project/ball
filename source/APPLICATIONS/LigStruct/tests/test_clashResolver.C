// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
//#include <BALL/FORMAT/lineBasedFile.h>
//#include <BALL/SYSTEM/file.h>

#include <BALL/KERNEL/forEach.h>
#include <BALL/DATATYPE/string.h>

#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>

#include "../sources/clashResolver.h"
#include "../sources/ioModule.h"

using namespace BALL;
using namespace std;


/// ################################## C O M M A N D L I N E    P A R S E R
int main(int argc, char* argv[])
{
	CommandlineParser parpars("test ClashResolver", "  ", 0.1, String(__DATE__), "Testing");
	parpars.registerParameter("i", "input SDF", INFILE, true);
//	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	
//	parpars.registerParameter("c", "location of conf file", INFILE, false);
//	parpars.setSupportedFormats("c","conf");
	
	parpars.setSupportedFormats("i","sdf");
//	parpars.setSupportedFormats("o","sdf");
//	parpars.setOutputFormatSource("o","i");

	String manual = "...just playing...";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
/// C O D E ##################################

	Log << "Reading molecule..."<<endl;
	SDFile in_file(parpars.get("i"), ios::in);
	SDFile outfile("/Users/pbrach/out.sdf", ios::out);
	Molecule* mol = in_file.read();
	
	TemplateDatabaseManager lib_manag;
	lib_manag.libraryPathesFromConfig("/Users/pbrach/files/projects/Master-2014_2015/1_code/ball_ligandStruct/source/APPLICATIONS/LigStruct/examples/libraries.conf");
	lib_manag.readSiteTemplates();
	lib_manag.readBondLenths();
	
	ConnectionClashResolver c_resolver;
	
	outfile<< *mol;
	Log << "......done!"<<endl;
}

