// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "basic.h"
#include "matcher.h"
#include "fragmenter.h"
#include "assembler.h"

using namespace OpenBabel;
using namespace BALL;
using namespace std;

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	/// ------ S E T    P A R S E R -------
	CommandlineParser parpars("3D structure generation of ligands", " generate coordinates for a combiLib", 0.1, String(__DATE__), "Prediction");
	parpars.registerParameter("i", "combiLib as CONF file", INFILE, false);
	parpars.registerParameter("o", "output molecule with 3D coordinates SDF", OUTFILE, true);
	parpars.registerParameter("l", "library configuration file", INFILE, false);
	
	parpars.setSupportedFormats("i","conf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setSupportedFormats("l","conf");
	parpars.setOutputFormatSource("i","o");

	String manual = 
			"Generate a valid (not optimized) 3D structure for structures of an"
			"entire combinatorial library which is defined in a CONF file (either "
			"containing SIMLES for each group or giving the paths multi line SMILES "
			"files).";
	
	parpars.setToolManual(manual);
	parpars.parse(argc, argv);
	

/// L O A D    L I B s
	// Load all template libs:
	vector <String> libPathes(3); //0=fragments, 1=bondlenths, 2=connections
	if ( parpars.has("l") )
	{
		getLibraryPathes(libPathes, parpars.get("l"));
	}
	else
	{
		getLibraryPathes(libPathes, "/Users/pbrach/files/projects/Master-2014_2015/1_code/ball_ligandStruct/source/APPLICATIONS/LigStruct/examples/libraries.conf");
	}
	Log<<"Configuration is:"<<endl;
	Log<<"FRAGMENTS are in:"<<endl<<libPathes[0]<<endl;
	Log<<"BONDS are in:"<<endl<<libPathes[1]<<endl;
	Log<<"CONNECTIONS are in:"<<endl<<libPathes[2]<<endl;
	
	boost::unordered_map <String, Molecule*> fragmentLib;
	boost::unordered_map <String, float > bondLib;
	boost::unordered_map <String, Molecule* > connectLib;

	boost::unordered_map <String, TemplateCoord*> newFragmentLib;
	Log << "loading template libs...";
	readNewFragmentLib(libPathes[0], newFragmentLib);
	readBondLib(libPathes[1], bondLib);
	readConnectionLib(libPathes[2], connectLib);
	Log <<"done!"<<endl<<endl;
	
	Log << "Hit enter to exit"<<endl;
	int a;
	cin >>a;
	
//	readNewFragmentLib(String("/Users/pbrach/OUT.txt"), newFragmentLib);
	
//	boost::unordered_map <String, TemplateCoord*>::iterator ito = newFragmentLib.begin();
//	Log<< " FOund "<< newFragmentLib.size() << " templates"<<endl;
//	for (; ito != newFragmentLib.end(); ito++)
//	{
//		Log<<endl;
//		Log << ito->first<<endl;
//		TemplateCoord* tmp = ito->second;
//		for (int i = 0; i< tmp->size(); i++)
//		{
//			Log <<i <<" "<< (*tmp)[i] <<endl;
//		}
//	}

	
	exit(0);
/// F R A G M E N T I N G
	Log << "Reading query molecule..."<<endl;
	OBMol ob_mol; // input query molecule
	Molecule* ball_mol = 0;
	readOBMolecule(parpars.get("i"), ob_mol);
	ball_mol = MolecularSimilarity::createMolecule(ob_mol, true);// convert from Babel to BALL molecule
	Log << "......done!"<<endl<<endl;
	
	vector <Molecule*> rigid_fragments;
	vector <Molecule*> linker_fragments;
	list< pair<Atom*, Atom*> > connections;
	
	Log << "Fragmenting the input molecule..."<<endl;
	assignFragments(ob_mol, *ball_mol, rigid_fragments, linker_fragments, connections);
	Log << "......done!"<<endl<<endl;
	
	
/// M A T C H I N G
	Log << "Matching fragments against FragmentLib..."<<endl;
	canonicalize(rigid_fragments);
	canonicalize(linker_fragments);
	
	matchRigidFragments(fragmentLib, rigid_fragments);
	Log << "......done!"<<endl<<endl;
	
	
///// A S S E M B L E   3 D
	if( ball_mol->countAtoms() > 0 )
	{
		Log << "ERROR: not all atoms were correctly fragmented!!!"<< endl;
		exit(EXIT_FAILURE);
	}
	Log << "Connecting 3D fragments..."<<endl;
	connectFragments(ball_mol, connections, connectLib, bondLib);
	Log << "......done!"<<endl<<endl;
	
	// memory clean up (the fragment entries should now all be empty)
	checkAndDeleteFragments(rigid_fragments);
	checkAndDeleteFragments(linker_fragments);
	rigid_fragments.clear();
	linker_fragments.clear();
	
	
/// F I N I S H
/// write output
	String outfile_name = String(parpars.get("o"));
	SDFile outfile(outfile_name, ios::out);
	outfile << *ball_mol;
	Log << "wrote structure to file: "<< endl << outfile_name << endl<<endl;
	
/// Clean up
	// clean up:
	delete ball_mol;
	outfile.close();
	
	// clean db
	Log << "Removing loaded libs from memory..."<<endl;
	boost::unordered_map <BALL::String, Molecule*>::iterator map_it;
	for(map_it = fragmentLib.begin(); map_it!=fragmentLib.end(); map_it++)
		delete map_it->second;
	
	// bonds are of primitive type and on the stack
	
	for(map_it = connectLib.begin(); map_it != connectLib.end(); map_it++)
		delete map_it->second;
	
	Log << "......done!"<<endl;
}

