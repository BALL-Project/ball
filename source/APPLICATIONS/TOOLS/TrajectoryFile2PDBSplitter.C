// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// A very simple utility for splitting a trajectory file into 
// several pdb snapshots of a system
// 

#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/trajectoryFile.h>
#include <BALL/FORMAT/trajectoryFileFactory.h>
#include <BALL/MOLMEC/COMMON/snapShot.h>

#include <BALL/FORMAT/commandlineParser.h>
#include <iostream>
#include "version.h"

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	CommandlineParser parpars("TrajectoryFile2PDBSplitter", "splits SnapShots into PDB files ", VERSION, String(__DATE__), "Convert, combine and store");

	parpars.registerMandatoryInputFile("i_traj", "input trajectory file");
	parpars.registerMandatoryInputFile("i_pdb", "input pdb-file");

	parpars.registerMandatoryOutputFile("o", "output pdb-file for first solution");
	parpars.setParameterAsHidden("o");

	// parameters for galaxy for handling multiple output files
	parpars.registerOptionalGalaxyOutputId("o_id", "output id", "$o.id");
	// need to be hidden in command line mode
	parpars.setParameterAsAdvanced("o_id");
	parpars.setParameterAsHidden("o_id");

	// parameters for galaxy for handling multiple output files
	parpars.registerOptionalGalaxyOutputFolder("o_dir", "output directory for 2nd to last pdb file", "$__new_file_path__");
	// need to be hidden in command line mode
	parpars.setParameterAsAdvanced("o_dir");
	parpars.setParameterAsHidden("o_dir");

	// the manual
	String man = "This tool splits SnapShots of a given TrajectoryFile and the reference PDBFile into separate PDBFiles.\n\nParameters are the input SnapShots as TrajectoryFile (-i_traj), the corresponding reference pdb file that was originally used to create the TrajectoryFile (-i_pdb) and a naming schema for the results (-o).\n\nOutput of this tool is a number of PDBFiles each containing one SnapShot.";

	parpars.setToolManual(man);

	// here we set the types of I/O files
	parpars.setSupportedFormats("i_traj", "dcd");
	parpars.setSupportedFormats("i_pdb",  "pdb");
	parpars.setSupportedFormats("o",      "pdb");

	parpars.parse(argc, argv);

	//////////////////////////////////////////////////

	// read the input	
	PDBFile pdb;
	pdb.open(parpars.get("i_pdb"));
	System sys;
	pdb.read(sys);

	SnapShot ss;
	TrajectoryFile *traj_file = TrajectoryFileFactory::open(parpars.get("i_traj"));
	Size num_ss = traj_file->getNumberOfSnapShots();

	// called as command line or e.g. via galaxy?
	bool is_cmd =    !parpars.has("env")
			          || ((parpars.has("env") && parpars.get("env")=="cmdline"));

	for (Size i=0; i< num_ss; i++)
	{
		// create the output name
		String outfile_name = String(parpars.get("o")) + "_snapshot_" + String(i) + ".pdb";
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
				                                 + String(parpars.get("o_id"))  + "_snapshot" + String(i)
				                                 + "_visible_pdb";
		}
		Log << "   write SnapShot " << i << " as " << outfile_name << endl;

		if (traj_file->read(ss))
		{
			System sys_temp(sys);
			ss.applySnapShot(sys_temp);

			PDBFile file(outfile_name, ios::out);
			if (file.bad())
			{
				Log.error() << "cannot write file " << outfile_name << endl;
				return 2;
			}
			file << sys_temp;
			file.close();
		}
		else
			break;
	}

	Log << "done." << endl;

	return 0;
}
