// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// A small program for computing bond orders of a molecule
// by means of a atomic valence penalty function.


#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>

using namespace std;
using namespace BALL;

int main(int argc, char** argv)
{
	if (argc < 3)
	//	if (argc != 3)
	{
		Log << "Usage:" << argv[0] << " <Molecule infile> <Molecule outfile> [--num-sol=n] [--strategy=(fpt|a_star|ilp)] [--max-penalty=m]" << endl;
		return 1;
	}

	Log << "Loading..." << argv[1] << "..." << endl;
	GenericMolFile* infile = MolFileFactory::open(argv[1]);

	if (!infile)
	{
		Log.error() << "Could not determine filetype, aborting" << std::endl;
		return 2;
	}

	if (!*infile)
	{
		std::cerr << "Invalid file, aborting" << std::endl;
		return 2;
	}

	System system;
	*infile >> system;

	Log << "Compute bond orders..." << endl;
	AssignBondOrderProcessor abop;
	Log << "Current penalty: " << abop.evaluatePenalty(&system) << endl;

	//
	//    set the options:
	//
	// the solution strategy (A*, ILP or FPT)
	abop.options.set(AssignBondOrderProcessor::Option::ALGORITHM, AssignBondOrderProcessor::Algorithm::FPT);

	// specify the inifile with the atomic valence penalties
	abop.options.set(AssignBondOrderProcessor::Option::INIFile, AssignBondOrderProcessor::Default::INIFile);

	// options for considering bond length as well 
//	abop.options.setReal(AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING, 0);
//	abop.options.setReal(AssignBondOrderProcessor::Option::USE_FINE_PENALTY, true);

	// the combination of the following two options causes the computation of all optimal solutions
	abop.options.setInteger(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 0);
	abop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, false);

	if (argc > 3)
	{
		for (Index i=3; i<argc; ++i)
		{
			if (String(argv[i]).hasPrefix("--strategy"))
			{
				abop.options.set(AssignBondOrderProcessor::Option::ALGORITHM, String(argv[i]).getField(1, "="));
			}
			else if (String(argv[i]).hasPrefix("--num-sol"))
			{
				Log << "  with compute fixed number (" << argv[i] << ") solutions" << endl;
				abop.options.setInteger(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, String(argv[i]).getField(1, "=").toInt());
				abop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);
			}
			else if (String(argv[i]).hasPrefix("--max-penalty"))
			{
				abop.options.setInteger(AssignBondOrderProcessor::Option::MAX_PENALTY, String(argv[i]).getField(1, "=").toInt());
				abop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);
				Log << "  with maximum penalty (" <<  abop.options.getInteger(AssignBondOrderProcessor::Option::MAX_PENALTY) << ")" << endl;
			}
		}
	}

	// define input and output properties
	abop.options.setBool(AssignBondOrderProcessor::Option::KEKULIZE_RINGS, true);
	abop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SELECTED_BONDS, false);

/*	cout << endl;
	abop.options.dump();
	cout << endl;
*/
	// compute
	system.apply(abop);
	Size num_of_sols = abop.getNumberOfComputedSolutions();

	if (num_of_sols == 0)
	{
		Log << "No valid bond order assignment found!" << endl;
		return 1;
	}
	else
	{
		Log << "Found " << num_of_sols << " solutions:" << endl;

		for (Size i=0; (i<num_of_sols) /*&& (abop.getTotalPenalty(0)==optimal_score)*/; i++)
		{
			Log << "   Solution " << i << " has penalty " << abop.getTotalPenalty(i) << endl;

			// apply the solution
			if (abop.apply(i))
			{
				String outfile_name = String(argv[2]) + "_" + String(i) + ".mol2";
				Log << "   Writing solution " << String(i) << " as " << outfile_name << endl;
				GenericMolFile* outfile = MolFileFactory::open(outfile_name, ios::out);

				if (!outfile)
				{
					cerr << "Could not determine filetype, aborting" << endl;
					exit(-1);
				}

				if (!*outfile)
				{
					cerr << "Invalid file, aborting" << endl;
					exit(-1);
				}

				*outfile << system;

				// Important: Cleanup
				outfile->close();
				delete outfile;
			}
		}
	}
	Log << "done." << endl;

	return 0;
}

