// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

//  SplitConnectedComponents.C
//  A tool for splitting into connected components 
//

#include <BALL/FORMAT/MOL2File.h>
#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
#include <BALL/KERNEL/system.h>

#include <BALL/FORMAT/commandlineParser.h>
#include <iostream>

using namespace std;
using namespace BALL;

int main (int argc, char **argv)
{
	CommandlineParser parpars("Split2ConnectedComponents", "splits a molecular file into its connected components", "bla", String(__DATE__), "Preparation");

	parpars.registerParameter("i", "input mol2-file", INFILE, true);

	parpars.registerParameter("o", "output mol2-file name for first solution", OUTFILE, true);

	// parameters for galaxy for handling multiple output files
	parpars.registerParameter("o_id", "output id", GALAXY_OPT_OUTID, false, "$o.id", true);
	// need to be hidden in command line mode
	parpars.setParameterAsAdvanced("o_id");

	// parameters for galaxy for handling multiple output files
	parpars.registerParameter("o_dir", "output directory for 2nd to last solution", GALAXY_OPT_OUTDIR, false, "$__new_file_path__", true);
	// need to be hidden in command line mode
	parpars.setParameterAsAdvanced("o_dir");

	// register String parameter for supplying max number of solutions
	parpars.registerParameter("min_atoms", "ignore connected components with less than minimal number of atoms", INT, false, 4);

	String man = "This tool splits a given molecular file into its connected components.\n\nOptional parameters are the minimal number of atoms ('-min_atoms') a connected component should have to be stored.\n\nOutput of this tool is a number of mol2 files each containing one connected component.";
	parpars.setToolManual(man);

	// here we set the types of I/O files
	parpars.setSupportedFormats("i","mol2");
	parpars.setSupportedFormats("o","mol2");

	parpars.parse(argc, argv);

	// 
	MOL2File f0;
	f0.open(parpars.get("i"));
	System system;
	f0 >> system;

	// called as command line or e.g. via galaxy?
	bool is_cmd =    !parpars.has("env")
			          || ((parpars.has("env") && parpars.get("env")=="cmdline"));

	// create the processor
	ConnectedComponentsProcessor ccp;
	ccp.splitIntoMolecules(system);

	Size min_atoms = 0;

	// do we have to consider a minimal number of atoms?
	if (parpars.has("min_atoms"))
	{
		Log << "Used Options: Minimal number of atoms = " << String(min_atoms) << endl;
		min_atoms = parpars.get("min_atoms").toInt();
	}

	// counter for the created molecules
	int i = 0;

	// write all connected components into individual files
	for (MoleculeIterator m_it = system.beginMolecule(); +m_it; ++m_it, ++i)
	{
		if (m_it->countAtoms() > min_atoms)
		{
			// create the output name
			String outfile_name = String(parpars.get("o")) + "_solution_" + String(i) + ".mol2";

			if (parpars.has("o_dir"))
			{
				outfile_name =  String(parpars.get("o_dir")) + "/" + outfile_name;
			}

			// NOTE: Galaxy requires this strange naming convention 
			//       including the fact, that zero-th element has a different name
			if (!is_cmd)
			{
				outfile_name = (i == 0) ? String(parpars.get("o"))
				                        :   String(parpars.get("o_dir")) + "/primary_"
				                        + String(parpars.get("o_id"))  + "_solution" + String(i)
				                        + "_visible_mol2";
			}
			Log << "   Writing connected component " << String(i) << " as " << outfile_name << endl;

			//	GenericMolFile* outfile = MolFileFactory::open(outfile_name, ios::out);
			MOL2File outfile(outfile_name, ios::out);
			if (outfile.bad())
			{
				Log.error() << "cannot write file " << outfile_name << endl;
				return 2;
			}

			outfile << *m_it;
			outfile.close();
		}
	}

	Log << "done." << endl;

	return 0;
}

