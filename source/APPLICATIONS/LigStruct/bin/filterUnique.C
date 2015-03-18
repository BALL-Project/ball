// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/STRUCTURE/UCK.h>
#include <BALL/QSAR/aromaticityProcessor.h>

#include "../sources/base.h"

using namespace BALL;
using namespace std;

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("Unique Molecules", " filter a collection to contain only unique structures", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	
	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setOutputFormatSource("o","i");

	String manual = "...just playing...";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
/// C O D E ##################################
 
	Log << "Reading molecules..."<<endl;
	SDFile in_file(parpars.get("i"), ios::in);
	SDFile outfile(parpars.get("o"), ios::out);
	
	HashSet<String> unique_ids;
	
	/// filter to unique molecules only:
	AtomContainer* mol = in_file.read();
	
	int cnt = 0;
	while( mol )
	{
		// some user info every 1000 molecules:
		if( cnt % 1000 == 0)
		{
			cout << "\r" << flush;
			cout << "     filtered: "<< cnt<<" structures to "<<unique_ids.size()<<" uniques ";
		}
		cnt++;
		
		// assign canonical aromatic labels:
		AromaticityProcessor ar_pr;
		mol->apply(ar_pr);
		
		// make sure the molecule is leagl/valid
		if( !LigBase::containsUnknownElement( *mol ) && !LigBase::containsHydrogen( *mol))
		{
			UCK key_gen( *mol, true, 5 ); //generate canonical key
			
			String key = key_gen.getUCK(); //check if the key is new
			cout<<"Key is: "<<key<<endl;
			if( !unique_ids.has( key ))
			{
				unique_ids.insert( key_gen.getUCK());
				
				outfile << *mol;
			}
		}
		else
		{
			cout<<"found illegal molecule (unknown element or hydrogens found): "<<endl;
			cout<< LigBase::printInlineMol( mol)<<endl;
			cout<< LigBase::moleculeToSMILES( *mol) <<endl;
		}
			
		// read the next molecule
		mol = in_file.read();
	}
	
	
	in_file.close();
	outfile.close();

	Log << "......done!"<<endl;
}
