// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/commandlineParser.h>

#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/KERNEL/molecule.h>

#include <BALL/FORMAT/SDFile.h>

#include <openbabel/obconversion.h>
#include <openbabel/mol.h>
#include <openbabel/shared_ptr.h>
#include "testHeader.h"

using namespace OpenBabel;
using namespace BALL;
using namespace std;

shared_ptr<OBMol> GetMol(const std::string &filename)
{
  // Create the OBMol object.
  shared_ptr<OBMol> mol(new OBMol);

  // Create the OBConversion object.
  OBConversion conv;
  OBFormat *format = conv.FormatFromExt(filename.c_str());
  if (!format || !conv.SetInFormat(format)) {
    std::cout << "Could not find input format for file " << filename << std::endl;
    return mol;
  }

  // Open the file.
  std::ifstream ifs(filename.c_str());
  if (!ifs) {
    std::cout << "Could not open " << filename << " for reading." << std::endl;
    return mol;
  }
  // Read the molecule.
  if (!conv.Read(mol.get(), &ifs)) {
    std::cout << "Could not read molecule from file " << filename << std::endl;
    return mol;
  }

  return mol;
}
/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("Molecule2Fragments", "cut a molecule along its bonds, generating fragments", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF", OUTFILE, true);

	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setOutputFormatSource("o","i");

	String manual = "...just playing...";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);

	SDFile input(parpars.get("i"), ios::in);
	if (!input)
	{
		Log.error() << "Cannot open SDF input file " << parpars.get("i") << endl;
		return 2;
	}

	Molecule ball_mol;
	input >> ball_mol;
	input.close();


	// write the optimized structure to a file whose
	// name is given as the second command line argument
	String outfile_name = String(parpars.get("o"));
	SDFile outfile(outfile_name, ios::out);
	outfile << ball_mol;
	outfile.close();

	Log << "wrote file " << outfile_name << endl;
}


