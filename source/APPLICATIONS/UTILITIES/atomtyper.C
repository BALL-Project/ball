// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// A small program for using the GAFF atomtyper

#include <BALL/KERNEL/system.h>
// #include <BALL/FORMAT/HINFile.h>
#include <BALL/MOLMEC/AMBER/GAFFTypeProcessor.h>
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	if ((argc < 3 || argc > 4))
	{
		Log << "Usage:" << argv[0] << " <infile> <outfile> [GAFFTypes.dat]" << endl;
		return 1;
	}

	Log << "Loading " << argv[1] << "..." << endl;
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
	Log << "done." << endl;

	AssignBondOrderProcessor abp;
	system.apply(abp);

	Options options;
	options[GAFFTypeProcessor::Option::ATOMTYPE_FILENAME] = "atomtyping/GAFFTypes.dat";

	if (argc > 3)
		options[GAFFTypeProcessor::Option::ATOMTYPE_FILENAME] = String("Amber/")+argv[3];

	GAFFTypeProcessor gt(options);
	system.apply(gt);

	for (AtomIterator at_it = system.beginAtom(); +at_it; ++at_it)
		std::cout << "atom name: " << at_it->getName() << " atomtype  " << at_it->getProperty("atomtype").getString() << std::endl;

	Log << "Writing " << argv[2] << "..." << endl;
	GenericMolFile* outfile = MolFileFactory::open(argv[2], std::ios::out);

	if (!outfile)
	{
		std::cerr << "Could not determine filetype, aborting" << std::endl;
		exit(-1);
	}

	if (!*outfile)
	{
		std::cerr << "Invalid file, aborting" << std::endl;
		exit(-1);
	}

	*outfile << system;
	Log << "done." << endl;

	// Important: Cleanup
	outfile->close();
	delete outfile;
}
