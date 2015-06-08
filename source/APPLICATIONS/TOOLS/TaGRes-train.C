// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/DOCKING/COMMON/dockingAlgorithm.h>
#include <BALL/DOCKING/COMMON/structurePreparer.h>
#include <BALL/SCORING/FUNCTIONS/gridedMM.h>
#include <BALL/SCORING/FUNCTIONS/MMScoring.h>
#include <BALL/SCORING/FUNCTIONS/PBScoring.h>
#include <BALL/SCORING/FUNCTIONS/gridedPLP.h>
#include <BALL/SCORING/FUNCTIONS/PLPScoring.h>
#include <BALL/SCORING/FUNCTIONS/rescoring3D.h>
#include <BALL/SCORING/FUNCTIONS/rescoring4D.h>
#include <BALL/SCORING/FUNCTIONS/rescoring1D.h>
#include <BALL/SCORING/COMMON/rescorer.h>
#include "version.h"

using namespace BALL;

int main(int argc, char* argv[])
{
	CommandlineParser par("TaGResTrain", "Target-specific Grid-Rescoring, training", VERSION, String(__DATE__), "Rescoring");
	par.registerMandatoryInputFile("rec", "receptor pdb-file");
	par.registerMandatoryInputFile("rl", "reference-ligand");
	par.registerOptionalInputFile(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME, "configuration file");
	par.registerMandatoryInputFile("i", "training compound data set");
	par.registerMandatoryOutputFile("o", "model file");
	par.registerOptionalOutputFile("write_ini", "write ini-file w/ default parameters (and don't do anything else)");
	par.registerMandatoryStringParameter("method", "rescoring type: 'Rescoring3D' or 'Rescoring4D', or 'Rescoring1D'");
	par.registerOptionalStringParameter("function", "scoring function: 'MM' or 'PLP'");
	par.registerMandatoryStringParameter("exp", "property-tag name containing experimentally determined binding-free-energies");

	String man = "This tool generates a model for Target-specific Grid-Rescoring (TaGRes).\nAs input we need:\n\n\
    * a file containing a protonated protein in pdb-format\n\
    * a file containing a reference ligand. This reference ligand should be located in the binding pocket. Supported formats are mol2, sdf or drf (DockResultFile, xml-based).\n\
    * a file containing a training data set, i.e. compounds whose binding-free-energy to the specified target is known and annotated in this file. Those compounds should have been docked into the specified protein.\n\nA scoring function is applied and an interaction-grid is thereby generated for each input compound. Together with the known binding-free-energy, those grids are used to automatically search for the best linear or non-linear regression model that can approximate the binding-free-energy. After this model has been generated, you can pass it to the tool TaGRes and rescore (different) compounds with it.\n\nThe output of TaGRes-train is a file that contains the generated regression model. However, if no model with suitable prediction quality was found, an error will be shown and no model-file will be written.";
	par.setToolManual(man);
	list<String> slist;
	slist.push_back("Rescoring3D");
	slist.push_back("Rescoring4D");
	slist.push_back("Rescoring1D");
	par.setParameterRestrictions("method",slist);
	slist.clear();
	slist.push_back("MM");
	slist.push_back("PLP");
	par.setParameterRestrictions("function",slist);
	par.setSupportedFormats("rec","pdb");
	par.setSupportedFormats("rl",MolFileFactory::getSupportedFormats());
	par.setSupportedFormats(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME,"ini");
	par.setSupportedFormats("i",MolFileFactory::getSupportedFormats());
	par.setSupportedFormats("o","mod");
	par.setSupportedFormats("write_ini","ini");
	Options default_options;
	ScoringFunction::getDefaultOptions(default_options);
	par.registerAdvancedParameters(default_options);
	par.setSupportedFormats(ScoringFunction::SUBCATEGORY_NAME, "filename", "ini");
	par.parse(argc, argv);


	return Rescorer::runRescoring(par, false, true);

}
