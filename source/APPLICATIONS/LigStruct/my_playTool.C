// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/SYSTEM/file.h>

#include <BALL/KERNEL/forEach.h>
#include <BALL/DATATYPE/string.h>

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
#include <BALL/QSAR/ringPerceptionProcessor.h>

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
	

/// F R A G M E N T I N G
	Log << "Reading molecule, searching ring atoms..."<<endl;
	SDFile in_file(parpars.get("i"), ios::in);
	Molecule* ball_mol = in_file.read();

	RingPerceptionProcessor rpp;
	vector< vector < Atom* > > sssr;
	rpp.calculateSSSR(sssr, *ball_mol);
	Log << "......done!"<<endl<<endl;
	
	for(AtomIterator ati = ball_mol->beginAtom(); +ati; ++ati)
	{
		if( ati->hasProperty("InRing") )
		{
			cout<<ati->getElement().getSymbol()<< " ring atom"<<endl;
		}
		else
		{
			cout<<ati->getElement().getSymbol()<< " not in ring"<<endl;
		}
	}
	
	Log << "......done!"<<endl;
}

