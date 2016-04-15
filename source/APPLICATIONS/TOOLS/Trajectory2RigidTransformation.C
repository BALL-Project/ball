// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// A very simple utility for converting trajectory files like DCD
// into rigid transformations 
//

#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/DOCKING/COMMON/poseClustering.h>
#include <BALL/FORMAT/DCDFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/lineBasedFile.h>

#include <BALL/FORMAT/commandlineParser.h>
#include <iostream>
#include "version.h"

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	CommandlineParser parpars("Trajectory2RigidTransformation", "converts a trajectory file into a rigid transformation file ", VERSION, String(__DATE__), "Convert, combine and store");

	parpars.registerMandatoryInputFile("i_dcd", "input trajectory file");
	parpars.registerMandatoryInputFile("i_pdb",  "input pdb-file");

	parpars.registerMandatoryOutputFile("o", "output file for the rigid transformations");
	parpars.setParameterAsHidden("o");

	// the manual
	String man = "This tool converts SnapShots of a given TrajectoryFile and the reference PDBFile into a rigid transformation file.\n\nParameters are the input SnapShots as TrajectoryFile (-i_dcd), the corresponding reference pdb file that was originally used to create the TrajectoryFile (-i_pdb) and a naming schema for the resulting transformation file (-o).\n\nOutput of this tool is a file storing each given SnapShot as rigid transformation per line.";

	parpars.setToolManual(man);

	// here we set the types of I/O files, for example sdf is also allowed
	parpars.setSupportedFormats("i_dcd", "dcd");
	parpars.setSupportedFormats("i_pdb", "pdb");
	parpars.setSupportedFormats("o",    "txt");

	parpars.parse(argc, argv);

	//////////////////////////////////////////////////

	// read the input	
	PDBFile pdb;
	pdb.open(parpars.get("i_pdb"));
	System sys;
	pdb.read(sys);

	ConformationSet cs;
	cs.setup(sys);
	cs.readDCDFile(parpars.get("i_dcd"));
	cs.resetScoring();

	PoseClustering pc;
	pc.options.set(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);
	pc.setConformationSet(&cs);

	// now convert
	pc.convertSnaphots2Transformations();
	std::vector< PoseClustering::PosePointer > poses = pc.getPoses();

	String outfile_name = String(parpars.get("o"));
	LineBasedFile file(outfile_name, ios::out);
	if (file.bad())
	{
		Log.error() << "cannot write file " << outfile_name << endl;
		return 2;
	}

	Log << "write rigid poses into file " << outfile_name << endl;

	for (Size i=0; i<poses.size(); i++)
	{
		Eigen::Matrix3f const & rot    = poses[i].trafo->rotation;
		Eigen::Vector3f const & transl = poses[i].trafo->translation;
		file << "A A " << " 0.0 "
			   << rot(0) << " " << rot(1) << " " << rot(2) << " " << transl[0] << " "
				 << rot(3) << " " << rot(4) << " " << rot(5) << " " << transl[1] << " "
				 << rot(6) << " " << rot(7) << " " << rot(8) << " " << transl[2] << " "
				 << " 0.0 0.0 0.0 0.0"
				 << endl;
	}

	file.close();

	Log << "done." << endl;

	return 0;
}
