// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "../sources/ioModule.h"
#include "../sources/fragmenter.h"
#include "../sources/canonicalizer.h"

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/lineBasedFile.h>

using namespace OpenBabel;
using namespace BALL;
using namespace std;


/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("libFragmenter", "cut a molecule along its rotable bonds, generating fragments", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	
	parpars.registerFlag("unique", "only output one fragment for each topology");

	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");

	String manual = "For creating a fragment library.";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
	// START of CODE#############################################################
	
	// open input and output file:
	SDFile in_file(parpars.get("i"), ios::in);
	SDFile outfile(parpars.get("o"), ios::out);
//	LineBasedFile outfile(parpars.get("o"), ios::out);
	
	ACVec fragments, dummy;
	ConnectList dummy2;
	Molecule* tmp;
	
	int molecule_cnt=0;
	int total_fragment_cnt=0;
	int unique_fragment_cnt=0;
	
	MoleculeFragmenter molfrag;
	Canonicalizer canoni;
	tmp = in_file.read();
	
	// For each molecule:
	while ( tmp )
	{
		// get all rigid fragments from molecule 'tmp'
		molfrag.setMolecule( *tmp );
		molfrag.getMoleculeFragments(fragments, dummy, dummy2);
		
		total_fragment_cnt += fragments.size();
		
		// for each fragment in the molecule:
		vector<AtomContainer*>::iterator fit;
		for(fit = fragments.begin(); fit!=fragments.end(); fit++)
		{
			// canonicalise the atomlist & set UCK key
			canoni.canonicalize( **fit );
			(**fit).setProperty("key", Matcher::getUCK( **fit) );
		}
		
		// write to out file:
		LigIO::writeMolVec( fragments, outfile );
		
		// increase iteration:
		delete tmp;
		molecule_cnt++;
		tmp = in_file.read();
	}
	
	in_file.close();
	outfile.close();
	Log << "read "<< molecule_cnt<<" input structures, giving a total of "
			<< total_fragment_cnt <<" fragments and "<< unique_fragment_cnt
			<< "unique fragments"<<endl;
	
	Log << " wrote fragments to: " << parpars.get("o") << endl;
}
