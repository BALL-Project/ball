// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// A utility for converting Pose indices of a clustering run 
// into pdb files 
//

#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/lineBasedFile.h>

#include <BALL/DOCKING/COMMON/poseClustering.h>

#include <BALL/FORMAT/commandlineParser.h>
#include <iostream>
#include "version.h"

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	CommandlineParser parpars("PoseIndices2PDB", "converts pose indices into PDB files ", VERSION, String(__DATE__), "Convert, combine and store");

	parpars.registerMandatoryInputFile("i_clust", "input cluster index file");
	parpars.registerMandatoryInputFile("i_trans", "input tranformation file");
	parpars.registerMandatoryInputFile("i_pdb",   "input reference pdb file");

	parpars.registerMandatoryOutputFile("o", "output file name prefix for resulting pdb files");
	parpars.setParameterAsHidden("o");

	// parameters for galaxy for handling multiple output files
	parpars.registerOptionalGalaxyOutputId("o_id", "output file name prefix for 2nd to last pdb file", "$o.id");
	// need to be hidden in command line mode
	parpars.setParameterAsHidden("o_id");
	parpars.setParameterAsAdvanced("o_id");

	// parameters for galaxy for handling multiple output files
	parpars.registerOptionalGalaxyOutputFolder("o_dir", "output directory for 2nd to last pdb file", "$__new_file_path__");
	// need to be hidden in command line mode
	parpars.setParameterAsHidden("o_dir");
	parpars.setParameterAsAdvanced("o_dir");

	// the manual
	String man = "This tool converts all pose indices from a given transformation file and the corresponding reference PDBFile into separate PDBFiles.\n\nParameters are the input pose index file (-i_clust), the original transformation file (-i_trans), the corresponding reference pdb file (-i_pdb) and a naming schema for the resulting pdb files (-o). \n\nOutput of this tool is a set of PDBFiles representing the docking poses belonging to the given input cluster.";

	parpars.setToolManual(man);

	// here we set the types of I/O files
	parpars.setSupportedFormats("i_clust","txt");
	parpars.setSupportedFormats("i_trans","dcd");
	parpars.setSupportedFormats("i_pdb","pdb");
	parpars.setSupportedFormats("o","pdb");

	parpars.parse(argc, argv);

	//////////////////////////////////////////////////

	// read the input		
	PDBFile pdb;
	pdb.open(parpars.get("i_pdb"));
	System sys;
	pdb.read(sys);

	PoseClustering pc;

	if (parpars.has("i_trans"))
	{
		pc.options.set(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);
		pc.setBaseSystemAndTransformations(sys, parpars.get("i_trans"));
	}

	//std::vector< std::set<Index> > clusters;

	LineBasedFile file(parpars.get("i_clust"), std::ios::in);
	vector<String> fields;

	String cluster_id = -1;
	String pose_id    = -1;

	// called as command line or e.g. via galaxy?
	bool is_cmd =    !parpars.has("env")
			          || ((parpars.has("env") && parpars.get("env")=="cmdline"));
	bool first_sol = true;

	while (file.LineBasedFile::readLine())
	{
		// get the line
		String current_cluster = file.getLine();
		if (current_cluster.getField(1) == "cluster")
		{
			cluster_id = current_cluster.getField(2);
			pose_id = -1;

			if (file.LineBasedFile::readLine())
			{
				current_cluster = file.getLine();
				fields.clear();
				current_cluster.split(fields);
				for (Size i=0; i < fields.size(); i++)
				{
					System new_pose_sys(sys);

					pose_id = fields[i];
					pc.applyTransformation2System(pose_id.toInt(), new_pose_sys);

					// create the output name
					String outfile_name = String(parpars.get("o"))
																+ "_clust_" + cluster_id
																+ "_pose_" + String(pose_id) + ".pdb";

					if (parpars.has("o_dir") && is_cmd && (parpars.get("o_dir") != "$__new_file_path__"))
					{
						outfile_name =  String(parpars.get("o_dir")) + "/" + outfile_name;
					}

					// NOTE: Galaxy requires this strange naming convention 
					//       including the fact, that zero-th element has a different name
					if (!is_cmd)
					{
						outfile_name = (first_sol) ? String(parpars.get("o"))
				                               :   String(parpars.get("o_dir")) + "/primary_"
				                                 + String(parpars.get("o_id"))  + "_clust_" + cluster_id
																				 + "_pose_" + String(pose_id)
				                                 + "_visible_pdb";
					}
					PDBFile file(outfile_name, ios::out);

					if (file.bad())
					{
						Log.error() << "cannot write file " << outfile_name << endl;
						return 2;
					}
					file << new_pose_sys;
					file.close();

					// needed for galaxy output
					if (first_sol)
						first_sol = false;

					Log << "wrote file " << outfile_name << endl;
				}
			}
		}
	}

	Log << "done." << endl;

	return 0;
}
