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
#include <set>
#include <BALL/QSAR/ringPerceptionProcessor.h>

#include "sources/fragmenter.h"

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

	
	FragVec rigid, linker;
	ConnectList connections;
	
	MoleculeFragmenter mfrag;
	
	mfrag.setMolecule( *ball_mol );
	mfrag.getMoleculeFragments(rigid, linker, connections);
	
	int i = 1;
	for( Fragment*& f : rigid)
	{
		cout<<"Frag "<<i<<endl;
		cout<<LigBase::printMol(f)<<endl<<endl;
		f->setProperty("FragNum", i);
		++i;
	}

	rigid[1]->insert( * rigid[2] );
	rigid[0]->insert( * rigid[1] );
	
	i = 1;
	for( Fragment*& f : rigid)
	{
		cout<<"Frag "<<i<<" root:"<<((AtomContainer*) &f->getRoot())->getProperty("FragNum").getInt()<<endl;
		cout<<LigBase::printMol(f)<<endl<<endl;
		++i;
	}
	Log << "......done!"<<endl;
}

