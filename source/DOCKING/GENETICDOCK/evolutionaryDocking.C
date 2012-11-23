// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/DOCKING/GENETICDOCK/evolutionaryDocking.h>
#include <BALL/DOCKING/GENETICDOCK/geneticAlgorithm.h>
#include <BALL/DOCKING/GENETICDOCK/dockProblem.h>
#include <BALL/SCORING/FUNCTIONS/MMScoring.h>
#include <BALL/SCORING/FUNCTIONS/gridedMM.h>
#include <BALL/SCORING/FUNCTIONS/gridedPLP.h>
#include <BALL/SCORING/FUNCTIONS/PLPScoring.h>

#include <iostream>

bool system_changed_;

using namespace std;

namespace BALL
{
	const String EvolutionaryDocking::Option::MAX_ITERATIONS = "max_iterations";
	const String EvolutionaryDocking::Option::POPULATION_NUMBER = "population_number";
	const String EvolutionaryDocking::Option::INITIAL_POPULATION = "initial_population";
	const String EvolutionaryDocking::Option::POPULATION = "population";
	const String EvolutionaryDocking::Option::SURVIVORS = "survivors";
	const String EvolutionaryDocking::Option::MUTATION_RATE = "mutation_rate";
	const String EvolutionaryDocking::Option::MUTATION_SAVE = "mutation_save";
	const String EvolutionaryDocking::Option::CONV_ITERATIONS = "conv_iterations";
	const String EvolutionaryDocking::Option::CONV_VALUE = "conv_value";
	const String EvolutionaryDocking::Option::CONV_START = "conv_start";
	const String EvolutionaryDocking::Option::BEST_NUM = "best_num";
	const char* EvolutionaryDocking::Option::SCORING_TYPE="scoring_type";

	const int EvolutionaryDocking::Default::MAX_ITERATIONS = 400;
	const int EvolutionaryDocking::Default::POPULATION_NUMBER = 4;
	const int EvolutionaryDocking::Default::INITIAL_POPULATION = 1000;
	const int EvolutionaryDocking::Default::POPULATION = 60;
	const int EvolutionaryDocking::Default::SURVIVORS = 30;
	const double EvolutionaryDocking::Default::MUTATION_RATE = 0.05;
	const int EvolutionaryDocking::Default::MUTATION_SAVE = 1;
	const int EvolutionaryDocking::Default::CONV_ITERATIONS = 5;
	const double EvolutionaryDocking::Default::CONV_VALUE = 0.1;
	const int EvolutionaryDocking::Default::CONV_START = 20;
	const int EvolutionaryDocking::Default::BEST_NUM  = 10;
	const String EvolutionaryDocking::Default::SCORING_TYPE="MM";


	EvolutionaryDocking:: EvolutionaryDocking(System &system1, System &system2, Options& new_options)
		: DockingAlgorithm(system1, system2, new_options),
		ga_(0),
		dp_(0)
	{
		scoring_function_ = 0;
		name_ = "EvolutionaryDocking";
		getDefaultOptions(new_options);
		setup(system1, system2, new_options);
	}

	EvolutionaryDocking::EvolutionaryDocking(System& receptor, System& ligand, string config_file)
		: DockingAlgorithm(receptor, ligand)
	{
		scoring_function_ = NULL;
		name_ = "EvolutionaryDocking";
		list<Constraint*> constraint;
		readOptionFile(config_file, DockingAlgorithm::options, constraint);
		setup(receptor, ligand, DockingAlgorithm::options);
		for (list < Constraint* > ::iterator it = constraint.begin(); it != constraint.end(); it++)
		{
			(*it)->setScoringFunction(scoring_function_);
			scoring_function_->constraints.push_back(*it);
		}
	}

	EvolutionaryDocking::~EvolutionaryDocking()
	{
		destroy_();
		for (list < Constraint* > ::iterator it = scoring_function_->constraints.begin();
		it!=scoring_function_->constraints.end(); it++)
		{
			delete *it;
		}
		delete scoring_function_;
	}


	void EvolutionaryDocking::getDefaultOptions(Options& options)
	{
		options.setDefaultInteger(Option::MAX_ITERATIONS, Default::MAX_ITERATIONS);
		options.setDefaultInteger(Option::POPULATION_NUMBER, Default::POPULATION_NUMBER);
		options.setDefaultInteger(Option::INITIAL_POPULATION, Default::INITIAL_POPULATION);
		options.setDefaultInteger(Option::POPULATION, Default::POPULATION);
		options.setDefaultInteger(Option::SURVIVORS, Default::SURVIVORS);
		options.setDefaultReal(Option::MUTATION_RATE, Default::MUTATION_RATE);
		options.setDefaultInteger(Option::MUTATION_SAVE, Default::MUTATION_SAVE);
		options.setDefaultInteger(Option::CONV_ITERATIONS, Default::CONV_ITERATIONS);
		options.setDefaultReal(Option::CONV_VALUE, Default::CONV_VALUE);
		options.setDefaultInteger(Option::CONV_START, Default::CONV_START);
		options.setDefaultInteger(Option::BEST_NUM, Default::BEST_NUM);
		options.setDefault(Option::SCORING_TYPE, Default::SCORING_TYPE);
	}


	void EvolutionaryDocking::setup(System& receptor, System& ligand, Options& new_options)
	{
		options = new_options;
		scoring_type_ = options.get("scoring_type");
		parameter_filename_ = options.get("filename");
		if (scoring_type_ == "PLP")
		{
			scoring_function_ = new PLPScoring(receptor, ligand, options);
		}
		else if (scoring_type_ == "MM")
		{
			scoring_function_ = new MMScoring(receptor, ligand, options);
		}
		else if (scoring_type_ == "GridedMM")
		{
			scoring_function_ = new GridedMM(receptor, ligand, options);
		}
		else if (scoring_type_ == "GridedPLP")
		{
			scoring_function_ = new GridedPLP(receptor, ligand, options);
		}
		else
		{
			String mess="ScoringFunction type \'"+scoring_type_+"\' unknown/unsupported!";
			throw Exception::GeneralException(__FILE__, __LINE__, "EvolutionaryDocking setup-error", mess);
		}

		setup(ligand, scoring_function_, new_options);
	}


	void EvolutionaryDocking::setup(System& ligand, ScoringFunction* sf, Options& new_options)
	{
		options  = new_options;
		scoring_function_ = sf;
		setup(ligand, sf);
		reference_center_ = scoring_function_->getLigandCenter();
	}

	void EvolutionaryDocking::setup(System& ligand, ScoringFunction* sf)
	{
		/** create new mapping class
		*/
		delete dp_;
		bool post_dock_optimization = options.setDefaultBool("post_dock_optimization", 0);
		dp_ = new DockProblem(ligand, sf, this, post_dock_optimization);
		scoring_function_ = sf;
	}

	void EvolutionaryDocking::start()
	{
		DockingAlgorithm::start();
		dockLigand(*ligand_);
		finish();
	}

	double EvolutionaryDocking::dockLigand(AtomContainer& ligand, bool /*verbose*/)
	{
		// If an optimization of the poses generated by a previous docking-run is desired, do NOT translate the ligand to the geometrical center of the reference ligand but directly use the given input conformation
		AtomContainer ligand_backup;
		double initial_score = 1e10;
		bool post_dock_optimization = options.setDefaultBool("post_dock_optimization", 0);
		if (!post_dock_optimization)
		{
			/// translate current ligand to the geometrical center of the reference ligand
			Vector3 ligand_center(0, 0, 0);
			int no = 0;
			for (AtomIterator atom_it = ligand.beginAtom(); !atom_it.isEnd(); ++atom_it)
			{
				ligand_center += atom_it->getPosition();
				no++;
			}
			ligand_center /= no;
			Vector3 diff =  reference_center_ - ligand_center;

			TMatrix4x4<float> M;
			M.setTranslation(diff);
			for (AtomIterator it = ligand.beginAtom(); it != ligand.endAtom(); it++)
			{
				it->setPosition(M*it->getPosition());
			}
		}
		else
		{
			cout<<"doing PostDock optimization ..."<<endl;
			ligand_backup = ligand;
		}

		// If PostDockOptimization is desired, repeat optimization several times until an enhancement of the score is found. This is different from simply using a higher number of iterations because we always re-start the optimization with the same conformation here.
		double score = 1e10;
		for (Size i = 0; i < 5; i++)
		{
			/// create new DockProblem for current ligand
			dp_->setup(ligand);

			if (i == 0 && post_dock_optimization)
			{
				scoring_function_->update(); // setLigand() has been done by dp_->setup()
				initial_score = scoring_function_->updateScore();
			}

			/// start the docking
			if (dp_ == 0)
			{
				std::cerr << "no mapping class!" << std::endl;
				exit(0);
			}

				// create new genetic algorithm
			delete ga_;

			ga_ = new GeneticAlgorithm(this, dp_, options.setDefaultInteger(Option::POPULATION_NUMBER, Default::POPULATION_NUMBER),
				options.setDefaultInteger(Option::MAX_ITERATIONS, Default::MAX_ITERATIONS),
				options.setDefaultInteger(Option::INITIAL_POPULATION, Default::INITIAL_POPULATION),
				options.setDefaultInteger(Option::POPULATION, Default::POPULATION),
				options.setDefaultInteger(Option::SURVIVORS, Default::SURVIVORS),
				options.setDefaultReal(Option::MUTATION_RATE, Default::MUTATION_RATE),
				options.setDefaultInteger(Option::MUTATION_SAVE, Default::MUTATION_SAVE),
				options.setDefaultInteger(Option::CONV_ITERATIONS, Default::CONV_ITERATIONS),
				options.setDefaultReal(Option::CONV_VALUE, Default::CONV_VALUE),
				options.setDefaultInteger(Option::CONV_START, Default::CONV_START));

				// start optimization
			ga_->start();

			/// set ligand molecule to best found conformation
			applyBestConformation();
			scoring_function_->update();
			score = scoring_function_->updateScore();

			if (!post_dock_optimization)
			{
				break;
			}
			else
			{
				if (score < initial_score) break;
				else ligand = ligand_backup;
			}
		}

		if (post_dock_optimization && score > initial_score)
		{
			ligand = ligand_backup;
			score = initial_score;
			cout<<"No enhancement of pose found during PostDockOptimization."<<endl;
			scoring_function_->setLigand(ligand);
			scoring_function_->update();

			//double score = scoring_function_->updateScore();
			scoring_function_->updateScore();
		}

		if (display_mode_ != NO_DISPLAY) // show final pose
		{
			setVisualizationPose(score);
			cout<<"final score = "<<score<<endl;
		}
		return score;
	}

	void EvolutionaryDocking::applyBestConformation()
	{
		ga_->calculate(&ga_->pools_[0][0]); // assign best conformation-vector to DockProblem
		dp_->move(); // reassign coordinates to molecule (because DockProblem::calculate() restores original atom positions after calculating a score)
	}


	float EvolutionaryDocking::getProgress() const
	{
	if (ga_->iteration_ == 0)
	return 0.0;

	return float(ga_->iteration_/ga_->max_iterations_);
	}


	bool EvolutionaryDocking::redraw()
	{
	return (dp_->redraw());
	}

	//   ConformationSet EvolutionaryDocking::getConformationSet(Index total_number)
	//   {
	//     /** store best individuals
	//      */
	//     vector<GeneticIndividual*> gis;
	//
	//     for (Index x = 0; x < total_number; ++x)
	//       gis.push_back(ga_->getIndividual(x));
	//
	//     return dp_->getConformationSet();
	//   }

	Vector3& EvolutionaryDocking::getReferenceLigandCenter()
	{
		return reference_center_;
	}

	void EvolutionaryDocking::destroy_()
	{
	delete ga_;
	delete dp_;
	}


	//   ConformationSet EvolutionaryDocking::getConformationSet(Index total_conformations)
	//   {
	// 	if(total_conformations==0) total_conformations=10;
	// 	ConformationSet conf_set;
	//  	QSAR::SortedList<pair<double,GeneticIndividual*> > slist;
	//
	//  	/// fetch the best conformations from all pools
	//  	vector<std::vector<GeneticIndividual> >& pools = ga_->pools_;  // alias
	// 	for(Size i=0; i<pools.size();i++)
	// 	{
	// 		for(Size j=0; j<pools[i].size(); j++)
	// 		{
	// 			double score = pools[i][j].getFitnessValue();
	// 			if(slist.size()<total_conformations || score<slist.back().first)
	// 			{
	// 				slist.insert(make_pair(score, &pools[i][j]));
	// 				if(slist.size()>total_conformations)
	// 				{
	// 					slist.pop_back();
	// 				}
	// 			}
	// 		}
	// 	}
	//
	// 	/// create a snapshot for each selected conformation
	// 	slist.front();
	// 	while(slist.hasNext())
	// 	{
	// 		const pair<double,GeneticIndividual*>& p = slist.next();
	// 		ga_->calculate(p.second); // assign coordinates coording to p.second
	// 		conf_set.add(p.first,*scoring_function_->getLigand());
	// 	}
	//
	// 	applyBestConformation();
	//
	// 	return conf_set;
	//   }


	//   const AtomContainer& EvolutionaryDocking::getIntermediateResult_()
	//   {
	//     return dp_->getIntermediateResult(system_changed_);
	//   }
}
