// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/DOCKING/COMMON/structurePreparer.h>
#include <BALL/DOCKING/COMMON/dockingAlgorithm.h>
#include <BALL/COMMON/exception.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/SCORING/COMMON/gridBasedScoring.h>
#include <BALL/SCORING/FUNCTIONS/gridedPLP.h>
#include <BALL/SCORING/FUNCTIONS/gridedMM.h>
#include "version.h"


using namespace BALL;
using namespace std;

int main(int argc, char* argv[])
{
	CommandlineParser parpars("GridBuilder", "create score-grids for docking", VERSION, String(__DATE__), "Docking");
	parpars.registerMandatoryInputFile("rec", "receptor pdb-file");
	parpars.registerMandatoryInputFile("rl", "reference-ligand");
	parpars.registerMandatoryOutputFile("grd", "ScoreGrid file");
	parpars.registerOptionalInputFile("pocket", "configuration file");
	parpars.registerOptionalOutputFile("write_ini", "write ini-file w/ default parameters (and don't do anything else)");

	String man = "This tool precalculates a score-grid for a binding pocket of a given receptor.\n\nAs input we need:\n\
    * a file containing a protonated protein in pdb-format\n\
    * a file containing a reference ligand.\n\
      This reference ligand should be located in the binding pocket, \n\
      so that a grid can be precalculated around it.\n\
      Supported formats are mol2, sdf or drf (DockResultFile, xml-based).\n\nOutput of this tool is a file containing the score-grids that can be used by docking-/scoring-tools (e.g. IMeedyDock).";
	parpars.setToolManual(man);
	parpars.setSupportedFormats("rec","pdb");
	parpars.setSupportedFormats("rl",MolFileFactory::getSupportedFormats());
	parpars.setSupportedFormats("pocket","ini");
	parpars.setSupportedFormats("write_ini","ini");
	parpars.setSupportedFormats("grd","grd.gz,grd,bngrd.gz,bngrd");

	Options default_options;
	ScoringFunction::getDefaultOptions(default_options);
	Options* scoring_options = default_options.getSubcategory(ScoringFunction::SUBCATEGORY_NAME);
	scoring_options->setDefault("atom_types", "C, H, N, O, P, S, Cl, F, I");
	scoring_options->addParameterDescription("atom_types", "elements for which grids should be precalculated", STRING);
	parpars.registerAdvancedParameters(default_options);
	parpars.setSupportedFormats(ScoringFunction::SUBCATEGORY_NAME, "filename", "ini");

	parpars.parse(argc, argv);

	String default_inifile = parpars.get("write_ini");
	if (default_inifile != CommandlineParser::NOT_FOUND)
	{
		// if ini-file already exists, read its entries first
		list<Constraint*> clist;
		if (ifstream(default_inifile.c_str()))
		{
			DockingAlgorithm::readOptionFile(default_inifile, default_options, clist);
		}

		scoring_options->setDefault("scoring_type", "GridedMM");
		scoring_options->setDefault("scoregrid_resolution", 0.5);
		scoring_options->setDefault("nonbonded_cutoff_precalculation", scoring_options->get("nonbonded_cutoff"));
		scoring_options->set("nonbonded_cutoff", 3.0);
		DockingAlgorithm::writeOptionFile(parpars.get("write_ini"), default_options, clist);
		Log << "Ini-file w/ default values has been written to file '"<<default_inifile<<"'. Goodbye!"<<endl;
		return 0;
	}

	Log.setMinLevel(cout, 10);

	GenericMolFile* ref_ligand_file = MolFileFactory::open(parpars.get("rl"));
	Molecule* ref_ligand = ref_ligand_file->read();
	ref_ligand_file->close();
	delete ref_ligand_file;

	Options option;
	parpars.copyAdvancedParametersToOptions(option);
	list<Constraint*> constraints;
	if (parpars.get("pocket") != CommandlineParser::NOT_FOUND)
	{
		DockingAlgorithm::readOptionFile(parpars.get("pocket"), option, constraints, ref_ligand);
	}
	Options* option_category = option.getSubcategory(ScoringFunction::SUBCATEGORY_NAME);
	if (!option_category) option_category = &option;
	String scoring_type = option_category->setDefault("scoring_type", "GridedMM");
	String grid_file = parpars.get("grd");

	// Support for using one and the same config-file for grid precalculation and docking
	String precalc_nonbonded_cuttoff = option_category->get("nonbonded_cutoff_precalculation");
	if (precalc_nonbonded_cuttoff != "")
	{
		option_category->set("nonbonded_cutoff", precalc_nonbonded_cuttoff);
	}

	StructurePreparer* sp;
	bool use_PLP = 0;
	if (scoring_type.hasSubstring("PLP"))
	{
		use_PLP = 1;
		sp = new StructurePreparer("PLP");
	}
	else
	{
		use_PLP = 0;
		sp = new StructurePreparer;
	}

	String par_file = option_category->get("filename");
	if (par_file == "") par_file="Amber/amber96-docking.ini";

	System receptor;
	GenericMolFile* input = MolFileFactory::open(parpars.get("rec"));
	if (!input)
	{
		cerr<<"Format of receptor-file not supported!"<<endl;
		return 1;
	}
	*input >> receptor;
	sp->prepare(&receptor, par_file);

	delete input;

	sp->prepare(ref_ligand, par_file);

	GridBasedScoring* gbs = 0;
	if (scoring_type == "GridedMM" || scoring_type == "MM")
	{
		gbs = new GridedMM(receptor, *ref_ligand, option);
	}
	else if (scoring_type == "GridedPLP" || scoring_type == "PLP")
	{
		gbs = new GridedPLP(receptor, *ref_ligand, option);
	}
	else
	{
		Log.error() << "[Error:] Scoring function '" << scoring_type << "' unknown/unsupported!" << endl;
		return 1;
	}
	for (list < Constraint* > ::iterator it = constraints.begin(); it != constraints.end(); it++)
	{
		gbs->constraints.push_back(*it);
		(*it)->setScoringFunction(gbs);
	}

	Log.level(10)<<endl<<"-----------------------------------------"<<endl;
	Log.level(10)<<"Scores will be calculated as : "<<gbs->getEquation()<<endl;
	Log.level(10)<<"-----------------------------------------"<<endl<<endl;


	String at = option_category->get("atom_types");
	if (!use_PLP && at != "")
	{
		set<String> types;
		Size no = at.countFields(", ");
		for (Size i = 0; i < no; i++)
		{
			String type_i = at.getField(i, ", ");
			type_i.trim();
			types.insert(type_i);
		}
		gbs->setAtomTypeNames(types);
	}

	gbs->precalculateGrids();
	gbs->saveGridSetsToFile(grid_file, "GridSets for receptor '"+parpars.get("rec")+"'");


	for (list < Constraint* > ::iterator it = constraints.begin(); it != constraints.end(); it++)
	{
		delete *it;
	}
	delete gbs;
	delete sp;
	delete ref_ligand;
	return 0;
}
