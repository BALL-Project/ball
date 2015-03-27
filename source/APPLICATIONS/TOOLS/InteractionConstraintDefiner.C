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
	CommandlineParser parpars("InteractionConstraintDefiner", "define interaction constraint", VERSION, String(__DATE__), "Docking");
	parpars.registerMandatoryStringParameter("res", "residue ID");
	parpars.registerMandatoryDoubleParameter("s", "desired minimal interation (score) between ligand and specified residue(s)");
	parpars.registerMandatoryDoubleParameter("p", "penalty value");
	parpars.registerMandatoryOutputFile("o", "output configuration file");
	parpars.registerOptionalInputFile(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME, "input configuration file");
	String man = "This tool allows to define interaction constraints for docking or scoring.\n\nThe constraint to be created will enforce a specified minimal interaction between ligands and the specified residue(s) of the receptor. Please specify residue IDs in the following manner: <chain-ID>:<residue-ID>, e.g. A:57. If you want to use more than one residue, separate their IDs by commas, e.g. A:57,B:17.\n\nOutput of this tool is a ini-file that contains the desired interaction constraint.";
	parpars.setToolManual(man);
	parpars.setSupportedFormats(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME,"ini");
	parpars.setSupportedFormats("o","ini");
	parpars.parse(argc, argv);

	Log.setMinLevel(cout, 10);

	Options options;
	list<Constraint*> constraints;
	if (parpars.has(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME))
	{
		DockingAlgorithm::readOptionFile(parpars.get(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME), options, constraints);
	}

	vector<String> residue_IDs;
	String res = parpars.get("res");
	res.split(residue_IDs,",");
	list<String> interaction_types;
	interaction_types.push_back("vdW");
	interaction_types.push_back("HB");
	interaction_types.push_back("ES");
	PharmacophoreConstraint* phC = new PharmacophoreConstraint(residue_IDs, interaction_types, parpars.get("s").toDouble(), parpars.get("p").toDouble());
	String name = "InteractionConstraintDefiner on ";
	name += parpars.get("res");
	phC->setName(name);
	constraints.push_back(phC);

	/// Write output and clean everthing up
	DockingAlgorithm::writeOptionFile(parpars.get("o"), options, constraints);
	for (list<Constraint*>::iterator it = constraints.begin(); it != constraints.end(); it++)
	{
		delete *it;
	}
	return 0;
}
