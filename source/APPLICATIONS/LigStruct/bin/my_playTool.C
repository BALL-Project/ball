// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/SYSTEM/file.h>

#include <BALL/KERNEL/forEach.h>
#include <BALL/DATATYPE/string.h>

#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/STRUCTURE/UCK.h>

#include <vector>
#include <BALL/COMMON/hash.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>

#include "../sources/fragmenter.h"
#include "../sources/moleculeConnector.h"
#include "../sources/canonicalizer.h"
#include "../sources/linkerBuilder.h"
#include "../sources/ioModule.h"

#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/MATHS/quaternion.h>
#include <BALL/STRUCTURE/RMSDMinimizer.h>

#include <BALL/STRUCTURE/geometricTransformations.h>
//using namespace OpenBabel;
using namespace BALL;
using namespace std;

static bool compareMolSize(AtomContainer*& ac1, AtomContainer*& ac2)
{
	return ac1->countAtoms() < ac2->countAtoms();
}

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("queryFragments", " generate query fragments and connections", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	
//	parpars.registerParameter("c", "location of conf file", INFILE, false);
//	parpars.setSupportedFormats("c","conf");
	
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
	
	// filter to unique molecules only:
	AtomContainer* mol = in_file.read();
	int cnt = 0;
	while( mol )
	{
		// some user info every 1000 molecules:
		if( cnt % 1000 == 0)
		{
			cout << "\r" << flush;
			cout << "     fragmented: "<< cnt<<" structures to "<<unique_ids.size()<<" uniques ";
		}
		cnt++;
		if( !LigBase::containsUnknownElement( *mol ) )
		{
			UCK key_gen( *mol, true, 5 );
			
			String key = key_gen.getUCK();
			if( !unique_ids.has( key ))
			{
				unique_ids.insert( key_gen.getUCK());
				
				outfile << *mol;
			}
		}
		else
		{
			cout<<"found illegal molecule: "<<endl;
			cout<< LigBase::printInlineMol( mol)<<endl;
			cout<< LigBase::moleculeToSMILES( *mol) <<endl;
		}
		
		mol = in_file.read();
	}
	in_file.close();
	outfile.close();

	Log << "......done!"<<endl;
}

