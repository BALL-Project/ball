// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "../sources/ioModule.h"
#include "../sources/fragmenter.h"
#include "../sources/canonicalizer.h"
#include "../sources/rmsdBinner.h"

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/QSAR/aromaticityProcessor.h>

using namespace OpenBabel;
using namespace BALL;
using namespace std;


/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("libFragmenter", "cut a molecule along its rotable bonds, generating fragments", 0.3, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	
	parpars.registerFlag("sdf", "output the lib in sdf AND lineBased format");

	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");

	String manual = "For creating a fragment library.";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
	// START of CODE#############################################################
	Log<<" * Reading file..."<<endl<<endl;
	
	// open input file:
	SDFile in_file(parpars.get("i"), ios::in);
	
	ACVec fragments, dummy;
	ConnectList dummy2;
	Molecule* tmp;
	
	int molecule_cnt=0;
	int total_fragment_cnt=0;
	
	MoleculeFragmenter molfrag;
	Canonicalizer canoni;
	RMSDBinner binner(false);
	AromaticityProcessor arproc;
	
	tmp = in_file.read();
	
	// For each molecule:
	Log<<" * fragmenting..."<<endl;
	while ( tmp )
	{
		// normalize the structures:
		LigBase::removeHydrogens( *tmp );
		tmp->apply(arproc);

		// some user info every 100 molecules:
		if( molecule_cnt % 1000 == 0)
		{
			cout << "\r" << flush;
			cout << "     fragmented: "<< molecule_cnt<<" structures"<<", unique fragments: "<<binner.size();
		}
		// get all rigid fragments from molecule 'tmp'
		molfrag.setMolecule( *tmp );
		molfrag.fragment(fragments, dummy, dummy2);
		
		total_fragment_cnt += fragments.size();
		
		// for each fragment in the molecule:
		vector<AtomContainer*>::iterator fit;
		for(fit = fragments.begin(); fit!=fragments.end(); fit++)
		{
			// canonicalise the atomlist & set UCK key
			canoni.canonicalize( **fit );
			
			String key = Matcher::getUCK( **fit);

			binner.addMolecule(key, **fit);
		}
	
		// increase iteration:
		delete tmp;
		molecule_cnt++;
		tmp = in_file.read();
	
	}
	cout << "\r" << flush;
	cout << "                                                                " << endl;
	in_file.close();
	
	Log <<endl<< " * read "<< molecule_cnt<<" input structures, giving a total of "
			<< total_fragment_cnt <<" fragments and "<< binner.size()
			<< " unique fragments"<<endl<<endl;

	
	// write to out file:
	LineBasedFile* outfile = 0;
	SDFile* out_sdf = 0;
	
	// open simple LineBased- or SDFile:
	bool use_sdf = parpars.has("sdf");
	if( use_sdf )
	{
		outfile = new LineBasedFile(parpars.get("o")+".line", ios::out);
		out_sdf = new SDFile(parpars.get("o")+".sdf", ios::out);
	}
	else
		outfile = new LineBasedFile(parpars.get("o"), ios::out);
	
	// write all 
	map <String, vector< pair<AtomContainer*, int> > >::iterator bin_it;
	for(bin_it = binner.begin(); bin_it != binner.end(); ++bin_it)
	{
		AtomContainer& frag = * bin_it->second[0].first;
		frag.setProperty("key", bin_it->first);
		
		if(use_sdf)
		{
			LigIO::writeMol(frag, * (LineBasedFile*)outfile);
			LigIO::writeMol(frag, * (SDFile*)out_sdf);
		}
		else
			LigIO::writeMol(frag, * (LineBasedFile*)outfile);
	}
	outfile->close();
	
	
	Log << " * wrote unique fragments to: " << parpars.get("o") << endl;
}
