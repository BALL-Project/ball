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
#include <BALL/KERNEL/molecularInteractions.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/SCORING/FUNCTIONS/PLPScoring.h>
#include <BALL/SCORING/FUNCTIONS/MMScoring.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <set>
#include "version.h"

using namespace BALL;
using namespace std;

int main(int argc, char* argv[])
{
	CommandlineParser parpars("ConstraintsFinder", "find strongly interacting residues", VERSION, String(__DATE__), "Docking");
	parpars.registerMandatoryInputFile("rec", "receptor pdb-file");
	parpars.registerMandatoryInputFile("rl", "reference-ligand");
	parpars.registerOptionalInputFile(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME, "configuration file");
	parpars.registerOptionalOutputFile("o", "output configuration file");
	parpars.registerOptionalOutputFile("write_ini", "write ini-file w/ default parameters (and don't do anything else)");
	String man = "This tool searches protein residues with which the reference ligand interacts strongly.\nTherefore the interaction of the reference ligand to each residue is evaluated. Residues with a score worse (i.e. larger) than -2 are ignored. A maximum of 3 constraints are created for the most strongly interacting residues that met the above criterion.\n\nAs input we need:\n\
    * a file containing a protonated protein in pdb-format\n\
    * a file containing a reference ligand.\n\
      This reference ligand should be located in the binding pocket.\n\
      Supported formats are mol2, sdf or drf (DockResultFile, xml-based).\n\nOutput of this tool is a docking configuration file containing the created constraints. This file should in following pipeline steps be specified for grid precalculation and docking.";
	parpars.setToolManual(man);
	parpars.setSupportedFormats("rec","pdb");
	parpars.setSupportedFormats("rl",MolFileFactory::getSupportedFormats());
	parpars.setSupportedFormats(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME,"ini");
	parpars.setSupportedFormats("o","ini");
	parpars.setSupportedFormats("write_ini","ini");
	Options default_options;
	ScoringFunction::getDefaultOptions(default_options);
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
		Options* scoring_options = default_options.getSubcategory(ScoringFunction::SUBCATEGORY_NAME);

		scoring_options->setDefault("scoring_type", "MM");
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
	if (parpars.has(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME))
	{
		DockingAlgorithm::readOptionFile(parpars.get(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME), option, constraints, ref_ligand);
	}
	Options* option_category = option.getSubcategory(ScoringFunction::SUBCATEGORY_NAME);
	if (!option_category) option_category = &option;
	String scoring_type = option_category->setDefault("scoring_type", "MM");
	Options original_options = option;

	// Support for using one and the same config-file for grid precalculation and docking
	String precalc_nonbonded_cuttoff = option_category->get("nonbonded_cutoff_precalculation");
	String backup_nonbonded_cuttoff = option_category->get("cutoff_precalculation");
	if (precalc_nonbonded_cuttoff != "")
	{
		option_category->set("nonbonded_cutoff", precalc_nonbonded_cuttoff);
	}

	if (scoring_type != "GridedMM" && scoring_type != "GridedPLP" && scoring_type != "MM" && scoring_type != "PLP")
	{
		Log.error() << "[Error:] no valid scoring_type specified !" << endl;
		exit(1);
	}

	// all residues with an interaction energy worse than this value are ignored
	double THRESHOLD = -2;

	StructurePreparer* sp;
	String par_file = option_category->get("filename");
	if (par_file == "") par_file="Amber/amber96-docking.ini";
	if (scoring_type.hasSubstring("PLP"))
	{
		sp = new StructurePreparer("PLP");
	}
	else
	{
		sp = new StructurePreparer;
	}

	sp->prepare(ref_ligand, par_file);

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


	/// Evaluate binding of reference ligand to each protein residue
	ScoringFunction* scoring_function = 0;
	if (scoring_type == "GridedMM" || scoring_type == "MM")
	{
		scoring_function = new MMScoring(receptor, *ref_ligand, option);
	}
	else if (scoring_type == "GridedPLP" || scoring_type == "PLP")
	{
		scoring_function = new PLPScoring(receptor, *ref_ligand, option);
	}
	for (list < Constraint* > ::iterator it = constraints.begin(); it != constraints.end(); it++)
	{
		scoring_function->constraints.push_back(*it);
		(*it)->setScoringFunction(scoring_function);
	}

	Log.level(10) << endl << "-----------------------------------------" << endl;
	Log.level(10) << "Scores will be calculated as : " <<scoring_function->getEquation() << endl;
	Log.level(10) << "-----------------------------------------" << endl << endl;

	scoring_function->enableStoreInteractions();
	scoring_function->setLigand(*ref_ligand);
	scoring_function->update();
	scoring_function->updateScore();
	scoring_function->printResult(1);

	set<Residue*> selected_waters;

	list<String> interaction_types;
	interaction_types.push_back("ES");
	interaction_types.push_back("HB");
	interaction_types.push_back("vdW");

	map<double, PharmacophoreConstraint*> ph_map;
	bool first = 1;
/*	double max_neighbors = 0;
	double search_radius = 4;
	int no_atoms = 0;
	set<int> nm;
	for (AtomConstIterator lig_it = ref_ligand->beginAtom(); +lig_it; lig_it++, no_atoms++)
	{
		nm.insert(scoring_function->countNeighboringReceptorAtoms(&*lig_it, search_radius, true));
	}
	int i = 0; int max = (int)(no_atoms*0.3);
	no_atoms = 0;
	for (set < int > ::reverse_iterator it = nm.rbegin(); it != nm.rend() && i < max; it--, i++)
	{
		max_neighbors += *it;
		no_atoms++;
	}
	max_neighbors /= no_atoms; */

	for (ResidueIterator it = receptor.beginResidue(); +it; it++)
	{
		// Do not create PharmacophoreConstraint for glycine and alanine.
		// Since those residues are so small, good scores are most likely due to backbone interaction, which does usually not constitute specific binding.
		String name = it->getName();
		if (name.hasPrefix("HOH") || name.hasPrefix("WAT") || name.hasPrefix("GLY") || name.hasPrefix("ALA"))
		{
			continue;
		}

		double score_sum = 0;
		for (AtomConstIterator it2 = it->beginAtom(); +it2; it2++)
		{
			if (it2->interactions)
			{
				score_sum += it2->interactions->getInteractionEnergy();
			}
		}

		if (fabs(score_sum) > 1.5)
		{
			if (first)
			{
				Log.level(10) << "Strong interactions between reference-ligand and receptor residues:  ---- " << endl;
				first = 0;
			}
			Log.level(10) << it->getName() << " " << it->getID() << " : " << score_sum << " kJ/mol" << endl;
		}

		if (score_sum < THRESHOLD)
		{
		/*	map<double, const Atom*> nearest_atoms;
			for (AtomConstIterator lig_it = ref_ligand->beginAtom(); +lig_it; lig_it++)
			{
				if (!lig_it->interactions) continue;
				double min_dist = 9999;
				for (AtomConstIterator it2 = it->beginAtom(); +it2; it2++)
				{
					double dist = lig_it->getDistance(*it2);
					if (dist < min_dist) min_dist = dist;
				}
				nearest_atoms.insert(make_pair(min_dist, &*lig_it));
			}

			if (nearest_atoms.size() > 0)
			{
				double no_neighbors = 0;
				int no_atoms = 0;
				map<double, const Atom*>::iterator map_it = nearest_atoms.begin();
				for (Size i = 0; i < 10 && map_it != nearest_atoms.end(); map_it++, i++)
				{
					no_neighbors += scoring_function->countNeighboringReceptorAtoms(map_it->second, search_radius, true);
				//	cout<<"no neighbors of "<<map_it->second->getFullName()<<" : "<<scoring_function->countNeighboringReceptorAtoms(map_it->second, search_radius, true)<<"  dist="<<map_it->first<<endl;
					no_atoms++;
				}

				/// Do NOT create constraints for residues that are located outside out of the binding pocket
				no_neighbors /= no_atoms;
				if (no_neighbors > 0.65*max_neighbors)
				{*/
					PharmacophoreConstraint* ph = new PharmacophoreConstraint(&*it, interaction_types, score_sum/2, 10);
					ph->setName(it->getName()+it->getID());
					ph_map.insert(make_pair(score_sum, ph));
				/*	cout<<it->getFullName()<<" : "<<max_neighbors<<"  "<<no_neighbors<<endl;
				}
				else
				{
					cout<<"Not creating a constraint for residue '"<<it->getFullName()<<it->getID()<<"' because it is located on the protein surface  "<<max_neighbors<<"  "<<no_neighbors<<endl;
				}
			}*/
		}
	}

	Size map_id = 0;
	for (map < double, PharmacophoreConstraint* > ::iterator it = ph_map.begin(); it != ph_map.end(); it++, map_id++)
	{
		if (map_id < 3) // add constraints for the 3 strongest interactions
		{
			constraints.push_back(it->second);
		}
		else
		{
			delete it->second;
		}
	}


	/// Write output and clean everthing up
	if (precalc_nonbonded_cuttoff != "")
	{
		option_category->set("nonbonded_cutoff", backup_nonbonded_cuttoff);
	}

	String outfile = parpars.get("o");
	if (outfile != CommandlineParser::NOT_FOUND)
	{
		DockingAlgorithm::writeOptionFile(outfile, option, constraints);
	}

	for (list < Constraint* > ::iterator it = constraints.begin(); it != constraints.end(); it++)
	{
		delete *it;
	}
	delete scoring_function;
	delete sp;
	delete ref_ligand;
	return 0;
}
