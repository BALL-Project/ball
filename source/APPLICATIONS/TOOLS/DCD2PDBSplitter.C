// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// A very simple utility for splitting a dcd file into 
// several pdb snapshots of a system
// 

#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/DCDFile.h>
#include <BALL/MOLMEC/COMMON/snapShot.h>

#include <BALL/FORMAT/commandlineParser.h>
#include <iostream>
#include "version.h"

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	CommandlineParser parpars("DCD2PDBSplitter", "splits SnapShots into PDB files ", VERSION, String(__DATE__), "Convert, combine and store");

	parpars.registerParameter("i_dcd", "input dcd-file", INFILE, true);
	parpars.registerParameter("i_pdb", "input pdb-file", INFILE, true);

	parpars.registerParameter("o", "output pdb-file name for first solution", STRING, true, "", true);

	parpars.registerParameter("o_id", "output id", STRING, true, "", true);

	parpars.registerParameter("o_dir", "output directory for 2nd to last pdb file", STRING, true, "", true);


	// the manual
	String man = "This tool splits SnapShots of a given DCDFile and the reference PDBFile into separate PDBFiles.\n\nParameters are the input SnapShots as DCDFile (-i_dcd), the corresponding reference pdb file that was originally used to create the DCDFile (-i_pdb) and a naming schema for the results (-o).\n\nOutput of this tool is a number of PDBFiles each containing one SnapShot.";

	parpars.setToolManual(man);

	// here we set the types of I/O files, for example sdf is also allowed
	parpars.setSupportedFormats("i_dcd","dcd");
	parpars.setSupportedFormats("i_pdb","pdb");
	parpars.setSupportedFormats("o","pdb");

	//TODO
	//parpars.setSupportedFormats("o",MolFileFactory::getSupportedFormats());

	parpars.parse(argc, argv);

	//////////////////////////////////////////////////

	// read the input	
	PDBFile pdb;
	pdb.open(parpars.get("i_pdb"));
	System sys;
	pdb.read(sys);

	SnapShot ss;
	DCDFile dcd(parpars.get("i_dcd"));
	Size num_ss = dcd.getNumberOfSnapShots();

	for (Size i=0; i< num_ss; i++)
	{
		String outfile_name = (i == 0) ? String(parpars.get("o"))
				                               :   String(parpars.get("o_dir")) + "/primary_"
				                                 + String(parpars.get("o_id"))  + "_snapshot" + String(i)
				                                 + "_visible_pdb";

		Log << "   write SnapShot " << i << " as " << outfile_name << endl;

		if (dcd.read(ss))
		{
			System sys_temp(sys);
			ss.applySnapShot(sys_temp);

			PDBFile file(outfile_name, ios::out);
			if (file.bad())
			{
				Log.error() << "cannot write PDB file " << outfile_name << endl;
				return 2;
			}
			file << sys_temp;
			file.close();
		}
	}

	Log << "done." << endl;

	return 0;
}
