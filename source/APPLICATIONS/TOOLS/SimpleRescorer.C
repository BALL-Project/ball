/* Rescorer.C
*
* Copyright (C) 2011 Marcel Schumann
*
* This program free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or (at
* your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/FORMAT/commandlineParser.h>
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
	par.registerParameter("rec", "receptor pdb-file", INFILE, true);
	par.registerParameter("rl", "reference-ligand", INFILE, true);
	par.registerParameter("ini", "configuration file", INFILE);
	par.registerParameter("i", "compounds to be rescored", INFILE, true);
	par.registerParameter("o", "rescored compounds", OUTFILE, true);
	par.registerParameter("write_ini", "write ini-file w/ default parameters (and don't do anything else)", OUTFILE);
	par.registerParameter("function", "scoring function: 'MM', 'PLP' or 'PB'", STRING);
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
	par.setSupportedFormats("ini","ini");
	par.setSupportedFormats("i",MolFileFactory::getSupportedFormats());
	par.setSupportedFormats("o","mol2,sdf,drf");
	par.setOutputFormatSource("o","i");
	Options default_options;
	ScoringFunction::getDefaultOptions(default_options);
	par.registerAdvancedParameters(default_options);
	par.setSupportedFormats("filename","ini");
	par.parse(argc, argv);


	int status = Rescorer::runRescoring(par, true, false);

	if (status == 0 && par.has("rm"))
	{
		File::remove(par.get("i"));
	}
	return status;

}
