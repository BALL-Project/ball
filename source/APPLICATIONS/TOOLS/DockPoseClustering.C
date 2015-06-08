// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// A tool for clustering docking poses
//
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
	parpars.registerMandatoryInputFile("i_pdb", "input pdb-file");
	parpars.registerOptionalInputFile("i_dcd", "input dcd-file");
	///TODO: offer the alternatives in a more elegant way!
	parpars.registerOptionalInputFile("i_trans", "or input transformation file for rigid rmsd clustering ");

	// we register an output file parameter 
	// - CLI switch
	// - description	
	// - parameter type	
	// - required
	// - default value
	// - hidden in galaxy
	parpars.registerMandatoryOutputFile("o_index_list", "output file name for the index list ");
	parpars.setParameterAsHidden("o_index_list");
	parpars.registerOptionalOutputFile("o_score_matrix", "output file name for scoring matrix ");
	parpars.setParameterAsHidden("o_score_matrix");
	parpars.registerOptionalOutputFile("o_dcd", "output file name for the first cluster dcd file ");
	parpars.setParameterAsHidden("o_dcd");

	parpars.registerOptionalGalaxyOutputId("o_dcd_id", "output id ", "$o_dcd.id");
	// need to be hidden in command line mode
	parpars.setParameterAsAdvanced("o_dcd_id");
	parpars.setParameterAsHidden("o_dcd_id");

	parpars.registerOptionalGalaxyOutputFolder("o_dcd_dir", "output directory for 2nd to last cluster dcd file (if needed) ", "$__new_file_path__");
	// need to be hidden in command line mode
	parpars.setParameterAsAdvanced("o_dcd_dir");
	parpars.setParameterAsHidden("o_dcd_dir");

	// register String parameter for supplying minimal rmsd between clusters
	parpars.registerOptionalDoubleParameter("rmsd_cutoff", "minimal rmsd between the final clusters (default 5.0) ", 5.0);
	parpars.setParameterRestrictions("rmsd_cutoff", 0, 100);

	// choice of cluster algorithm  
	parpars.registerOptionalStringParameter("alg", "algorithm used for clustering (CLINK_DEFAYS, CLINK_ALTHAUS, NEAREST_NEIGHBOR_CHAIN_WARD, SLINK_SIBSON, TRIVIAL_COMPLETE_LINKAGE) ", "CLINK_DEFAYS");
	list<String> cluster_algs;
	cluster_algs.push_back("CLINK_DEFAYS");
	cluster_algs.push_back("CLINK_ALTHAUS");
	cluster_algs.push_back("TRIVIAL_COMPLETE_LINKAGE");
	cluster_algs.push_back("NEAREST_NEIGHBOR_CHAIN_WARD");
	cluster_algs.push_back("SLINK_SIBSON");
	parpars.setParameterRestrictions("alg", cluster_algs);

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

	// further optional output parameters
	parpars.registerOptionalOutputFile("o_red_dcd", "output file for the reduced cluster set (dcd with one structure per final cluster) ");

	// write the final cluster tree in boost::serialize format, if it was computed
	parpars.registerOptionalOutputFile("o_cluster_tree", "output file containing the cluster tree in boost::serialize format (if the tree was computed) ");

	// register bool parameter for using pre-clustering
	parpars.registerFlag("use_refinement", "Apply a second clustering run with different options (-refine_alg <string>, -refine_rmsd_type <string>, and -refine_rmsd_scope <string>)", false, true);

	// refinement algorithm
	parpars.registerOptionalStringParameter("refine_alg", "algorithm used for second clustering run (CLINK_DEFAYS, NEAREST_NEIGHBOR_CHAIN_WARD, SLINK_SIBSON, TRIVIAL_COMPLETE_LINKAGE) ", "CLINK_DEFAYS");
	parpars.setParameterAsHidden("refine_alg");
	parpars.setParameterRestrictions("refine_alg", cluster_algs);

	// refinement rmsd type
	parpars.registerOptionalStringParameter("refine_rmsd_type", "rmsd type used for second clustering run (SNAPSHOT_RMSD, RIGID_RMSD, CENTER_OF_MASS_DISTANCE) ", "SNAPSHOT_RMSD");
	parpars.setParameterAsHidden("refine_rmsd_type");
	parpars.setParameterRestrictions("refine_rmsd_type", rmsd_types);

	// refinement rmsd scope
	parpars.registerOptionalStringParameter("refine_rmsd_scope", "atoms to be considered for rmsd score in second clustering run (C_ALPHA, BACKBONE, ALL_ATOMS) ", "C_ALPHA");
	parpars.setParameterAsHidden("refine_rmsd_scope");
	parpars.setParameterRestrictions("refine_rmsd_scope", rmsd_levels);

	// force serial execution, even if the algorithm supports parallel runs
	parpars.registerFlag("run_serial", "force serial excecution, even if parallel execution would be supported by the algorithm", false, true);

	// the manual
	String man = "This tool computes clusters of docking poses given as conformation set or a list of rigid transformations.\n\nParameters are either the input ConformationSet (-i_dcd) and one corresponding pdb file (-i_pdb), or a transformation file (-i_trans). Output can be a cluster index list (-o_index_list), a cluster scoring matrix (-o_score_matrix), or dcd files per cluster (-o_dcd). Optional parameters are the algorithm (-alg), the minimal rmsd between the final clusters (-rmsd_cutoff), the rmsd type (-rmsd_type), and the type of atoms used for scoring a pose (-scope). The optional parameter -o_red_dcd sets the output file for the reduced cluster set (one representative per cluster). The optional parameter -o_cluster_tree specifies the output file for storing the cluster tree.\n\nOutput of this tool depends in the choice of the output parameters.";

	parpars.setToolManual(man);

	// here we set the types of I/O files
	parpars.setSupportedFormats("i_dcd","dcd");
	parpars.setSupportedFormats("i_pdb","pdb");
	parpars.setSupportedFormats("i_trans","txt");
	parpars.setSupportedFormats("o_index_list","txt");
	parpars.setSupportedFormats("o_score_matrix","txt");
	parpars.setSupportedFormats("o_dcd","dcd");
	parpars.setSupportedFormats("o_red_dcd","dcd");
	parpars.setSupportedFormats("o_cluster_tree","dat");

	parpars.parse(argc, argv);

	//////////////////////////////////////////////////

	if (parpars.has("o_dcd"))
	{
		if (!parpars.has("o_dcd_dir") || !parpars.has("o_dcd_id"))
		{
			Log << "Output type \"dcd\" requires setting the options \"o_dir\" \"o_id\"! Abort!" << endl;
			return 1;
		}
	}

	if (     parpars.has("o_cluster_tree")
			&& (!parpars.has("alg") || parpars.get("alg") != "NEAREST_NEIGHBOR_CHAIN_WARD"))
	{
		Log << "Output of cluster tree requires Ward algorithm! Abort!" << endl;
		return 1;
	}

	// read the input	
	PDBFile pdb;
	pdb.open(parpars.get("i_pdb"));
	System sys;
	pdb.read(sys);

	ConformationSet cs;
	cs.setup(sys);

	if (parpars.has("i_dcd"))
	{
		cs.readDCDFile(parpars.get("i_dcd"));
	}

	cs.resetScoring();

	PoseClustering pc;

	if (parpars.has("i_trans"))
	{
		pc.setBaseSystemAndTransformations(sys, parpars.get("i_trans"));
	}

	if (parpars.has("rmsd_cutoff"))
	{
		float rmsd = parpars.get("rmsd_cutoff").toInt();
		pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, rmsd);
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

	if (parpars.has("alg"))
	{
		String alg = parpars.get("alg");
		if (alg == "CLINK_DEFAYS")
			pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::CLINK_DEFAYS);
		else if (alg == "CLINK_ALTHAUS")
			pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::CLINK_ALTHAUS);
		else if (alg == "SLINK_SIBSON")
			pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::SLINK_SIBSON);
		else if (alg == "TRIVIAL_COMPLETE_LINKAGE")
			pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
		else if (alg == "NEAREST_NEIGHBOR_CHAIN_WARD")
			pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
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
		{
			pc.options.set(PoseClustering::Option::RMSD_TYPE, PoseClustering::CENTER_OF_MASS_DISTANCE);
			Log << "Parameter scope will be ignored!" << endl;
		}
		else
			Log.info() << "Unknown value " << type  << " for option rmsd_type." << endl;

	}

	if (parpars.has("run_serial"))
	{
		pc.options.set(PoseClustering::Option::RUN_PARALLEL, false);
	}
	else
	{
		pc.options.set(PoseClustering::Option::RUN_PARALLEL, true);
	}

	if (parpars.has("i_dcd"))
	{
		pc.setConformationSet(&cs);
	}

	pc.compute();

	// do we need a second clustering run?
	if (parpars.has("use_refinement"))
	{
		// get the options
		Options refine_options = pc.options;
		if (parpars.has("refine_rmsd_scope"))
		{
			String scope = parpars.get("refine_rmsd_scope");
			if (scope == "C_ALPHA")
				refine_options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::C_ALPHA);
			else if (scope == "BACKBONE")
				refine_options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::BACKBONE);
			else if (scope == "ALL_ATOMS")
				refine_options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::ALL_ATOMS);
			else
				Log.info() << "Unknown value " << scope  << " for option refine_rmsd_scope." << endl;
		}

		if (parpars.has("refine_alg"))
		{
			String alg = parpars.get("refine_alg");
			if (alg == "CLINK_DEFAYS")
				refine_options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::CLINK_DEFAYS);
			else if (alg == "CLINK_ALTHAUS")
				refine_options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::CLINK_ALTHAUS);
			else if (alg == "SLINK_SIBSON")
				refine_options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::SLINK_SIBSON);
			else if (alg == "TRIVIAL_COMPLETE_LINKAGE")
				refine_options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
			else if (alg == "NEAREST_NEIGHBOR_CHAIN_WARD")
				refine_options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
			else
				Log.info() << "Unknown value " << alg  << " for option refine_alg." << endl;
		}

		if (parpars.has("refine_rmsd_type"))
		{
			String type = parpars.get("refine_rmsd_type");
			if (type == "SNAPSHOT_RMSD")
				refine_options.set(PoseClustering::Option::RMSD_TYPE, PoseClustering::SNAPSHOT_RMSD);
			else if (type == "RIGID_RMSD")
				refine_options.set(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);
			else if (type == "CENTER_OF_MASS_DISTANCE")
			{
				refine_options.set(PoseClustering::Option::RMSD_TYPE, PoseClustering::CENTER_OF_MASS_DISTANCE);
				Log << "Parameter scope will be ignored!" << endl;
			}
			else
				Log.info() << "Unknown value " << type  << " for option refine_rmsd_type." << endl;
		}

		pc.refineClustering(refine_options);
	}

	Size num_clusters = pc.getNumberOfClusters();

	Log << "Computed " <<  num_clusters << " clusters, start writing..." << endl;

	if (parpars.has("o_dcd"))
	{
		for (Size i = 0; i < num_clusters; i++)
		{
			Log << "   Cluster " << i << " has " << pc.getClusterSize(i) << " members." << endl;

			boost::shared_ptr<ConformationSet> new_cs = pc.getClusterConformationSet(i);

			String outfile_name = (i == 0) ? String(parpars.get("o_dcd"))
				: String(parpars.get("o_dcd_dir")) + "/primary_"
				+ String(parpars.get("o_dcd_id"))  + "_cluster" + String(i)
				+ "_visible_dcd";
			//Log << "   Writing solution " << String(i) << " as " << outfile_name << endl;

			new_cs->writeDCDFile(outfile_name);
		}
	}
	if (parpars.has("o_index_list"))
	{
		String outfile_name = String(parpars.get("o_index_list"));

		File cluster_outfile(outfile_name, std::ios::out);

		pc.printClusters(cluster_outfile);
	}
	if (parpars.has("o_score_matrix"))
	{
		String outfile_name = String(parpars.get("o_score_matrix"));

		File cluster_outfile(outfile_name, std::ios::out);

		pc.printClusterScores(cluster_outfile);
	}

	// print
	pc.printClusters();
	pc.printClusterScores();

	if (parpars.has("o_cluster_tree"))
	{
		File cluster_out(parpars.get("o_cluster_tree"), std::ios::out);
		pc.serializeWardClusterTree(cluster_out, true);
		cluster_out.close();
	}

	if (parpars.has("o_red_dcd"))
	{
		String outfile_name = String(parpars.get("o_red_dcd"));
		boost::shared_ptr<ConformationSet> cs = pc.getReducedConformationSet();
		cs->writeDCDFile(outfile_name);
	}

	Log << "done." << endl;

	return 0;
}

