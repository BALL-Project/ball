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
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/MATHS/angle.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/matrix44.h>


#include <vector>
#include <set>
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
	
	map<String, AtomContainer*> unique_mols;
	
	// filter to unique molecules only:
	AtomContainer* mol = in_file.read();
	Canonicalizer canon;
	while( mol )
	{
		if( !LigBase::containsUnknownElement( *mol ) && LigBase::containsHydrogen(*mol) )
		{
			
			LigBase::removeHydrogens( *mol );

			canon.canonicalize( *mol );
			
			UCK key_gen( *mol, true, 5 );
			
			unique_mols[ key_gen.getUCK() ] = mol;
		}
		mol = in_file.read();
	}
	in_file.close();
	Log << "Sorting molecules..."<<endl;
	
	// sort according to molecule size:
	vector<AtomContainer*> sorted_uniques;
	sorted_uniques.reserve( unique_mols.size() );
	
	Log << "Writing molecules..."<<endl;
	for(map<String, AtomContainer*>::iterator it = unique_mols.begin(); 
			it != unique_mols.end(); ++it)
	{
		sorted_uniques.push_back( it->second );
	}
	sort(sorted_uniques.begin(), sorted_uniques.end(), compareMolSize);
	
	// write out the uniques:
	for(vector<AtomContainer*>::iterator it = sorted_uniques.begin(); 
			it != sorted_uniques.end(); ++it)
	{
		outfile << **it;
	}
	outfile.close();

	Log << "......done!"<<endl;
}

