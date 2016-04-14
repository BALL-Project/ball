#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/SCORING/COMMON/rescorer.h>
#include <BALL/DOCKING/COMMON/dockingAlgorithm.h>
#include <BALL/SCORING/FUNCTIONS/gridedMM.h>
#include <BALL/SCORING/FUNCTIONS/MMScoring.h>
#include <BALL/SCORING/FUNCTIONS/PBScoring.h>
#include <BALL/SCORING/FUNCTIONS/gridedPLP.h>
#include <BALL/SCORING/FUNCTIONS/PLPScoring.h>
#include <BALL/SCORING/FUNCTIONS/rescoring3D.h>
#include <BALL/SCORING/FUNCTIONS/rescoring4D.h>
#include <BALL/SCORING/FUNCTIONS/rescoring1D.h>

using namespace std;

namespace BALL
{
	int Rescorer::runRescoring(CommandlineParser& par, bool simple_rescoring, bool train)
	{
		// just to make sure ...
		if (simple_rescoring) train = false;


		/** If desired, write ini-file with default parameters and abort */

		String default_inifile = par.get("write_ini");
		if (default_inifile != CommandlineParser::NOT_FOUND)
		{
			// if ini-file already exists, read its entries first
			Options default_options;
			ScoringFunction::getDefaultOptions(default_options);
			list<Constraint*> clist;
			if (ifstream(default_inifile.c_str()))
			{
				DockingAlgorithm::readOptionFile(default_inifile, default_options, clist);
			}

			Options* scoring_options = default_options.getSubcategory(ScoringFunction::SUBCATEGORY_NAME);
			scoring_options->setDefault("scoring_type", "MM");
			scoring_options->setDefault("nonbonded_cutoff_precalculation", scoring_options->get("nonbonded_cutoff"));
			scoring_options->set("nonbonded_cutoff", 3.0);
			DockingAlgorithm::writeOptionFile(par.get("write_ini"), default_options, clist);
			Log << "Ini-file w/ default values has been written to file '"<<default_inifile<<"'. Goodbye!"<<endl;
			return 0;
		}


		/** Fetch information about specification of desired rescoring approach */

		String method = "";
		String free_energy_label = "";
		String model_file = "";
		String scoring_type = "MM";
		String grid_file = "";
		Options option;
		par.copyAdvancedParametersToOptions(option);
		Options* option_category = option.getSubcategory(ScoringFunction::SUBCATEGORY_NAME);
		if (!option_category) option_category = &option;

		list<Constraint*> constraints;
		if (par.get("ini") != CommandlineParser::NOT_FOUND)
		{
			DockingAlgorithm::readOptionFile(par.get("ini"), option, constraints);
			scoring_type = option_category->setDefault("scoring_function", "MM");
			grid_file = option_category->setDefault("grid_file", "");
		}
		// Overload ini-file settings with values taken from command-line (if any)
		if (par.get("function") != CommandlineParser::NOT_FOUND)
		{
			scoring_type = par.get("function");
		}

		method = par.get("method");
		free_energy_label = par.get("exp");
		if (par.get("mod") != CommandlineParser::NOT_FOUND)
		{
			model_file = par.get("mod");
		}

		if (!simple_rescoring && method != "Rescoring3D" && method != "Rescoring4D" && method != "Rescoring1D")
		{
			cerr << "[Error:] Type of desired rescoring method unknown: available are 'Rescoring3D', 'Rescoring4D' and 'Rescoring1D'." << endl;
			exit(1);
		}


		/** Setup StructurePreparer, Rescoring and ScoringFunction  */

		//Log.setMinLevel(cout, 39);

		GenericMolFile* ref_ligand_file = MolFileFactory::open(par.get("rl"));
		Molecule* ref_ligand = ref_ligand_file->read();
		ref_ligand_file->close();
		delete ref_ligand_file;

		StructurePreparer* sp;
		if (scoring_type.hasSubstring("PLP"))
		{
			sp = new StructurePreparer("PLP");
		}
		else
		{
			sp = new StructurePreparer;
		}

		String par_file = option_category->get("filename");
		if (par_file == "") par_file="Amber/amber96-docking.ini";
		System receptor;
		GenericMolFile* receptor_file = MolFileFactory::open(par.get("rec"));
		if (!receptor_file)
		{
			cerr<<"Format of receptor-file not supported!"<<endl;
			return 1;
		}

		*receptor_file >> receptor;
		delete receptor_file;
		sp->prepare(&receptor, par_file);
		sp->prepare(ref_ligand, par_file);

		ScoringFunction* scoring_function;
		if (scoring_type == "MM" || scoring_type == "PB")
		{
			// Support for using one and the same config-file for docking and rescoring
			String precalc_nonbonded_cuttoff = option_category->get("nonbonded_cutoff_precalculation");
			if (precalc_nonbonded_cuttoff != "")
			{
				option_category->set("nonbonded_cutoff", precalc_nonbonded_cuttoff);
			}
		}
		if (scoring_type == "MM")
		{
			scoring_function = new MMScoring(receptor, *ref_ligand, option);
		}
		else if (scoring_type == "GridedMM")
		{
			scoring_function = new GridedMM(receptor, *ref_ligand, option);
		}
		else if (scoring_type == "GridedPLP")
		{
			scoring_function = new GridedPLP(receptor, *ref_ligand, option);
		}
		else if (scoring_type == "PLP")
		{
			scoring_function = new PLPScoring(receptor, *ref_ligand, option);
		}
		else if (scoring_type == "PB")
		{
			scoring_function = new PBScoring(receptor, *ref_ligand, option);
		}
		else
		{
			String mess="ScoringFunction type \'"+scoring_type+"\' unknown/unsupported!";
			cerr<<"[Error:] "<<mess<<endl;
			return 1;
		}

		GridBasedScoring* gbs = dynamic_cast<GridBasedScoring*>(scoring_function);
		for (list < Constraint* > ::iterator it = constraints.begin(); it != constraints.end(); it++)
		{
			scoring_function->constraints.push_back(*it);
			(*it)->setScoringFunction(scoring_function);
		}

		cout<<endl<<"-----------------------------------------"<<endl;
		cout<<"Scores will be calculated as : "<<scoring_function->getEquation()<<endl;
		cout<<"-----------------------------------------"<<endl<<endl;

		if (gbs != NULL)
		{
			gbs->replaceGridSetFromFile(grid_file);
		}

		Rescoring* rescoring = 0;

		if (!simple_rescoring)
		{
			if (!train && free_energy_label == "")
			{
				cerr<<"[Error:] free-energy label must be specified !"<<endl;
				return 1;
			}
			if (method == "Rescoring3D")
			{
				rescoring = new Rescoring3D(receptor, *ref_ligand, option, free_energy_label, scoring_function);
			}
			else if (method == "Rescoring4D")
			{
				rescoring = new Rescoring4D(receptor, *ref_ligand, option, free_energy_label, scoring_function);
			}
			else if (method == "Rescoring1D")
			{
				rescoring = new Rescoring1D(receptor, *ref_ligand, option, free_energy_label, scoring_function);
			}
			else
			{
				cerr<<"[Error:] Rescoring-method unknown !"<<endl;
				return 1;
			}
			if (!train)
			{
				rescoring->loadModel(model_file);
			}
			scoring_function->enableStoreInteractions();
		}

		scoring_function->setLigand(*ref_ligand);
		scoring_function->update();
		scoring_function->updateScore();
		Log<<"====== Reference ligand ============"<<endl;
		scoring_function->printResult();


		/** If top fraction of docking results should not be rescored, then fetch scores and compute threshold for this fraction */

		bool ignore_top = false;
		double ignore_top_fraction = 0.0;
		if (par.has("tf"))
		{
			ignore_top_fraction = par.get("tf").toDouble();
			if (ignore_top_fraction < 1e-14 || ignore_top_fraction < 0 || ignore_top_fraction > 1)
			{
				ignore_top_fraction = 0.0;
				ignore_top = false;
			}
			else
			{
				ignore_top = true;
			}
		}
		double min_dock_score = -1e100;
		list<pair<double, bool> > rescore_list;
		if (ignore_top)
		{
			set<double> scores;
			GenericMolFile* input = MolFileFactory::open(par.get("i"));
			for (Molecule* mol = input->read(); mol; delete mol, mol = input->read())
			{
				if (mol->hasProperty("score"))
				{
					scores.insert(mol->getProperty("score").toString().toDouble());
				}
			}
			Size i = 0;
			Size max = scores.size()*ignore_top_fraction;
			set<double>::iterator s_it = scores.begin();
			for (; i < max; s_it++)
			{
				i++;
			}
			min_dock_score = *s_it;
			delete input;
		}


		/** Now, Rescore entire sd-/mol2-file   */

		double threshold = option.setDefaultReal("output_score_threshold", 1e100);

		GenericMolFile* input = MolFileFactory::open(par.get("i"));
		GenericMolFile* output = 0;

		if (simple_rescoring || !train)
		{
			output = MolFileFactory::open(par.get("o"), ios::out, input);

			DockResultFile* drf_output = dynamic_cast<DockResultFile*>(output);
			if (drf_output)
			{
				String dummy = "0";
				BALL::Result::Method method = Result::getMethod(3);
				String description = "";
				description = rescoring->getName()+"+"+scoring_function->getName();
				drf_output->setOutputParameters(method, "re-score", dummy, description);
			}
		}

		// Do the actual work ..
		processMultiMoleculeFile(scoring_function, sp, par_file, rescoring, train, min_dock_score, "score", input, output, threshold, ignore_top);

		if(!simple_rescoring)
		{
			if (train)
			{
				rescoring->recalibrate();
				rescoring->saveModel(par.get("o"));
			}
			else
			{
				double correlation, q2, std_err;
				rescoring->calculateQuality(correlation, q2, std_err);
				cout<<"Correlation = "<<String(correlation)<<endl;
				cout<<"Q2="<<String(q2)<<endl;
				cout<<"Standard error="<<String(std_err)<<endl;
			}
		}

		for (list<Constraint*>::iterator it = constraints.begin(); it != constraints.end(); it++)
		{
			delete *it;
		}

		delete rescoring;
		delete sp;
		delete ref_ligand;

		return 0;
	}

	void Rescorer::processMultiMoleculeFile(ScoringFunction* scoring_function, StructurePreparer* sp, String par_file, Rescoring* rescoring, bool train, double min_dock_score, String dock_score_label, GenericMolFile* input, GenericMolFile* output, double output_score_threshold, bool ignore_top)
	{
		list<pair<double, bool> > rescore_list;
		double min_rescore = 1e12;
		int i = 1;

		for (Molecule* mol = input->read(); mol; mol = input->read(), i++)
		{
			String name = mol->getName();
			Log<<"====== Ligand "<<i;
			if (name != "") Log<<", "<<name;
			Log<<" ============"<<endl;
			Log.flush();

			if (mol->hasProperty("score_ligcheck"))
			{
				double score_ligcheck = ((String)mol->getProperty("score_ligcheck").toString()).toDouble();
				if (score_ligcheck < 0.95) // 0 = error, 1 = check passed
				{
					cout<<"Skipping compound because it has been marked as containing errors by LigCheck."<<endl;
					delete mol;
					continue;
				}
			}

			double score = 0;
			try
			{
				sp->prepare(mol, par_file);
				if (!rescoring || train)
				{
					scoring_function->setLigand(*mol);
					scoring_function->update();
					score = scoring_function->updateScore();
					scoring_function->printResult();
				}
				if (rescoring)
				{
					if (train)
					{
						if (score > 1000)
						{
							Log.level(10)<<"score>1000, thus current compound is not added to training data set."<<endl;
							delete mol;
							continue;
						}
						rescoring->addScoreContributions(mol);
					}
					else score = rescoring->rescore(mol);
				}

				if (score < output_score_threshold)
				{
					if (ignore_top)
					{
						bool keep_dock_score = 0;
						if (mol->hasProperty("score"))
						{
							double docking_score = mol->getProperty(dock_score_label).toString().toDouble();
							if (docking_score < min_dock_score)
							{
								rescore_list.push_back(make_pair(docking_score, false));
								keep_dock_score = 1;
							}
						}
						if (!keep_dock_score)
						{
							rescore_list.push_back(make_pair(score, true));
							if (score < min_rescore)
							{
								min_rescore = score;
							}
						}
					}
					else
					{
						mol->setProperty("re-score", score);
					}
				}
			}
			catch (BALL::Exception::GeneralException e)
			{
				cout<<e.getMessage()<<endl;
				cout<<"Error! Skipping this molecule!"<<endl;
			}

			if (score < output_score_threshold && (!rescoring || !train) && !ignore_top)
			{
				*output << *mol;
			}

			delete mol;
		}

		/** If ignoring (i.e. not rescoring) the top fraction of docking-results, we need to make sure
		 that all rescored compounds get a rescore-value larger than that of the former. */
		if (ignore_top && !train)
		{
			input->reopen();
			int i = 1;
			double offset = min_dock_score-min_rescore+0.25;

			list<pair<double, bool> >::iterator rescore_it = rescore_list.begin();
			for (Molecule* mol = input->read(); mol; mol = input->read(), i++)
			{
				String name = mol->getName();
				if (mol->hasProperty("score_ligcheck"))
				{
					double score_ligcheck = ((String)mol->getProperty("score_ligcheck").toString()).toDouble();
					if (score_ligcheck < 0.95) // 0 = error, 1 = check passed
					{
						delete mol;
						continue;
					}
				}
				double rescore = rescore_it->first;
				if (rescore_it->second) // if compound was rescored
				{
					rescore += offset;
				}
				if (rescore < output_score_threshold)
				{
					mol->setProperty("re-score", rescore);
					*output << *mol;
				}
				delete mol;
				rescore_it++;
			}
		}

		delete input;
		if (output) delete output;
	}
}
