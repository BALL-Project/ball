// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

/* SplitConnectedComponents.C
*  2011 Anna Dehof
*/

// ----------------------------------------------------
// $Maintainer: Anna Dehof $
// $Authors:    Anna Dehof $
// ----------------------------------------------------

// TODO: maintainer notwendig?
// TODO: ifdefs notwendig?

#include <BALL/FORMAT/MOL2File.h>
#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
#include <BALL/KERNEL/system.h>

#include <BALL/FORMAT/commandlineParser.h>
#include <iostream>

using namespace std;
using namespace BALL;

// TODO notwendig?
void usage (const char *progname);

int main (int argc, char **argv)
{
	// instantiate CommandlineParser object supplying
	// - tool name
	// - short description
	// - version string
	// - build date
	// - category
	CommandlineParser parpars("Split2ConnectedComponents", "splits a molecular file into its connected components", "bla", String(__DATE__), "Preparation");

	// we register an input file parameter 
	// - CLI switch
	// - description
	// - Inputfile
	// - required
	parpars.registerParameter("i", "input mol2-file", INFILE, true);

	// we register an output file parameter 
	// - CLI switch
	// - description
	// - Outputfile
	// - required
	parpars.registerParameter("o", "output mol2-file name for first solution", STRING, true);
	parpars.registerParameter("o_id", "output id", STRING, true);
	parpars.registerParameter("o_dir", "output directory for 2nd to last solution", STRING, true);

	// register String parameter for supplying max number of solutions
	parpars.registerParameter("min_atoms", "ignore connected components with less than minimal number of atoms", INT, false, 4);

	String man = "This tool splits a given molecular file into its connected components.\n\nOptional parameters are the minimal number of atoms ('-min_atoms') a connected component should have to be stored.\n\nOutput of this tool is a number of mol2 files each containing one connected component.";
	parpars.setToolManual(man);

	// here we set the types of I/O files
	parpars.setSupportedFormats("i","mol2");
	//parpars.setSupportedFormats("o","mol2"); //TODO

	parpars.parse(argc, argv);

	// 
	MOL2File f0;
	f0.open(parpars.get("i"));
	System system;
	f0 >> system;

	// create the processor
	ConnectedComponentsProcessor ccp;
	ccp.splitIntoMolecules(system);

	int min_atoms = 0;

	// do we have to consider a minimal number of atoms?
	if (parpars.has("min_atoms"))
	{
		Log << "Used Options: Minimal number of atoms = " << String(min_atoms) << endl;
		min_atoms = parpars.get("min_atoms").toInt();
	}

	// counter for the created molecules
	int i = 0;

	// write all connected components into individual files
	for (MoleculeIterator m_it = system.beginMolecule(); +m_it; ++m_it)
	{
		if (m_it->countAtoms() > min_atoms)
		{
			String outfile_name = (i == 0) ? String(parpars.get("o"))
				:   String(parpars.get("o_dir")) + "/primary_"
					+ String(parpars.get("o_id"))  + "_solution" + String(i)
					+ "_visible_mol2";

			Log << "   Writing connected component " << String(i) << " as " << outfile_name << endl;

			//	GenericMolFile* outfile = MolFileFactory::open(outfile_name, ios::out);
			MOL2File outfile(outfile_name, ios::out);
			//TODO 
			//if (!outfile)	
			outfile << *m_it;
			outfile.close();
		}
	}

	Log << "done." << endl;


	return 0;
}

