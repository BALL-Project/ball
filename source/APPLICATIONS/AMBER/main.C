// $Id: main.C,v 1.1 2000/06/10 11:23:22 oliver Exp $

#include <iomanip>

#include "global.h"
#include "files.h"
#include <BALL/SYSTEM/timer.h>
#include <BALL/STRUCTURE/numericalSAS.h>
#include <BALL/STRUCTURE/analyticalSES.h>

using namespace BALL;
using namespace std;

void usage()
{
	Log.error() << "BALL -- AMBER optimization and energy calculation" << endl 
							<< endl
	            << "amber <main_options> [<options>]" << endl
	            <<"   where <main_options> is one or more of the following possibilities:" << endl
	            << "     -E                   perform an energy calculation" << endl
	            << "     -O                   perform an energy minimization" << endl
	            << "     -C                   check the input structures" << endl
	            << endl
	            << "and <options> one or more of" << endl
	            << "     -p <FILE>            read <FILE> as a PDB file" << endl
	            << "     -h <FILE>            read <FILE> as a HyperChem file" << endl
	            << "     -H <FILE>            read <FILE> as a HyperChem file but do not assign charges" << endl
	            << "     -n                   normalize all atom names in subsequently read structures" << endl
	            << "     -b                   try to build the bonds (e.g. for PDB files)" << endl
	            << "     -v                   verbose output" << endl
							<< "     -g <GRAD>            gradient criterion for optimization (in units of kJ/(mol A))" << endl
							<< "     -s <STRING>          select only the atoms that match <STRING>" << endl
							<< "     -f <FILE>            force field file (default: Amber/amber91.ini)" << endl
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
		// options requiring a filename (-p -h -H -g -s -f)
		if (String("phHgsf").has(option[1]) && (i == (argc - 1)))
		{
			// pring usage hints, an error message, exit
			usage();
			Log.error() << "Option " << option << " requires an additional argument." << endl;
			return 3;
		}		

		// interpret all command line options
		switch (option[1])
		{
			// check for the 'main' options (single point/optimization)
			case 'E':
				// perform a single point calculation

				energy_calculation = true;
				break;

			case 'O':
				// perform an optimization
				energy_minimization = true;
				break;

			case 'C':
				// check the structures
				check_structures = true;
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
		
			case 'v':		// change verbosity
				verbose = true;
				break;
		
			case 'n':		// normalize names for next files read
				// set the normalize names flag
				normalize_names = true;
				break;

			case 'b':		// build bonds for next files read
				// set the normalize names flag
				build_bonds = true;
				break;

			case 's':		// selection
				use_selection = true;
				selection = argv[++i];
				break;

			case 'g':		// selection
				max_gradient = atof(argv[++i]);
				break;

			case 'f':		// selection
				FF_filename = argv[++i];
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
	if (!(energy_calculation || energy_minimization || check_structures))
	{
		usage();
		Log.error() << "Error: specify at least one of the main options -E, -O, or -C!" << endl;
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

	if (check_structures)
	{
		checkStructures();
	}
	if (energy_calculation || energy_minimization)
	{
		setup();
		singlePoint();
	}
	if (energy_minimization)	
	{
		optimize();
		singlePoint();
	}

	// done
	return 0;
}
