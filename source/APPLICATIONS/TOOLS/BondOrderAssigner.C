// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/KERNEL/system.h>

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
	CommandlineParser parpars("BondOrderAssigner", "computes bond order assignments for a ligand ", VERSION, String(__DATE__), "Preparation");

	// we register an input file parameter 
	// - CLI switch
	// - description
	// - Inputfile
	// - required
	parpars.registerParameter("i", "input mol2-file", INFILE, true);

	// we register an output file parameter 
	// - description
	// - Outputfile
	// - required
	parpars.registerParameter("o", "output mol2-file name for first solution", OUTFILE, true, "", true);

	// parameters for galaxy for handling multiple output files
	parpars.registerParameter("o_id", "output id", GALAXY_OPT_OUTID, false, "$o.id", true);
	// need to be hidden in command line mode
	parpars.setParameterAsAdvanced("o_id");

	// parameters for galaxy for handling multiple output files
	parpars.registerParameter("o_dir", "output directory for 2nd to last solution", GALAXY_OPT_OUTDIR, false, "$__new_file_path__", true);
	// need to be hidden in command line mode
	parpars.setParameterAsAdvanced("o_dir");

	// register String parameter for supplying max number of solutions
	parpars.registerParameter("max_sol", "maximal number of assignments solutions to compute", INT, false, 25);
	parpars.setParameterRestrictions("max_sol", 0, 100);

	// parameter for computing sub-optimal solutions
	parpars.registerFlag("non_opt", "compute sub-optimal assignments as well", false);

	// option for hydrogen addition 
	// TODO: test and then release :-)
	//parpars.registerFlag("add_hyd", "add hydrogens as well", false);

	// choice of penalty table 
	parpars.registerParameter("scr_pen", "penalty table (Antechamber, BALL)", STRING, false, "Antechamber");
	list<String> ini_files;
	ini_files.push_back("Antechamber");
	ini_files.push_back("BALL");
	parpars.setParameterRestrictions("scr_pen", ini_files);

  // the manual
	String man = String("This tool computes optimal and sub-optimal bond order assignments based on an atomic penalty function for a given ligand in mol2 file format.\n\nOptional parameters are the maximal number of solutions to be computed ('-max_sol'), the penalty table specifiying the atomic penalty rules ('-scr_pen'),and a flag indicating if sub-optimal solutions should be computed as well ('-non_opt')") + // and a flag indicating if hydrogens should be computed as well ('-add_hyd')
	".\n\nOutput of this tool is a number of mol2 files each containing one bond order assignment.\n\nTo upload an input file please use the upload tool (Get Data -> Upload File).\n\n**Further information and help** can be found in our wiki http://ball-trac.bioinf.uni-sb.de/wiki/ballaxy/BOAConstructor_Help.\n\nPlease cite the following: Dehof, A.K., Rurainski, A., Bui, Q.B.A., Boecker, S., Lenhof, H.-P. & Hildebrandt, A. (2011). Automated Bond Order Assignment as an Optimization Problem. Bioinformatics, 2011";

	parpars.setToolManual(man);

	// here we set the types of I/O files, for example sdf is also allowed
	parpars.setSupportedFormats("i","mol2");
	parpars.setSupportedFormats("o","mol2");

	parpars.parse(argc, argv);

	// read the input
	MOL2File f0;
	f0.open(parpars.get("i"));
	System system;
	f0 >> system;

	AssignBondOrderProcessor abop;

	// the combination of the following two options causes the computation of all optimal solutions
	abop.options.setInteger(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 0);
	abop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, false);

	if (parpars.has("max_sol"))
	{
		int max_sol = parpars.get("max_sol").toInt();
		Log << "  Limit number of solutions to " << max_sol << endl;
		abop.options.setInteger(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, String(max_sol).toInt());
	}

	bool non_opt = false;
	if (parpars.has("non_opt"))
	{
		non_opt = parpars.get("non_opt").toBool();
		if (non_opt)
			Log << "  Compute also non-optimal solutions." << endl;
		abop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, non_opt);
	}

	if (parpars.has("add_hyd"))
	{
		bool add_hyd = parpars.get("add_hyd").toBool();
		//if (add_hyd)
		//	cout << "   Add hydrogens as well." << endl;
		abop.options.setBool(AssignBondOrderProcessor::Option::ADD_HYDROGENS, add_hyd);
	}

	if (parpars.has("scr_pen"))
	{
		String penalty_table = parpars.get("scr_pen");
		if (penalty_table == "Antechamber")
		{
			Log << "  Use Antechamber scoring function" << endl;
			abop.options[AssignBondOrderProcessor::Option::INIFile] = "/bond_lengths/BondOrderGAFF.xml";
		}
		else
		{
			Log << "  Use BALL scoring function" << endl;
			abop.options[AssignBondOrderProcessor::Option::INIFile] = "/bond_lengths/BondOrder.xml";
		}
	}

	// set the solver
	abop.options.set(AssignBondOrderProcessor::Option::ALGORITHM, AssignBondOrderProcessor::Algorithm::FPT);
	//Log << "parameters are: " << endl;
	//abop.options.dump();
	Log << "run ..." << endl;

	system.apply(abop);
	Size num_of_sols = abop.getNumberOfComputedSolutions();

	if (num_of_sols == 0)
	{
		Log << "No valid bond order assignment found!" << endl;
		return 1;
	}
	else
	{
		// called as command line or e.g. via galaxy?
		bool is_cmd =    !parpars.has("env")
			            || ((parpars.has("env") && parpars.get("env")=="cmdline"));

		//Log << "Found " << num_of_sols << " solutions:" << endl;
		for (Size i=0; (i<num_of_sols) && (non_opt || (abop.getTotalPenalty(0)==abop.getTotalPenalty(i))); i++)
		{
			Log << "   Solution " << i << " has penalty " << abop.getTotalPenalty(i) << " ... ";

			// apply the solution
			if (abop.apply(i))
			{
				// create the output name
				String outfile_name = String(parpars.get("o")) + "solution_"
															+ String(i) + ".mol2";
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
				                                 + String(parpars.get("o_id"))  + "_solution" + String(i)
				                                 + "_visible_mol2";
				}
				//Log << "   Writing solution " << String(i) << " as " << outfile_name << endl;
				//	GenericMolFile* outfile = MolFileFactory::open(outfile_name, ios::out);
				MOL2File outfile(outfile_name, ios::out);

				if (outfile.bad())
				{
					Log.error() << endl << "cannot write file " << outfile_name << endl;
					return 2;
				}

				system.beginMolecule()->setProperty("BOA_Constructor_penalty", abop.getTotalPenalty(i));

				outfile << system;
				outfile.close();

				Log << "wrote file " << outfile_name << endl;
			}
		}
	}
	Log << "done." << endl;

	return 0;
}

