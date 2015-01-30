// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "../sources/ioModule.h"
#include "../sources/fragmenter.h"
#include "../sources/rmsdBinner.h"

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/DATATYPE/hashSet.h>

#include <boost/unordered_map.hpp>

using namespace BALL;
using namespace std;

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("libFragmenter", "cut a AtomContainer along its rotable bonds, generating fragments", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF and bond length file", OUTFILE, true);
	
	parpars.registerFlag("unique", "only output one fragment for each topology");

	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");

	String manual = "For creating a connection library.";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
//## START of TOOL #############################################################
	
	// open in- and output files:
	SDFile infile(parpars.get("i"), ios::in);
	SDFile outfile(parpars.get("o"), ios::out);
	LineBasedFile bondFile(parpars.get("o")+"_bondlen.sdf", ios::out);
	
	MoleculeFragmenter mol_fragger;
	RMSDBinner binner(0.3, 100);
	
	int cnt = 0;
	boost::unordered_map <String, pair<float, int> > lengths;
	vector<pair<String, AtomContainer *> > temp_sites;
	
	// Read all AtomContainers.
	AtomContainer* tmp_mol = infile.read();
	while ( tmp_mol )
	{
		// fragment to connection sites:
		mol_fragger.setMolecule( *tmp_mol );
		mol_fragger.fragmentToSites(lengths, temp_sites);
		
		vector<pair<String, AtomContainer *> >::iterator ita;
		for(ita = temp_sites.begin(); ita != temp_sites.end(); ++ita)
		{
			binner.addMolecule( ita->first, *ita->second );
		}
		
		// prepare next iteration
		delete tmp_mol;
		tmp_mol = infile.read();
		cnt++;
	}
	
	// write out the filtered sites for this molecule:
	map <String, vector< pair<AtomContainer*, int> > >::iterator it;
	for(it = binner.begin(); it != binner.end(); ++it)
	{
		String key = it->first;
		AtomContainer* ac = it->second[0].first;
		ac->setProperty("key", key);
		
		LigIO::writeMol(*ac, outfile);
	}
	outfile.close();
	Log << "read "<< cnt<<" input structures, wrote resulting fragments"<< endl;
	
	// print results of standard lengths calculation:
	boost::unordered_map <String, pair<float, int> >::iterator mit;
	for(mit = lengths.begin(); mit != lengths.end(); mit++)
	{
		bondFile <<mit->first<<" "<< mit->second.first<<endl ;
	}
	bondFile.close();
}











