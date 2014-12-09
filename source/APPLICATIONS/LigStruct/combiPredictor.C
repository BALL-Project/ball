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
	
	parpars.setSupportedFormats("i","smi");
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
	Log<<"FRAGMENTS are in:"<<endl<<libPathes[0]<<endl;
	Log<<"BONDS are in:"<<endl<<libPathes[1]<<endl;
	Log<<"CONNECTIONS are in:"<<endl<<libPathes[2]<<endl<<endl;
	
//	boost::unordered_map <String, Molecule*> fragmentLib;
	boost::unordered_map <String, TemplateCoord*> lineFragmentLib;
	boost::unordered_map <String, float > bondLib;
	boost::unordered_map <String, Molecule* > connectLib;
	
	Log << "loading template libs..."<<endl;
//	readFragmentLib(libPathes[0], fragmentLib);
	readNewFragmentLib(libPathes[0], lineFragmentLib);
	readBondLib(libPathes[1], bondLib);
	readConnectionLib(libPathes[2], connectLib);
	Log <<" - done - "<<endl<<endl;
	
//	Log<<"num of read fragments: "<<fragmentLib.size()<<endl;

	CombiLib in_lib;
	readGroups(in_lib, "/Users/pbrach/files/projects/Master-2014_2015/1_code/ball_ligandStruct/source/APPLICATIONS/LigStruct/examples/example_combi1.conf");
	exit(EXIT_SUCCESS);
/// F R A G M E N T I N G

	Log << "Reading query molecule...";
	OBMol ob_mol; // input query molecule
	Molecule* ball_mol = 0;
	readOBMolecule(parpars.get("i"), ob_mol);
	ball_mol = MolecularSimilarity::createMolecule(ob_mol, true);// convert from Babel to BALL molecule
	Log << "......done!"<<endl<<endl;
	
	vector <Molecule*> rigid_fragments;
	vector <Molecule*> linker_fragments;
	list< pair<Atom*, Atom*> > connections;
	
	Log << "Fragmenting the input molecule...";
	assignFragments(ob_mol, *ball_mol, rigid_fragments, linker_fragments, connections);
	Log << "......done!"<<endl<<endl;
	
	
/// M A T C H I N G
	Log << "Matching fragments against FragmentLib...";
	canonicalize(rigid_fragments);
	canonicalize(linker_fragments);
	
//	matchRigidFragments(fragmentLib, rigid_fragments);
	matchRigidFragments(lineFragmentLib, rigid_fragments);
	Log << "......done!"<<endl<<endl;
	
	if( ball_mol->countAtoms() > 0 )
	{
		Log << "ERROR: not all atoms were correctly fragmented!!!"<< endl;
		exit(EXIT_FAILURE);
	}
	
	
///// A S S E M B L E   3 D
	Log << "Connecting 3D fragments...";
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
	Log << "Removing loaded libs from memory...";
	boost::unordered_map <BALL::String, TemplateCoord*>::iterator tmp_it;
	for(tmp_it = lineFragmentLib.begin(); tmp_it!=lineFragmentLib.end(); tmp_it++)
		delete tmp_it->second;
	
	// bonds are of primitive type and on the stack
	
	boost::unordered_map <BALL::String, Molecule*>::iterator mit;
	for(mit = connectLib.begin(); mit != connectLib.end(); mit++)
		delete mit->second;
	
	Log << "......done!"<<endl;
}

