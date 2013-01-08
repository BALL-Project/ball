// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/DOCKING/COMMON/poseClustering.h>
#include <BALL/FORMAT/DCDFile.h>
#include <BALL/FORMAT/PDBFile.h>
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
	// - required
	parpars.registerParameter("i_dcd", "input dcd-file", INFILE, true);
	parpars.registerParameter("i_pdb", "input pdb-file", INFILE, true);

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


	// choice of atom rmsd scope 
	parpars.registerParameter("rmsd_scope", "atoms to be considered for rmsd score (C_ALPHA, BACKBONE, ALL_ATOMS) ", STRING, false, "C_ALPHA");
	list<String> rmsd_levels;
	rmsd_levels.push_back("C_ALPHA");
	//rmsd_levels.push_back("HEAVY_ATOMS"); //TODO
	rmsd_levels.push_back("BACKBONE");
	rmsd_levels.push_back("ALL_ATOMS");
	parpars.setParameterRestrictions("rmsd_scope", rmsd_levels);

	// choice of cluster algorithm  
	parpars.registerParameter("alg", "algorithm used for clustering (CLINK_DEFAYS, SLINK_SIBSON) ", STRING, false, "CLINK_DEFAYS");
	list<String> cluster_algs;
	cluster_algs.push_back("CLINK_DEFAYS");
	cluster_algs.push_back("SLINK_SIBSON");
	parpars.setParameterRestrictions("alg", cluster_algs);

  // the manual
	String man = "This tool computes clusters of docking poses given as conformation set using the SLINK or CLINK algorithm.\n\nParameters are the input ConformationSet (-i_dcd), one corresponding pdb file (-i_pdb), the algorithm (-alg) and a naming schema for the results (-o). Optional parameters are the minimal rmsd between the final clusters  (-rmsd_cutoff) and the scope/level of detail of the rmsd computation (-rmsd_scope). The optional parameter (-o_dcd)\n\nOutput of this tool is a number of dcd files each containing one ConformationSet.";

	parpars.setToolManual(man);

	// here we set the types of I/O files
	parpars.setSupportedFormats("i_dcd","dcd");
	parpars.setSupportedFormats("i_pdb","pdb");
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
	cs.readDCDFile(parpars.get("i_dcd"));
	cs.resetScoring();

	PoseClustering pc;

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
	}

	if (parpars.has("alg"))
	{
		String alg = parpars.get("alg");
		if (alg == "CLINK_DEFAYS")
			pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::CLINK_DEFAYS);
		else if (alg == "SLINK_SIBSON")
			pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::SLINK_SIBSON);
	}

	pc.setConformationSet(&cs);

	pc.compute();

	Size num_clusters = pc.getNumberOfClusters();

	for (Size i = 0; i < num_clusters; i++)
	{
		Log << "   Cluster " << i << " has " << pc.getClusterSize(i) << " members." << endl;

		boost::shared_ptr<ConformationSet> new_cs = pc.getClusterConformationSet(i);

		String outfile_name = (i == 0) ? String(parpars.get("o"))
			                             : String(parpars.get("o_dir")) + "/primary_"
			                               + String(parpars.get("o_id"))  + "_cluster" + String(i)
			                               + "_visible_dcd";
		//Log << "   Writing solution " << String(i) << " as " << outfile_name << endl;

		new_cs->writeDCDFile(outfile_name);
	}

	if (parpars.has("o_dcd"))
	{
		String outfile_name = String(parpars.get("o_dcd"));
		boost::shared_ptr<ConformationSet> cs = pc.getReducedConformationSet();
		cs->writeDCDFile(outfile_name);
	}

	Log << "done." << endl;

	return 0;
}

