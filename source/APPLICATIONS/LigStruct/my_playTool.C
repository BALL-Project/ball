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

#include "sources/fragmenter.h"
#include "sources/moleculeConnector.h"
#include "sources/clashResolver.h"

#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/MATHS/quaternion.h>
using namespace OpenBabel;
using namespace BALL;
using namespace std;



/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("queryFragments", " generate query fragments and connections", 0.1, String(__DATE__), "Preparation");
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
	Molecule* large = in_file.read();
	Molecule* small = in_file.read();

	Atom* atm1 = large->getAtom(8);
	Atom* atm2 = small->getAtom(8);
	if(atm1->getElement().getSymbol() != "N")
	{
		cout<<"got wrong one!"<<endl;
		exit(EXIT_FAILURE);
	}
	
	// determine a connection-list:
	MoleculeFragmenter molfrag;
	ConnectList connections;
	Atom::BondIterator bit;
	AtomIterator ait;
	
	molfrag.setMolecule(*large);
	BALL_FOREACH_BOND(*large, ait, bit)
	{
		if( molfrag.isBridgingBond(*bit) )
			connections.push_back( make_pair( bit->getFirstAtom(), bit->getSecondAtom()));
	}
	
	molfrag.setMolecule(*small);
	BALL_FOREACH_BOND(*small, ait, bit)
	{
		if( molfrag.isBridgingBond(*bit) )
			connections.push_back( make_pair( bit->getFirstAtom(), bit->getSecondAtom()));
	}
	
	ClashResolver cresov;
	cresov.setMolecule(*atm1,*atm2, connections);
	cout<<"Detecting... "<<cresov.detect()<<endl;
	cout<<"Resolving..."<<endl;
	cresov.resolve();
	
	outfile<< *large;
	outfile<< *small;
	Log << "......done!"<<endl;
}

