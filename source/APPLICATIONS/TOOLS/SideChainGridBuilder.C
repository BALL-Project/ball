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
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/rotamerLibrary.h>
#include <BALL/STRUCTURE/peptideBuilder.h>
#include "version.h"


using namespace BALL;
using namespace std;

GridBasedScoring* createScoringFunction(AtomContainer* receptor, AtomContainer* ref_ligand, String scoring_type, list<Constraint*>* constraints, Options* option)
{
	GridBasedScoring* gbs = 0;
	if (scoring_type == "GridedMM")
	{
		gbs = new GridedMM(*receptor, *ref_ligand, *option);
	}
	else if (scoring_type == "GridedPLP")
	{
		gbs = new GridedPLP(*receptor, *ref_ligand, *option);
	}
	for (list < Constraint* > ::iterator it = constraints->begin(); it != constraints->end(); it++)
	{
		gbs->constraints.push_back(*it);
		(*it)->setScoringFunction(gbs);
	}
	return gbs;
}


int main(int argc, char* argv[])
{
	CommandlineParser parpars("SideChainGridBuilder", "build side chain grid", VERSION, String(__DATE__), "Docking");
	parpars.registerMandatoryInputFile("param", "parameter file");
	parpars.registerMandatoryStringParameter("d", "output directory");
	parpars.setSupportedFormats("param", "ini");
	parpars.setToolManual("This tool precalculates a side chain grid.");

	parpars.parse(argc, argv);

	//Log.setMinLevel(cout, 10);

	Options option;
	list<Constraint*> constraints;

	if (parpars.get("param") == CommandlineParser::NOT_FOUND)
	{
		cout<<"[Error:] ini file must be given" << std::endl;
		exit (1);
	}

	String inifile = parpars.get("param");
	DockingAlgorithm::readOptionFile(inifile, option, constraints);
	String scoring_type = option.get("scoring_type");
	String grid_file = option.get("grid_file");

	// Support for using one and the same config-file for grid precalculation and docking
	String precalc_nonbonded_cuttoff = option.get("nonbonded_cutoff_precalculation");
	if (precalc_nonbonded_cuttoff != "")
	{
		option.set("nonbonded_cutoff", precalc_nonbonded_cuttoff);
	}

	if (scoring_type == "")
	{
		cout<<"[Error:] scoring_type must be specified in the config-file!"<<endl;
		exit(1);
	}
	if (scoring_type != "GridedMM" && scoring_type != "GridedPLP")
	{
		cout<<"[Error:] specified scoring_type is not grid-based, so no grid can be build for it!"<<endl;
		exit(1);
	}
	String par_file = option.get("filename");
	if (par_file == "" && !scoring_type.hasSubstring("GH"))
	{
		cout<<"[Error:] 'filename' for force-field parameter file must be specified in the config-file  !"<<endl;
		return(1);
	}
	if (grid_file == "")
	{
		cout<<"'grid_file' must be specified in the config-file  !"<<endl;
		return(1);
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

	String at = option.get("atom_types");
	set<String> types;
	if (!use_PLP && at != "")
	{
		Size no = at.countFields(", ");
		for (Size i = 0; i < no; i++)
		{
			String type_i = at.getField(i, ", ");
			type_i.trim();
			types.insert(type_i);
		}
	}

	// Angles are irrelevant, since we construct only single amino acids
	Angle angle(0);

	if (parpars.get("d") == CommandlineParser::NOT_FOUND)
	{
		cout<<"[Error:] out directory must be given" << std::endl;
		exit (1);
	}

	String prefix = parpars.get("d");
	prefix += "/";

	FragmentDB frag_db("fragments/Fragments.db");
	RotamerLibrary rotamer_lib("rotamers/bbind02.May.lib", frag_db);
	String one_letter_codes = "ARNDCQEGHILKMFPSTWYV";

	for (Size i = 0; i < 20; i++)
	{
		Peptides::PeptideBuilder pb(one_letter_codes[i], angle, angle, angle);
		pb.setFragmentDB(&frag_db);
		Protein* protein_template = pb.construct();
		sp->prepare(protein_template, par_file);

		Residue* residue = protein_template->getResidue(0);
		ResidueRotamerSet* rotamer_set = rotamer_lib.getRotamerSet(*residue);

		if (!rotamer_set) // GLY and ALA have no rotamers
		{
			cout<<"No rotamers for "<<residue->getName()<<" found."<<endl;
			continue;
		}

		// Remove atoms that are not part of the residue templates
		for (AtomIterator atom_it = protein_template->beginAtom(); +atom_it; atom_it++)
		{
			const String& name = atom_it->getName();
			if (name == "1H" || name == "2H" || name == "3H" || name == "OXT")
			{
				atom_it->select();
			}
		}
		protein_template->removeSelected();

		// Do for each rotamer of the current amino acid
		for (ResidueRotamerSet::ConstIterator it = rotamer_set->begin(); it != rotamer_set->end(); it++)
		{
			rotamer_set->setRotamer(*residue, *it);
			Protein protein = *protein_template; // make a copy

			/// Remove backbone atoms!!
			/// Grids should only contain contributions for side-chains!
			for (AtomIterator atom_it = protein.beginAtom(); +atom_it; atom_it++)
			{
				const String& name = atom_it->getName();
				if (name == "N" || name == "O" || name == "CA" || name == "C"
				||name == "HA")
				{
					atom_it->select();
				}
			}
			protein.removeSelected();


			GridBasedScoring* gbs = createScoringFunction(&protein, &protein, scoring_type, &constraints, &option);
			gbs->setAtomTypeNames(types);

			String name = residue->getName()+"_";
			name += String((int)it->chi1)+"_";
			name += String((int)it->chi2)+"_";
			name += String((int)it->chi3)+"_";
			name += String((int)it->chi4);

			cout<<endl<<"---- Will now precalculate grids for "<<name<<" ...  ------------"<<endl<<flush;

			// Make sure that atoms lying outside of the grids for flexible side-chains will not be penalized during docking/scoring.
			vector<ScoreGridSet*>* gridsets = gbs->getScoreGridSets();
			for (Size i = 0; i < gridsets->size(); i++)
			{
				gridsets->at(i)->setParameters(0, 0, 0);
			}

			gbs->precalculateGrids();

			gbs->saveGridSetsToFile(prefix+name+".bngrd", name);
			PDBFile out(prefix+name+".pdb", ios::out);
			out << *protein_template; // include backbone atoms into pdb-file, because they will be needed for mapping later
			out.close();
			delete gbs;
		}
		delete protein_template;
	}


	for (list < Constraint* > ::iterator it = constraints.begin(); it != constraints.end(); it++)
	{
		delete *it;
	}
	delete sp;
	return 0;
}
