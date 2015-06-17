// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include  <BALL/STRUCTURE/LIGAND3DGEN/ioModule.h>
#include  <BALL/STRUCTURE/fragmenter.h>
#include  <BALL/STRUCTURE/rmsdBinner.h>

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/DATATYPE/hashSet.h>
#include <BALL/QSAR/aromaticityProcessor.h>

#include <boost/unordered_map.hpp>

using namespace BALL;
using namespace std;

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("Fragment to sites", " create site templates", 0.5, String(__DATE__), "Preparation");
	parpars.registerMandatoryInputFile("i", "input SDF");
	parpars.registerMandatoryOutputFile("o", "output SDF with site templates");

	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");

	String manual = "For creating a connection library.";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
//## START of TOOL #############################################################
	
	// open in- and output files:
	SDFile infile(parpars.get("i"), ios::in);
	SDFile outfile(parpars.get("o"), ios::out);
	
	MoleculeFragmenter mol_fragger;
	RMSDBinner binner(true, 0.3, 100);
	AromaticityProcessor arproc;
	
	int cnt = 0;
	int total_sites = 0;
	vector<pair<String, AtomContainer *> > temp_sites;
	
	// Read all AtomContainers.
	AtomContainer* tmp_mol = infile.read();
	while ( tmp_mol )
	{
		// some user info every 1000 molecules:
		if( cnt % 1000 == 0)
		{
			cout << "\r" << flush;
			cout << "     fragmented: "<< cnt<<" structures to "<<total_sites<<" total sites, unique sites: "<<binner.size();
		}
		
		// recognize aromatic bonds:
		tmp_mol->apply(arproc);
		
		// fragment to connection sites:
		mol_fragger.setMolecule( *tmp_mol );
		mol_fragger.fragmentToSites(temp_sites, false);
		
		total_sites += temp_sites.size();
		
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
	cout << "\r" << flush;
	cout << "                                                                        " << endl;
	infile.close();
	
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
	
	Log << "fragmented "<< cnt<<" structures to "<<total_sites<<" sites, wrote "<<binner.size()<<" unique sites"<<endl;
}











