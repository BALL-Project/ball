// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/DOCKING/COMMON/poseClustering.h>
#include <BALL/FORMAT/DCDFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/DOCKING/COMMON/conformationSet.h>

#include <BALL/FORMAT/commandlineParser.h>
#include <iostream>
#include "version.h"

using namespace std;
using namespace BALL;

int main (int argc, char **argv)
{
	// instantiate CommandlineParser object supplying
	// - tool name
	// - short description
	// - version string
	// - build date
	// - category
	CommandlineParser parpars("DockPoseClustering", "clusters docking poses ", VERSION, String(__DATE__), "Docking");

	// we register an input file parameter 
	// - CLI switch
	// - description
	// - Inputfile
	parpars.registerParameter("i_pdb", "input pdb-file", INFILE, true);
	parpars.registerParameter("i_dcd", "input dcd-file or", INFILE, false);
	parpars.registerParameter("i_transformations", "input transformation file for rigid rmsd clustering", INFILE, false);

	// we register an output file parameter 
	// - description
	// - Outputfile
	// - required
	parpars.registerParameter("o", "output dcd-file name for first solution ", STRING, true, "", true);

	parpars.registerParameter("o_id", "output id ", STRING, true, "", true);

	parpars.registerParameter("o_dir", "output directory for 2nd to last solution ", STRING, true, "", true);

	parpars.registerParameter("o_dcd", "output directory for the reduced cluster set (one structure per final cluster) ", STRING, false, "", true);

	// register String parameter for supplying minimal rmsd between clusters
	parpars.registerParameter("rmsd_cutoff", "minimal rmsd between the final clusters (default 5.0) ", DOUBLE, false, 5.0);
	parpars.setParameterRestrictions("rmsd_cutoff", 0, 100);

	// choice of cluster algorithm  
	parpars.registerParameter("alg", "algorithm used for clustering (CLINK_DEFAYS, SLINK_SIBSON, TRIVIAL_COMPLETE_LINKAGE) ", STRING, false, "CLINK_DEFAYS");
	list<String> cluster_algs;
	cluster_algs.push_back("CLINK_DEFAYS");
	cluster_algs.push_back("SLINK_SIBSON");
	cluster_algs.push_back("TRIVIAL_COMPLETE_LINKAGE");
	parpars.setParameterRestrictions("alg", cluster_algs);

	// choice of atom rmsd scope 
	parpars.registerParameter("rmsd_scope", "atoms to be considered for rmsd score (C_ALPHA, BACKBONE, ALL_ATOMS) ", STRING, false, "C_ALPHA");
	list<String> rmsd_levels;
	rmsd_levels.push_back("C_ALPHA");
	//rmsd_levels.push_back("HEAVY_ATOMS"); //TODO
	rmsd_levels.push_back("BACKBONE");
	rmsd_levels.push_back("ALL_ATOMS");
	parpars.setParameterRestrictions("rmsd_scope", rmsd_levels);


	// choice of rmsd type
	parpars.registerParameter("rmsd_type", "rmsd type used for clustering (SNAPSHOT_RMSD, RIGID_RMSD, CENTER_OF_MASS_DISTANCE) ", STRING, false, "SNAPSHOT_RMSD");
	list<String> rmsd_types;
	rmsd_types.push_back("SNAPSHOT_RMSD");
	rmsd_types.push_back("RIGID_RMSD");
	rmsd_types.push_back("CENTER_OF_MASS_DISTANCE");
	parpars.setParameterRestrictions("rmsd_type", rmsd_types);

	// register bool parameter for using pre-clustering
	parpars.registerFlag("use_preclustering", "Switch on preclustering");

  // the manual
	String man = "This tool computes clusters of docking poses given as conformation set using the SLINK or CLINK algorithm.\n\nParameters are the input ConformationSet (-i_dcd), one corresponding pdb file (-i_pdb), the algorithm (-alg) and a naming schema for the results (-o). Optional parameters are (-alg), the minimal rmsd between the final clusters (-rmsd_cutoff) and the scope/level of detail of the rmsd computation (-rmsd_scope). The optional parameter -o_dcd set the output directory for the reduced cluster set.\n\nOutput of this tool is a number of dcd files each containing one ConformationSet.";

	parpars.setToolManual(man);

	// here we set the types of I/O files
	parpars.setSupportedFormats("i_dcd","dcd");
	parpars.setSupportedFormats("i_pdb","pdb");
	parpars.setSupportedFormats("i_transformations","txt");
	parpars.setSupportedFormats("o","dcd");
	parpars.setSupportedFormats("o_dcd","dcd");

	parpars.parse(argc, argv);

	//////////////////////////////////////////////////

	// read the input	
	PDBFile pdb;
	pdb.open(parpars.get("i_pdb"));
	System sys;
	pdb.read(sys);

	ConformationSet cs;
	cs.setup(sys);

	if (parpars.has("i_dcd"))
	{
		if (parpars.has("rmsd_type") && (parpars.get("rmsd_type") == "RIGID_RMSD"))
		{
			Log << "Trajectory input file cannot be used with rmsd_type 'RIGID_RMSD'. Abort!" << endl;
			//TODO Implement an automatic converstion from dcd to transformation!
			return 0;
		}

		cs.readDCDFile(parpars.get("i_dcd"));
	}

	cs.resetScoring();

	PoseClustering pc;

	if (parpars.has("i_transformations"))
	{
		if (parpars.has("rmsd_type"))
		{
			if (parpars.get("rmsd_type") != "RIGID_RMSD")
			{
				Log << "Transformation input file can only be used with rmsd_type 'RIGID_RMSD'. Abort!" << endl;
				return 0;
			}
		}
		else
		{
			pc.options.set(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);
		}
		pc.setBaseSystemAndTransformations(sys, parpars.get("i_transformations"));
	}

	if (parpars.has("rmsd_cutoff"))
	{
		float rmsd = parpars.get("rmsd_cutoff").toInt();
		pc.options.setReal(PoseClustering::Option::RMSD_THRESHOLD, rmsd);
	}

	if (parpars.has("rmsd_scope"))
	{
		String scope = parpars.get("rmsd_scope");
		if (scope == "C_ALPHA")
			pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::C_ALPHA);
		else if (scope == "BACKBONE")
			pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::BACKBONE);
		else if (scope == "ALL_ATOMS")
			pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::ALL_ATOMS);
		else
			Log.info() << "Unknown value " << scope  << " for option rmsd_scope." << endl;
	}

	if (parpars.has("alg"))
	{
		String alg = parpars.get("alg");
		if (alg == "CLINK_DEFAYS")
			pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::CLINK_DEFAYS);
		else if (alg == "SLINK_SIBSON")
			pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::SLINK_SIBSON);
		else if (alg == "TRIVIAL_COMPLETE_LINKAGE")
			pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
		else
			Log.info() << "Unknown value " << alg  << " for option alg." << endl;
	}

	if (parpars.has("rmsd_type"))
	{
		String type = parpars.get("rmsd_type");
		if (type == "SNAPSHOT_RMSD")
			pc.options.set(PoseClustering::Option::RMSD_TYPE, PoseClustering::SNAPSHOT_RMSD);
		else if (type == "RIGID_RMSD")
			pc.options.set(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);
		else if (type == "CENTER_OF_MASS_DISTANCE")
			pc.options.set(PoseClustering::Option::RMSD_TYPE, PoseClustering::CENTER_OF_MASS_DISTANCE);
		else
			Log.info() << "Unknown value " << type  << " for option rmsd_type." << endl;

	}

	if (parpars.has("use_preclustering"))
	{
		bool use_preclustering = parpars.get("use_preclustering").toBool();
		pc.options.setBool(PoseClustering::Option::USE_CENTER_OF_MASS_PRECLINK, use_preclustering);
	}


	pc.setConformationSet(&cs);

	pc.compute();

	Size num_clusters = pc.getNumberOfClusters();

	Log << "Computed " <<  num_clusters << " clusters, start writing..." << endl;

	for (Size i = 0; i < num_clusters; i++)
	{
		Log << "   Cluster " << i << " has " << pc.getClusterSize(i) << " members." << endl;

		if (parpars.has("rmsd_type") && parpars.get("rmsd_type") != "RIGID_RMSD")
		{
			boost::shared_ptr<ConformationSet> new_cs = pc.getClusterConformationSet(i);

			String outfile_name = (i == 0) ? String(parpars.get("o"))
				                             : String(parpars.get("o_dir")) + "/primary_"
			 	                              + String(parpars.get("o_id"))  + "_cluster" + String(i)
			 	                              + "_visible_dcd";
			//Log << "   Writing solution " << String(i) << " as " << outfile_name << endl;

			new_cs->writeDCDFile(outfile_name);
		}
	}

	// print
	pc.printClusterRMSDs();

	if (parpars.has("o_dcd"))
	{
		String outfile_name = String(parpars.get("o_dcd"));
		boost::shared_ptr<ConformationSet> cs = pc.getReducedConformationSet();
		cs->writeDCDFile(outfile_name);
	}

	Log << "done." << endl;

	return 0;
}

