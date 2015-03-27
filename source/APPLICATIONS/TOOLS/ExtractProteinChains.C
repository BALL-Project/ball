// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/FORMAT/PDBFile.h>

#include <BALL/FORMAT/commandlineParser.h>

#include "version.h"

using namespace std;
using namespace BALL;

int main(int argc, char* argv[])
{
	// instantiate CommandlineParser object
	CommandlineParser parpars("ExtractProteinChains", "separate all chains of a pdb file into separate pdb files", VERSION, String(__DATE__), "Preparation");

	parpars.registerMandatoryInputFile("pdb",  "input pdb file ");
	parpars.registerOptionalStringParameter("chain_id",  "chain to extract ");
	//TODO
	//parpars.registerFlag("skip_nonaa", "skip all non aminoacids", false);

	parpars.registerMandatoryOutputFile("o", "output file name ");
	parpars.setParameterAsHidden("o");

	// parameters for galaxy for handling multiple output files
	parpars.registerOptionalGalaxyOutputId("o_id", "output id", "$o.id");
	// need to be hidden in command line mode
	parpars.setParameterAsAdvanced("o_id");
	parpars.setParameterAsHidden("o_id");

	// parameters for galaxy for handling multiple output files
	parpars.registerOptionalGalaxyOutputFolder("o_dir", "output directory for 2nd to last solution", "$__new_file_path__");
	// need to be hidden in command line mode
	parpars.setParameterAsAdvanced("o_dir");
	parpars.setParameterAsHidden("o_dir");

	// the manual
	String man = String("This tool splits a pdb file into its chains.");

	parpars.setToolManual(man);

	parpars.setSupportedFormats("pdb", "pdb");
	parpars.setSupportedFormats("o", "pdb");

	// parse the command line
	parpars.parse(argc, argv);


	PDBFile pdb;
	pdb.open(parpars.get("pdb"), std::ios::in);

	if (!pdb)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << parpars.get("pdb") << " for input." << std::endl;
		exit(2);
	}

	System sys;
	pdb >> sys;
	pdb.close();

	// called as command line or e.g. via galaxy?
	bool is_cmd =    !parpars.has("env")
		            || ((parpars.has("env") && parpars.get("env")=="cmdline"));

	// get the optional chain_id
	String chain_id = "";
	if (parpars.has("chain_id"))
	{
		chain_id = parpars.get("chain_id");
	}
/*	else if ( is_cmd && (parpars.get("o_dir") == "$__new_file_path__"))
	{
		Log.error() << endl << "unspecified parameter o_dir. Abort." << endl;
		return 2;
	}
*/

	int export_counter = 0;
	ChainConstIterator c_it = sys.beginChain();
	for (; +c_it ; ++c_it)
	{
		if ((chain_id == "") || (chain_id == c_it->getName()))
		{
			// create the output name
			String outfile_name = String(parpars.get("o")) + "chain_" + String(export_counter) + ".pdb";

			//TODO this is a temporary hack :-(
			// are we given a parameter o_dir
			if (parpars.has("o_dir") && is_cmd && (parpars.get("o_dir") != "$__new_file_path__"))
			{
				outfile_name = String(parpars.get("o_dir")) + "/" + c_it->getName() + "_" + outfile_name;
			}

			// NOTE: Galaxy requires this strange naming convention 
			//       including the fact, that zero-th element has a different name
			if (!is_cmd)
			{
				outfile_name = (export_counter == 0) ? String(parpars.get("o"))
			                               :   String(parpars.get("o_dir")) + "/primary_"
			                                 + String(parpars.get("o_id"))  + "_chain" + String(export_counter)
			                                 + "_visible_pdb";
			}
			// now write the chain
			PDBFile outfile(outfile_name, ios::out);

			if (outfile.bad())
			{
				Log.error() << endl << "cannot write file " << outfile_name << endl;
				return 2;
			}

			outfile << *c_it;
			outfile.close();

			Log << "wrote file " << outfile_name << endl;

			export_counter++;
		}
	}

	return 0;
}
