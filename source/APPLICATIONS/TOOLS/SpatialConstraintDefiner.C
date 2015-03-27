// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/DOCKING/COMMON/constraints.h>
#include <BALL/DOCKING/COMMON/dockingAlgorithm.h>
#include <BALL/DATATYPE/options.h>
#include "version.h"

using namespace BALL;
using namespace std;

int main(int argc, char* argv[])
{
	CommandlineParser parpars("SpatialConstraintDefiner", "define spatial constraint", VERSION, String(__DATE__), "Docking");
	parpars.registerOptionalInputFile(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME, "input configuration file", INFILE);
	parpars.registerMandatoryInputFile("i", "input molecule file");
	parpars.registerMandatoryOutputFile("o", "output configuration file");
	parpars.registerMandatoryStringParameter("type", "'fraction' or 'number' of compound atoms");
	parpars.registerMandatoryDoubleParameter("n", "desired number/fraction of atoms in spatial area");
	parpars.registerMandatoryDoubleParameter("p", "penalty value");
	String man = "This tool allows to define spatial constraints for docking or scoring.\n\nFor convenience, we use a molecule file as input and generate a boundary box around the contained compound. This molecule can therefore for example contain the reference ligand (obtained from a co-crystal structure), or a docked compound, or just a set of dummy atoms used to manually define the boundaries of the desired spatial constraint.\nFurthermore, you need to specify how many atoms of the compound to be docked (or scored) should be located inside the spatial area. You can either specify a number of atoms or a fraction of molecule atoms for this.\n\nOutput of this tool is a ini-file that contains the desired spatial constraint.";
	parpars.setToolManual(man);
	parpars.setSupportedFormats("i",MolFileFactory::getSupportedFormats());
	parpars.setSupportedFormats(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME,"ini");
	parpars.setSupportedFormats("o","ini");
	list<String> slist;
	slist.push_back("fraction");
	slist.push_back("number");
	parpars.setParameterRestrictions("type",slist);
	parpars.parse(argc, argv);

	Log.setMinLevel(cout, 10);

	GenericMolFile* ligand_file = MolFileFactory::open(parpars.get("i"));
	Molecule* molecule = ligand_file->read();
	ligand_file->close();
	delete ligand_file;

	Options options;
	list<Constraint*> constraints;
	if (parpars.has(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME))
	{
		DockingAlgorithm::readOptionFile(parpars.get(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME), options, constraints, molecule);
	}

	bool atom_fraction = false;
	if (parpars.get("type") == "fraction") atom_fraction = true;

	ReferenceArea* rf = new ReferenceArea(molecule, atom_fraction, parpars.get("n").toDouble(), parpars.get("p").toDouble());
	String name = "SpatialConstraintDefiner on ";
	name += parpars.get("i");
	rf->setName(name);
	constraints.push_back(rf);

	/// Write output and clean everthing up
	DockingAlgorithm::writeOptionFile(parpars.get("o"), options, constraints);
	for (list<Constraint*>::iterator it = constraints.begin(); it != constraints.end(); it++)
	{
		delete *it;
	}
	delete molecule;
	return 0;
}
