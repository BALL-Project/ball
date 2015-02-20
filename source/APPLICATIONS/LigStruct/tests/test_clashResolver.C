// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>

#include "../sources/clashResolver.h"
#include "../sources/ioModule.h"

using namespace BALL;
using namespace std;

int main(int argc, char* argv[])
{
	/// ################################## C O M M A N D L I N E    P A R S E R
	CommandlineParser parpars("test ClashResolver", "  ", 0.1, String(__DATE__), "Testing");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	
	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setOutputFormatSource("o","i");
	
	String manual = "...just testing clashes...";
	parpars.setToolManual(manual);
	
	parpars.parse(argc, argv);
	
	/// ################################## C O D E
	
	Log << "Reading molecule..."<<endl;
	SDFile in_file(parpars.get("i"), ios::in);
	SDFile outfile(parpars.get("o"), ios::out);
	Molecule* mol = in_file.read();
	
	TemplateDatabaseManager lib_manag;
	lib_manag.libraryPathesFromConfig("/Users/pbrach/files/projects/Master-2014_2015/1_code/ball_ligandStruct/source/APPLICATIONS/LigStruct/examples/libraries.conf");
	lib_manag.readSiteTemplates();
	lib_manag.readBondLenths();
	
	ConnectionClashResolver c_resolver;
	
	outfile<< *mol;
	Log << "......done!"<<endl;
}

