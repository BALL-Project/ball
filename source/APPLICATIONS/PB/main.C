// $Id: main.C,v 1.5 2000/06/02 09:32:54 oliver Exp $

#include "global.h"
#include "reading.h"
#include "assignment.h"
#include <BALL/SYSTEM/timer.h>
#include <BALL/STRUCTURE/numericalSAS.h>

using namespace BALL;
using namespace std;

void usage()
{
	Log.error() << "BALL -- Finite Difference Poisson-Boltzmann Solver" << endl << endl;
	Log.error() << "PB [<options>]" << endl;
	Log.error() <<"   where <options> is one or more of the following possibilities:" << endl;
	Log.error() << "     -p <FILE>            read <FILE> as a PDB file" << endl;
	Log.error() << "     -h <FILE>            read <FILE> as a HyperChem file" << endl;
	Log.error() << "     -H <FILE>            read <FILE> as a HyperChem file but do not assign charges" << endl;
	Log.error() << "     -o <FILE>            read FDPB options from <FILE>" << endl;
	Log.error() << "     -c <FILE>            read charges from <FILE>" << endl;
	Log.error() << "     -r <FILE>            read radii from <FILE>" << endl;
	Log.error() << "     -t <FILE>            read charge and radius rules from <FILE>" << endl;
	Log.error() << "     -u <FILE>            read charge rules from <FILE>" << endl;
	Log.error() << "     -w <FILE>            read radius rules from <FILE>" << endl;
	Log.error() << "     -0                   clear all charges in subsequently read structures" << endl;
	Log.error() << "     -s                   calculate the solvation free energy by performing a second" << endl;
	Log.error() << "                          FDPB calculation in vacuum" << endl;
	Log.error() << "     -a                   calculate the solvent accessible surface of the solute" << endl;
	Log.error() << "     -n                   normalize all atom names in subsequently read structures" << endl;
	Log.error() << "     -b                   try to build the bonds (e.g. for PDB files)" << endl;
	Log.error() << "     -d <FILE>            dump the atom charges and radii to <FILE> (for debugging)" << endl;
	Log.error() << "     -v                   verbose output (implies ``verbosity 99'' in the" << endl;
	Log.error() << "                            option file, print additional results and options)" << endl;
	Log.error() << endl;
	Log.error() << "  By default, charges and radii are taken from data/charges/PARSE.crg" << endl;
	Log.error() << "  and data/radii/PARSE.siz." << endl << endl;
	Log.error() << "  Charge and radius assignment options can be repeated. They are valid for all" << endl;
	Log.error() << "  subsequently read structures." << endl;
	Log.error() << endl;
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
		// options requiring a filename (-p -h -c -r -o -u -t -w -d)
		if (String("phcroutwd").has(option[1]) && (i == (argc - 1)))
		{
			// pring usage hints, an error message, exit
			usage();
			Log.error() << "Option " << option << " requires an additional argument (filename)." << endl;
			return 3;
		}		

		// interpret all command line options
		switch (option[1])
		{
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

			case 'a':		// calculate solvent excluded surface
				calculate_SES = true;
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

			default:		// unknown option
				// print usage hints and an error message, exit
				usage();
				Log.error() << "Illegal option: " << option << endl;
				return 2;
		}
	}

	// check whether anything was constructed at all
	if (S.countAtoms() == 0)
	{
		usage();
		Log.error() << "Error: no atoms given." << endl;
		return 6;
	}

	// if the option -d was give, dump the positions, charges, and radii to a file
	if (dump_file != "")
	{
		dumpFile();
	}

	// setup logging to print the current time in front of each line
	Log.setPrefix(cout, "[%T] ");
	Log.setPrefix(cerr, "[%T ERROR] ");

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

	// calculate the solvent excluded surface area of the solute
	// (used to estimate the non electrostatic contribution
	// to the solvation free energy)
	if (calculate_SES)
	{
		double A = calculateNumericalSASArea(S, 1.4);
		Log.info() << "Solvent accessible surface : " << A << "A^2";
		Log.info() << " (1.4 Angstrom probe radius)" << endl;
	}

	// done
	return 0;
}
