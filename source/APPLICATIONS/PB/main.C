// $Id: main.C,v 1.7 2000/06/08 08:36:04 oliver Exp $

#include <iomanip>

#include "global.h"
#include "reading.h"
#include "assignment.h"
#include <BALL/SYSTEM/timer.h>
#include <BALL/STRUCTURE/numericalSAS.h>
#include <BALL/STRUCTURE/analyticalSES.h>

using namespace BALL;
using namespace std;

void usage()
{
	Log.error() << "BALL -- Finite Difference Poisson-Boltzmann Solver" << endl 
							<< endl
	            << "PB [<options>]" << endl
	            <<"   where <options> is one or more of the following possibilities:" << endl
	            << "     -P                   perform a Finite DIfference Poisson Boltzmann calculation" << endl
	            << "     -A                   calculate the solvent accessible surface of the solute" << endl
	            << "     -E                   calculate the solvent excluded surface of the solute" << endl
	            << endl
	            << "further options:" << endl
	            << "     -p <FILE>            read <FILE> as a PDB file" << endl
	            << "     -h <FILE>            read <FILE> as a HyperChem file" << endl
	            << "     -H <FILE>            read <FILE> as a HyperChem file but do not assign charges" << endl
	            << "     -o <FILE>            read FDPB options from <FILE>" << endl
	            << "     -c <FILE>            read charges from <FILE>" << endl
	            << "     -r <FILE>            read radii from <FILE>" << endl
	            << "     -t <FILE>            read charge and radius rules from <FILE>" << endl
	            << "     -u <FILE>            read charge rules from <FILE>" << endl
	            << "     -w <FILE>            read radius rules from <FILE>" << endl
	            << "     -0                   clear all charges in subsequently read structures" << endl
	            << "     -s                   calculate the solvation free energy by performing a second" << endl
	            << "                          FDPB calculation in vacuum" << endl
	            << "     -n                   normalize all atom names in subsequently read structures" << endl
	            << "     -b                   try to build the bonds (e.g. for PDB files)" << endl
	            << "     -d <FILE>            dump the atom charges, radii, and surface fractions to <FILE> (for debugging)" << endl
	            << "     -v                   verbose output (implies ``verbosity 99'' in the" << endl
	            << "                            option file, print additional results and options)" << endl
	            << "     -x <RADIUS>          the probe sphere radius for solvent accessible and" << endl
	            << "                            solvent excluded surface calculations [default = 1.4 A]" << endl
	            << endl
	            << "  By default, charges and radii are taken from data/charges/PARSE.crg" << endl
	            << "  and data/radii/PARSE.siz." << endl 
							<< endl
	            << "  Charge and radius assignment options can be repeated. They are valid for all" << endl
	            << "  subsequently read structures." << endl
	            << endl;
}

int main(int argc, char** argv)
{
	// print usage if no argument was given
	if (argc == 1)
	{
		usage();
		return 1;
	}

	// check all command line options
	for (int i = 1; i < argc; i++)
	{
		// retrieve the current option
		String option(argv[i]);

		// check for leading "-" and length of argument
		if ((option[0] != '-') || (option.size() != 2))
		{
			// print usage hints and an error message, exit
			usage();
			Log.error() << "Illegal option: " << option << endl;
			return 2;
		}

		// check for another argument for those 
		// options requiring a filename (-p -h -c -r -o -u -t -w -d -x)
		if (String("phcroutwdx").has(option[1]) && (i == (argc - 1)))
		{
			// pring usage hints, an error message, exit
			usage();
			Log.error() << "Option " << option << " requires an additional argument." << endl;
			return 3;
		}		

		// interpret all command line options
		switch (option[1])
		{
			// check for the 'main' options (FDPB/SAS/SES)
			case 'P':
				// perform a FDPB calculation
				fdpb_calculation = true;
				break;

			case 'A':
				// perform a SAS calculation
				sas_calculation = true;
				break;

			case 'E':
				// perform a SES calculation
				ses_calculation = true;
				break;
			
			// further options

			case 'p':		// read a PDB file
				readPDBFile(argv[++i]);
				break;

			case 'h':		// read a HyperChem file
				readHINFile(argv[++i]);
				break;
		
			case 'H':		// read a HyperChem file but do not assign chargs
				readHINFileNoAssignment(argv[++i]);
				break;
		
			case 'o':		// read an option file
				readOptionFile(argv[++i]);
				break;

			case 'c':		// read a charge file
				readChargeFile(argv[++i]);
				break;

			case 'r':		// read a radius file
				readRadiusFile(argv[++i]);
				break;

			case 'u':		// read a rule file
				readRuleFile(argv[++i], CHARGES_AND_RADII);
				break;

			case 't':		// read a rule file
				readRuleFile(argv[++i], CHARGES);
				break;

			case 'w':		// read a rule file
				readRuleFile(argv[++i], RADII);
				break;

			case 's':		// calculate solvation energy
				calculate_solvation_energy = true;
				break;

			case 'd':		// dump the final results
				dump_file = argv[++i];
				break;

			case 'v':		// change verbosity
				options[FDPB::Option::VERBOSITY] = 99;
				verbose = true;
				break;
		
			case '0':		// clear charges for next files read
				// set the clear charges flag (reset by -c)
				clear_charges = true;
				break;

			case 'n':		// normalize names for next files read
				// set the normalize names flag
				normalize_names = true;
				break;

			case 'b':		// build bonds for next files read
				// set the normalize names flag
				build_bonds = true;
				break;

			case 'x':		// set the probe sphere radius
				probe_radius = atof(argv[++i]);
				if (verbose)
				{
					Log.info() << "probe sphere radius for surface calculations is set to " 
										 << probe_radius << " Angstrom" << endl;
				}
				break;

			default:		// unknown option
				// print usage hints and an error message, exit
				usage();
				Log.error() << "Illegal option: " << option << endl;
				return 2;
		}
	}

	// check whether at least one of the main options 
	// was given 
	if (!(fdpb_calculation || ses_calculation || sas_calculation))
	{
		usage();
		Log.error() << "Error: specify at least one of the main options -P, -A, or -E!" << endl;
		return 8;
	}

	// check whether anything was constructed at all
	if (S.countAtoms() == 0)
	{
		usage();
		Log.error() << "Error: no atoms given." << endl;
		return 6;
	}

	// setup logging to print the current time in front of each line
	Log.setPrefix(cout, "[%T] ");
	Log.setPrefix(cerr, "[%T ERROR] ");


	// calculate the solvent excluded surface area of the solute
	// (used to estimate the non electrostatic contribution
	// to the solvation free energy)
	if (ses_calculation)
	{
		total_SES_area = calculateSESArea(S, probe_radius);
		Log.info() << "Solvent excluded surface : " << total_SES_area << " A^2";
		Log.info() << " (" << probe_radius << " Angstrom probe radius)" << endl;
	}

	// calculate the solvent accessible surface area of the solute
	// (used to estimate the non electrostatic contribution
	// to the solvation free energy)
	if (sas_calculation)
	{
		total_SAS_area = calculateSASAtomAreas(S, surface_map, probe_radius);
		Log.info() << "Solvent accessible surface : " << total_SAS_area << " A^2";
		Log.info() << " (" << probe_radius << " Angstrom probe radius)" << endl;
	}

	// if the option -d was give, dump the positions, charges, and radii to a file
	if (dump_file != "")
	{
		dumpFile();
	}

	// the part that performs the FDPB calculation
	if (fdpb_calculation)
	{
		// setup the calculation
		Timer T;
		T.start();
		fdpb.setup(S, options);
		if (verbose)
		{
			Log.info() << "FDPB setup CPU time: " << T.getCPUTime() << endl;
		}

		if (calculate_solvation_energy)
		{
			Log.info() << "Calculating the solvation free energy." << endl;
			Log.info() << "first calculation step: solvent dielectric constant = " 
								 << fdpb.options[FDPB::Option::SOLVENT_DC] << endl;
		}

		// solve the PB equation
		T.reset();
		fdpb.solve();
		if (verbose)
		{
			Log.info() << "FDPB solve CPU time: " << T.getCPUTime() << endl;
			// dump the options for documentation purposes
			fdpb.options.dump(Log);
		}
		
		// print the energies
		Log.info() << "total energy:          " << fdpb.getEnergy() << " kJ/mol" << endl;
		Log.info() << "reaction field energy: " << fdpb.getReactionFieldEnergy() << " kJ/mol" << endl;

		if (calculate_solvation_energy)
		{
			Log.info() << "Calculating the solvation free energy." << endl;
			Log.info() << "first calculation step: solvent dielectric constant = " 
								 << fdpb.options[FDPB::Option::SOLVENT_DC] << endl;
		}

		T.start();
		fdpb.setup(S, options);
		if (verbose)
		{
			Log.info() << "FDPB setup CPU time: " << T.getCPUTime() << endl;
		}

		if (calculate_solvation_energy)
		{
			Log.info() << "second calculation step: solvent dielectric constant = 1.0 (vacuum)"  << endl;

			// store the old energies
			double dG = fdpb.getEnergy();
			double dG_RF = fdpb.getReactionFieldEnergy();

			T.reset();
			options[FDPB::Option::SOLVENT_DC] = 1.0;
			fdpb.setup(S, options);
			if (verbose)
			{
				Log.info() << "FDPB setup CPU time: " << T.getCPUTime() << endl;
			}

			// solve the PB equation
			T.reset();
			fdpb.solve();
			if (verbose)
			{
				Log.info() << "FDPB solve CPU time: " << T.getCPUTime() << endl;
				// dump the options for documentation purposes
				fdpb.options.dump(Log);
			}

			// print the energies
			Log.info() << "total energy:          " << fdpb.getEnergy() << " kJ/mol" << endl;
			Log.info() << "reaction field energy: " << fdpb.getReactionFieldEnergy() << " kJ/mol" << endl;
			Log.info() << endl;
			
			Log.info() << "Solvation energy as change of the total energy:   " 
								 << dG - fdpb.getEnergy()<< " kJ/mol" << endl;
			Log.info() << "Solvation energy as change of the reaction field: " 
								 << dG_RF - fdpb.getReactionFieldEnergy() << " kJ/mol" << endl;
		}
	}

	// done
	return 0;
}
