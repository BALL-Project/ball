// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// A tool for computing rmsd between proteins
//
//

#include <BALL/DOCKING/COMMON/poseClustering.h>
#include <BALL/FORMAT/DCDFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/SYSTEM/timer.h>
#include <iostream>
#include "version.h"

using namespace std;
using namespace BALL;

int main (int argc, char **argv)
{
	Timer t;

	// instantiate CommandlineParser object supplying
	// - tool name
	// - short description
	// - version string
	// - build date
	// - category
	CommandlineParser parpars("PDBRMSDCalculator", "computes RMSD between protein poses ", VERSION, String(__DATE__), "Docking");

	// we register an input file parameter 
	// - CLI switch
	// - description
	// - Inputfile
	parpars.registerMandatoryInputFile("i_pdb", "input pdb-file");
	parpars.registerMandatoryInputFile("i_query", "molecule(s) to compare input file");

	parpars.registerOptionalStringParameter("i_type", "query type (pdb, dcd, or transformation file (rigid_transformations) ", "pdb");
	list<String> input_types;
	input_types.push_back("pdb");
	input_types.push_back("dcd");
	input_types.push_back("rigid_transformations");
	parpars.setParameterRestrictions("i_type", input_types);

	parpars.registerOptionalOutputFile("o", "output file name");
	parpars.setParameterAsHidden("o");

	// choice of atom rmsd scope 
	parpars.registerOptionalStringParameter("scope", "atoms to be considered for scoreing a pose (C_ALPHA, BACKBONE, ALL_ATOMS) ", "C_ALPHA");
	list<String> rmsd_levels;
	rmsd_levels.push_back("C_ALPHA");
	//rmsd_levels.push_back("HEAVY_ATOMS"); //TODO
	rmsd_levels.push_back("BACKBONE");
	rmsd_levels.push_back("ALL_ATOMS");
	parpars.setParameterRestrictions("scope", rmsd_levels);

	// choice of rmsd type
	parpars.registerOptionalStringParameter("rmsd_type", "rmsd type used for clustering (SNAPSHOT_RMSD, RIGID_RMSD, CENTER_OF_MASS_DISTANCE) ", "SNAPSHOT_RMSD");
	list<String> rmsd_types;
	rmsd_types.push_back("SNAPSHOT_RMSD");
	rmsd_types.push_back("RIGID_RMSD");
	rmsd_types.push_back("CENTER_OF_MASS_DISTANCE");
	parpars.setParameterRestrictions("rmsd_type", rmsd_types);

	// TODO: parameter for preceding determination of RMSD minimizing transformation?
	//parpars.registerFlag("find_transformation", "");


	// the manual
	String man = "This tool computes the RMSD between proteins.\n\nParameters are either a second input file (i_query) who's type has to be specified (i_type) and can be either a single pdb, a dcd or a transformation file. Optional parameters are the rmsd type (-rmsd_type), and the type of atoms used for scoring a pose (-scope).\n\nOutput of this tool is a either the rmsd (in the pdb-pdb case) or a file (-o) containing the RMSD between the first pose and all other poses.";

	parpars.setToolManual(man);

	// here we set the types of I/O files
	parpars.setSupportedFormats("i_pdb","pdb");
	parpars.setSupportedFormats("i_query","pdb,dcd,txt");
	parpars.setSupportedFormats("o","txt");

	parpars.parse(argc, argv);

	//////////////////////////////////////////////////

	// read the input	
	PDBFile pdb;
	pdb.open(parpars.get("i_pdb"));
	System sys;
	pdb.read(sys);

	// read the second file	
	if (parpars.has("i_type") && parpars.has("i_query"))
	{
		String query_type = parpars.get("i_type");
		String second_file = parpars.get("i_query");

		ConformationSet cs;
		cs.setup(sys);

		PoseClustering pc;
		String type = "";
		if (parpars.has("rmsd_type"))
		{
			type = parpars.get("rmsd_type");
		}
		else
		{
			Log.info() << "Missing parameter rmsd_type! Abort!" << endl;
			return 1;
		}

		if (type == "SNAPSHOT_RMSD")
		{
			pc.options.set(PoseClustering::Option::RMSD_TYPE, PoseClustering::SNAPSHOT_RMSD);
		}
		else if (type == "RIGID_RMSD")
		{
			pc.options.set(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);
		}
		else if (type == "CENTER_OF_MASS_DISTANCE")
		{
				pc.options.set(PoseClustering::Option::RMSD_TYPE, PoseClustering::CENTER_OF_MASS_DISTANCE);
				Log << "Parameter scope will be ignored!" << endl;
		}

		if (parpars.has("scope"))
		{
			String scope = parpars.get("scope");
			if (scope == "C_ALPHA")
				pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::C_ALPHA);
			else if (scope == "BACKBONE")
				pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::BACKBONE);
			else if (scope == "ALL_ATOMS")
				pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::ALL_ATOMS);
			else
				Log.info() << "Unknown value " << scope  << " for option scope." << endl;
		}

		// we have basically two scenarios: pdb vs pdb or pdb vs list of poses (DCD or transformation).
		//   PDB
		if (query_type == "pdb")
		{
			PDBFile pdb2;
			pdb2.open(parpars.get("i_query"));
			System sys_query;
			pdb2.read(sys_query);

			Log << "RMSD: " << pc.getScore(sys, sys_query, pc.options) << std::endl;
			Log << "done." << endl;

			return 0;
		} // DCD
		else if (query_type == "dcd")
		{
			cs.readDCDFile(parpars.get("i_query"));
			cs.resetScoring();
			pc.setConformationSet(&cs, true);

			if (type == "RIGID_RMSD")
			{
				pc.convertSnaphots2Transformations();
			}

		} // rigid transformations
		else if (query_type == "rigid_transformations")
		{
			// reads the poses given as transformations from a file and update the covariance matrix !
			pc.setBaseSystemAndTransformations(sys, parpars.get("i_query"));

			if (type == "SNAPSHOT_RMSD")
			{
				pc.convertTransformations2Snaphots();
			}
		}
		else
		{
			Log << "Invalid query option! Abort!" << endl;
		}

		bool file_output = false;
		File rmsd_outfile;
		if (parpars.has("o"))
		{
			String outfile_name = String(parpars.get("o"));
			rmsd_outfile.open(outfile_name, std::ios::out);
			file_output = true;
		}

		// do the computations	
		if (type == "RIGID_RMSD")
		{
			// TODO need the Atommapping etc??
			Size num_poses = pc.getNumberOfPoses();

			Eigen::Matrix3f covariance_matrix = pc.computeCovarianceMatrix(sys, pc.options.getInteger(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL));
			std::vector<PoseClustering::RigidTransformation> const & rigid_transformations = pc.getRigidTransformations();

			PoseClustering::RigidTransformation const & transform_i = rigid_transformations[0];

			for (Size i=0; i<num_poses; i++)
			{
				float result=0;
	// just for testing...
	//for (Size j=0; j<500; j++)
	//{
				PoseClustering::RigidTransformation const & transform_j = rigid_transformations[i];

				t.start();
				result = pc.getRigidRMSD(transform_i.translation - transform_j.translation,
					                             transform_i.rotation - transform_j.rotation,
												               covariance_matrix);
				t.stop();
	//} 
			if (file_output)
				rmsd_outfile << "RMSD for " << i << " : " << result << endl;
			else
				Log << "RMSD for " << i << " : " << result << endl;
			}
		}
		else if (type ==  "SNAPSHOT_RMSD")
		{
			System system_i = sys;
			System system_j = sys;

			std::vector<SnapShot> const& snaps = pc.getConformationSet()->getUnscoredConformations();

			StructureMapper mapper(system_i, system_j);
			AtomBijection atom_bijection;

			Index rmsd_level_of_detail = pc.options.getInteger(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL);
			switch (rmsd_level_of_detail)
			{
				case PoseClustering::C_ALPHA:
					atom_bijection.assignCAlphaAtoms(system_i, system_j);
					break;
				case PoseClustering::BACKBONE:
					atom_bijection.assignBackboneAtoms(system_i, system_j);
					break;
				case PoseClustering::ALL_ATOMS:
					mapper.calculateDefaultBijection();
					atom_bijection = mapper.getBijection();
					break;
				case PoseClustering::PROPERTY_BASED_ATOM_BIJECTION:
					atom_bijection.assignAtomsByProperty(system_i, system_j);
					break;
				case PoseClustering::HEAVY_ATOMS:
				default:
					Log.info() << "Option RMSDLevelOfDetaill::HEAVY_ATOMS not yet implemented" << endl;
			}

			snaps[0].applySnapShot(system_i);

			Size num_poses = pc.getNumberOfPoses();

			for (Size i=0; i<num_poses; ++i)
			{
				float rmsd=0;
	// just for testing...
	//for (Size j=0; j<500; j++)
	//{
				snaps[i].applySnapShot(system_j);

				t.start();
				rmsd = mapper.calculateRMSD(atom_bijection);
				t.stop();
	//}
				if (file_output)
					rmsd_outfile << "RMSD for " << " " << i << " : " << rmsd << endl;
				else
					Log << "RMSD for " << " " << i << " : " << rmsd << endl;
			}
		}
		else if (type == "CENTER_OF_MASS_DISTANCE")
		{
			std::vector<Vector3> & com = pc.getCentersOfMass();

			Size num_poses = pc.getNumberOfPoses();

			// just query the center distance
			for (Size i=1; i<num_poses; ++i)
			{
				t.start();
				float rmsd = com[0].getDistance(com[i]);
				t.stop();

				if (file_output)
					rmsd_outfile << "RMSD for " << " " << i << " : " << rmsd << endl;
				else
					Log << "RMSD for " << i << ": " << rmsd << endl;
			}
		}

		if (file_output)
		{

			rmsd_outfile.close();
		}
	}
	else
	{
		Log << "Incorrect input! Abort!" << endl;
	}

	Log << "Done." << endl;
	//Log << "Done. (rmsd computation took " << t.getClockTime() << " seconds for " << sys.countAtoms() << " atoms)" << std::endl;

	return 0;
}
