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
	CommandlineParser par("SimpleRescorer", "rescore docking results", VERSION, String(__DATE__), "Rescoring");
	par.registerMandatoryInputFile("rec", "receptor pdb-file");
	par.registerMandatoryInputFile("rl", "reference-ligand");
	par.registerOptionalInputFile(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME, "configuration file");
	par.registerMandatoryInputFile("i", "compounds to be rescored");
	par.registerMandatoryOutputFile("o", "rescored compounds");
	par.registerOptionalOutputFile("write_ini", "write ini-file w/ default parameters (and don't do anything else)");
	par.registerOptionalStringParameter("function", "scoring function: 'MM', 'PLP' or 'PB'");
	par.registerFlag("rm", "remove input file when finished");

	String man = "This tool rescores docking output poses.\nA scoring function is used to evaluate the binding-free-energy of each compound. This is similar to the scoring done during docking; details depend on the config-file (if one is specified).\n\nAs input we need:\n\
    * a file containing a protonated protein in pdb-format\n\
    * a file containing a reference ligand. This reference ligand should be located in the binding pocket. Supported formats are mol2, sdf or drf (DockResultFile, xml-based).\n\
    * a file containing the compounds that are to be rescored. Supported formats are mol2, sdf or drf (DockResultFile, xml-based). Those compound should have been docked into the specified protein.\n\nOutput of this tool is a file in the same format as the input ligand file containing all compounds with scores obtained by rescoring in form of a property 're-score'.";
	par.setToolManual(man);
	list<String> slist;
	slist.push_back("MM");
	slist.push_back("PLP");
	slist.push_back("PB");
	par.setParameterRestrictions("function",slist);
	par.setSupportedFormats("rec","pdb");
	par.setSupportedFormats("rl",MolFileFactory::getSupportedFormats());
	par.setSupportedFormats(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME,"ini");
	par.setSupportedFormats("i",MolFileFactory::getSupportedFormats());
	par.setSupportedFormats("o","mol2,sdf,drf");
	par.setSupportedFormats("write_ini","ini");
	Options default_options;
	ScoringFunction::getDefaultOptions(default_options);
	par.registerAdvancedParameters(default_options);
	par.setSupportedFormats(ScoringFunction::SUBCATEGORY_NAME, "filename", "ini");
	par.parse(argc, argv);


	int status = Rescorer::runRescoring(par, true, false);

	if (status == 0 && par.has("rm"))
	{
		File::remove(par.get("i"));
	}
	return status;

}
