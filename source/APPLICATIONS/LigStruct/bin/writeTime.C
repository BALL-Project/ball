// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/molecule.h>

#include "../sources/base.h"

using namespace BALL;
using namespace std;

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("Test time to write", " ", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input", INFILE, true);
	parpars.registerParameter("o", "output ", OUTFILE, true);
	
	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setOutputFormatSource("o","i");

	String manual = "measure time needed to write a AtomContainer to SDF";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
/// C O D E ##################################
 
	SDFile in_file(parpars.get("i"), ios::in);
	
	list<AtomContainer*> mol_list;
	
	Log << "Reading input"<<endl;
	
	AtomContainer* mol = in_file.read();
	Timer my_timer;

	int cnt = 0;
	my_timer.start();
	while( mol && cnt < 100000 )
	{
		mol_list.push_back( mol );
		
		cnt++;
		mol = in_file.read();
	}
	my_timer.stop();
	cout<<"Read "<<mol_list.size()<< " structures and needed "<< my_timer.getSeconds()<< " seconds"<<endl;
	in_file.close();
	
	Log << "Writing:"<<endl;
	my_timer.reset();
	my_timer.start();
	
	SDFile outfile(parpars.get("o"), ios::out);
	for(list<AtomContainer*>::const_iterator it = mol_list.begin();
			it != mol_list.end(); ++it)
	{
		outfile << **it;
	}
	outfile.close();
	
	my_timer.stop();
	
	Log << "Needed "<<my_timer.getSeconds()<<" seconds for "<<mol_list.size()
			<< " files"<<endl<<endl;
	Log << "......done!"<<endl;
}

