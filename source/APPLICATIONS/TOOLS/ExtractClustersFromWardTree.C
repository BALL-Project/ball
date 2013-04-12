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
	CommandlineParser parpars("ExtractClustersFromWardTree", "extracts docking clusters ", VERSION, String(__DATE__), "Docking");

	// we register an input file parameter 
	// - CLI switch
	// - description
	// - parameter type
	// - required
	parpars.registerParameter("i", "input cluster-file", INFILE, true);

	// we register an output file parameter 
	// - CLI switch
	// - description
	// - parameter type
	// - required
	// - default value
	// - hidden in galaxy
	parpars.registerParameter("o_out", "output file name ", STRING, false, "", true);

	// we register the output type
	parpars.registerParameter("o_type", "output type (gv, index_list) ", STRING, false, "index_list");
	list<String> output_types;
	output_types.push_back("gv");
	output_types.push_back("index_list");
	parpars.setParameterRestrictions("o_type", output_types);

	// we register the cutoff type
	parpars.registerParameter("cutoff_type", "cutoff type (ward_distance, num_clusters) ", STRING, false, "ward_distance");
	list<String> cutoff_types;
	cutoff_types.push_back("ward_distance");
	cutoff_types.push_back("num_clusters");
	parpars.setParameterRestrictions("cutoff_type", cutoff_types);

	// we register the cutoff value, either the minimal ward distance between the clusters
	//                               or the number of clusters to split into
	parpars.registerParameter("cut_value", "cut value for splitting the given WART tree using the cutoff-type (default 5.0) ", DOUBLE, false, 5.0);
	parpars.setParameterRestrictions("cut_value", 0.0, 10000);

	// we register a parameter defining the minimal size of clusters - e.g. for filtering out single outlieers
	parpars.registerParameter("min_size", "minimal size of clusters (default 1) ", INT, false, 1);
	parpars.setParameterRestrictions("min_size", 1, 10000);

  // the manual
	String man = "This tool extracts clusters of docking poses given a dat file.\n\nParameters are the filename (-i_out), the cutoff type (-cutoff_type)... . The optional parameter -min_size allows to filter outliers.\n\nOutput of this tool is either an index list of the clusters or a gv of the extracted cluster tree.";

	parpars.setToolManual(man);

	// here we set the types of I/O files
	parpars.setSupportedFormats("i", "dat");
	parpars.setSupportedFormats("o_out", "txt,gv");

	parpars.parse(argc, argv);

	//////////////////////////////////////////////////


	// read the input	
	File tree;
	tree.open(parpars.get("i"));

	PoseClustering pc;
	// this only works for Nearest neighbor chain ward trees...
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);

	// import
	pc.deserializeWardClusterTree(tree, true);


	float cut_value = 5;
	if (parpars.has("cut_value"))
	{
		cut_value = parpars.get("cut_value").toFloat();
	}
	else
	{
		Log << "Assume default value of " << cut_value << " for parameter cut_value!" << endl;
	}

	int min_cluster_size = 0;
	if (parpars.has("min_size"))
	{
		min_cluster_size = parpars.get("min_size").toInt();
	}

	float num_clusters_to_extract = 5.;
	int   max_ward_dist = 5;
	std::vector<std::set<Index> > clusters;
	if (parpars.has("cutoff_type"))
	{
		String type = parpars.get("cutoff_type");
		if (type == "ward_distance")
		{
			max_ward_dist = cut_value;
			clusters = pc.extractClustersForThreshold(max_ward_dist, min_cluster_size);
		}
		else if (type == "num_clusters")
		{
			num_clusters_to_extract = cut_value;
			clusters = pc.extractNBestClusters(num_clusters_to_extract, min_cluster_size);
		}
		else
		{
			Log.info() << "Unknown value " << type  << " for option cutoff_type." << endl;
			return 1;
		}
	}

	Log << "Extracted " <<  clusters.size() << " clusters, start writing..." << endl;

	String outfile_name = String(parpars.get("o_out"));

	if (parpars.get("o_type") == "index_list")
	{
		File cluster_outfile(outfile_name, std::ios::out);

		pc.printClusters(cluster_outfile);
	}
	else if (parpars.get("o_type") == "gv")
	{
		File gv_outfile(outfile_name, std::ios::out);

		pc.exportWardClusterTreeToGraphViz(gv_outfile);
		gv_outfile.close();
	}
	else
	{
		Log << "Unspecified output!!" << endl;
	}

	Log << "done." << endl;

	return 0;
}

