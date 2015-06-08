// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/molecule.h>

using namespace BALL;
using namespace std;

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("Union Molecule Lists", " according to their names", 0.1, String(__DATE__), "Preparation");
	parpars.registerMandatoryInputFile("small", "small mol list");
	parpars.registerMandatoryInputFile("large", "large mol list");
	parpars.registerMandatoryOutputFile("o", "output mol");
	
	parpars.setSupportedFormats("small","sdf");
	parpars.setSupportedFormats("large","sdf");
	parpars.setSupportedFormats("o","sdf");

	String manual = "Keeps the common set of both input structures according to their names";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
/// C O D E ##################################
 
	SDFile in_file1(parpars.get("small"), ios::in);
	SDFile in_file2(parpars.get("large"), ios::in);
	SDFile outfile(parpars.get("o"), ios::out);
	
	
	Log << "Reading small list"<<endl;
	
	HashSet<String> unique_names;
	AtomContainer* mol = in_file1.read();
	
	while( mol )
	{
		unique_names.insert( mol->getName().trim() );
		delete mol;
		
		mol = in_file1.read();
	}
	cout<<"Read "<<unique_names.size()<< " names"<<endl;
	in_file1.close();
	
	Log << "Filtering large list"<<endl;
	
	mol = in_file2.read();
	int cnt_tot = 0;
	int taken = 0;
	
	while( mol )
	{
		cnt_tot++;

		if( unique_names.has( mol->getName().trim() ) )
		{
			taken++;
			 outfile << *mol;
		}

		delete mol;
		
		mol = in_file2.read();
	}
	in_file2.close();
	outfile.close();
	Log << "Large had "<<cnt_tot<<" molecules "<<endl;
	Log <<" from those "<< taken <<" agreed with small"<<endl;
	Log << "......done!"<<endl;
}

