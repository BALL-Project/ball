// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/DOCKING/IMGDOCK/IMGDock.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/MATHS/plane3.h>
#include <cmath>
#include <sstream>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/SDFile.h>

#include <BALL/SCORING/FUNCTIONS/MMScoring.h>
#include <BALL/SCORING/FUNCTIONS/gridedMM.h>
#include <BALL/SCORING/FUNCTIONS/gridedPLP.h>
#include <BALL/SCORING/FUNCTIONS/PLPScoring.h>

using namespace std;

namespace BALL
{
	const char* IMGDock::Option::GLOBAL_ROTATION="global_rotation";
	const char* IMGDock::Option::POST_OPTIMIZATION_STEPS="post_optimization_steps";
	const char* IMGDock::Option::POST_OPTIMIZATION_STEP_WIDTH="post_optimization_step_width";
	const char* IMGDock::Option::MIN_INHIBITOR_ATOMS="min_inhibitor_atoms";
	const char* IMGDock::Option::SCORING_TYPE="scoring_type";
	const char* IMGDock::Option::STEP_WIDTH="step_width";
	const char* IMGDock::Option::NO_SOLUTIONS="no_solutions";
	const char* IMGDock::Option::ITERATIONS="iterations";
	const char* IMGDock::Option::DECREASE_STEPWIDTH="decrease_stepwidth";
	const char* IMGDock::Option::SUPERPOSE_LIGAND="superpose_ligand";

	bool IMGDock::Default::GLOBAL_ROTATION = 1;
	int IMGDock::Default::STEP_WIDTH = 10;
	int IMGDock::Default::NO_SOLUTIONS = 100;
	int IMGDock::Default::POST_OPTIMIZATION_STEPS = 6;
	double IMGDock::Default::POST_OPTIMIZATION_STEP_WIDTH = 0.5;
	int IMGDock::Default::MIN_INHIBITOR_ATOMS = 10;
	String IMGDock::Default::SCORING_TYPE="GridedMM";
	int IMGDock::Default::ITERATIONS = 4;
	bool IMGDock::Default::DECREASE_STEPWIDTH = 0;
	bool IMGDock::Default::SUPERPOSE_LIGAND = 1;



	IMGDock::IMGDock(System& receptor, System& ligand, Options& new_options)
		: DockingAlgorithm(receptor, ligand, new_options)
	{
		scoring_function_ = NULL;
		setup(receptor, ligand, new_options);
	}


	IMGDock::IMGDock(System& receptor, System& ligand)
		: DockingAlgorithm(receptor, ligand)
	{
		scoring_function_ = NULL;
		setup(receptor, ligand);
	}

	IMGDock::IMGDock(System& receptor, System& ligand, string config_file)
	: DockingAlgorithm(receptor, ligand)
	{
		scoring_function_ = NULL;
		list<Constraint*> constraint;
		readOptionFile(config_file, DockingAlgorithm::options, constraint);
		setup(receptor, ligand, DockingAlgorithm::options);
		for (list < Constraint* > ::iterator it = constraint.begin(); it != constraint.end(); it++)
		{
			(*it)->setScoringFunction(scoring_function_);
			scoring_function_->constraints.push_back(*it);
		}
	}


	IMGDock::~IMGDock()
	{
		for (list < Constraint* > ::iterator it = scoring_function_->constraints.begin();
			it!=scoring_function_->constraints.end(); it++)
		{
			delete *it;
		}
		delete scoring_function_;
	}

	void IMGDock::setup(System& receptor, System& ligand, Options& input_options)
	{
		ligand_ = ligand.getMolecule(0);
		receptor_ = receptor.getProtein(0);

		name_ = "Iterative Multi-Greedy Docking";
		Options* option_category = input_options.getSubcategory("Scoring Function");
		if (!option_category) option_category = &input_options;
		parameter_filename_ = option_category->get("filename");

		option_category = input_options.getSubcategory("IMGDock");
		if (!option_category) option_category = &input_options;

		// == == set the input_options == == == == //
		global_rotation_ = option_category->setDefaultBool(Option::GLOBAL_ROTATION, Default::GLOBAL_ROTATION);
		step_width_ = option_category->setDefaultInteger(Option::STEP_WIDTH, Default::STEP_WIDTH);
		no_solutions_ = option_category->setDefaultInteger(Option::NO_SOLUTIONS, Default::NO_SOLUTIONS);
		post_optimization_step_width_ = option_category->setDefaultReal(Option::POST_OPTIMIZATION_STEP_WIDTH, Default::POST_OPTIMIZATION_STEP_WIDTH);
		post_optimization_steps_ = option_category->setDefaultInteger(Option::POST_OPTIMIZATION_STEPS, Default::POST_OPTIMIZATION_STEPS);
		min_inhibitor_atoms_ = option_category->setDefaultInteger(Option::MIN_INHIBITOR_ATOMS, Default::MIN_INHIBITOR_ATOMS);
		scoring_type_ = option_category->setDefault(Option::SCORING_TYPE, Default::SCORING_TYPE);
		iterations_ = option_category->setDefaultInteger(Option::ITERATIONS, Default::ITERATIONS);
		decrease_stepwidth_ = option_category->setDefaultBool(Option::DECREASE_STEPWIDTH, Default::DECREASE_STEPWIDTH);
		superpose_ligand_ = option_category->setDefaultBool(Option::SUPERPOSE_LIGAND, Default::SUPERPOSE_LIGAND);
		// == == == == == == == == == == == == == //

		if (scoring_type_ == "MM")
		{
			scoring_function_ = new MMScoring(*receptor_, *ligand_, input_options);
		}
		else if (scoring_type_ == "GridedMM")
		{
			scoring_function_ = new GridedMM(*receptor_, *ligand_, input_options);
		}
		else if (scoring_type_ == "GridedPLP")
		{
			scoring_function_ = new GridedPLP(*receptor_, *ligand_, input_options);
		}
		else if (scoring_type_ == "PLP")
		{
			scoring_function_ = new PLPScoring(*receptor_, *ligand_, input_options);
		}
		else
		{
			String mess="ScoringFunction type \'"+scoring_type_+"\' unknown/unsupported!";
			throw Exception::GeneralException(__FILE__, __LINE__, "IMGDock error", mess);
		}

		if (scoring_function_->getStaticLigandFragments()->size() == 0)
		{
			// do only if not already by ScoringFunction (depended on input_options)
			scoring_function_->createStaticLigandFragments();
		}

		saveBondInformation();

		ReferenceArea* rf = new ReferenceArea(ligand_, 0, min_inhibitor_atoms_, 1e10);
		rf->setName("reference ligand area");
		scoring_function_->constraints.push_back(rf);

		reference_center_ = scoring_function_->getLigandCenter();
		sidechain_optimizer_ = NULL;
		if (scoring_function_->hasFlexibleResidues())
		{
			sidechain_optimizer_ = new SideChainOptimizer(scoring_function_);
		}
	}


	void IMGDock::getDefaultOptions(Options& output_options)
	{
		Options* option_category = output_options.createSubcategory("IMGDock");

		//option_category->setDefaultBool(Option::GLOBAL_ROTATION, Default::GLOBAL_ROTATION);

		option_category->setDefaultInteger(Option::STEP_WIDTH, Default::STEP_WIDTH);
		list<String> allowed_values;
		allowed_values.push_back("1");
		allowed_values.push_back("90");
		option_category->addParameterDescription(Option::STEP_WIDTH, "step-width (bond angle discretization)", BALL::INT, &allowed_values);

		option_category->setDefaultInteger(Option::NO_SOLUTIONS, Default::NO_SOLUTIONS);
		allowed_values.clear();
		allowed_values.push_back("10");
		allowed_values.push_back("1000");
		option_category->addParameterDescription(Option::NO_SOLUTIONS, "num. multi-greedy solutions", BALL::INT, &allowed_values);

		option_category->setDefaultReal(Option::POST_OPTIMIZATION_STEP_WIDTH, Default::POST_OPTIMIZATION_STEP_WIDTH);
		allowed_values.clear();
		allowed_values.push_back("0.1");
		allowed_values.push_back("2");
		option_category->addParameterDescription(Option::POST_OPTIMIZATION_STEP_WIDTH, "translation opt. step-width", BALL::DOUBLE, &allowed_values);

		option_category->setDefaultInteger(Option::POST_OPTIMIZATION_STEPS, Default::POST_OPTIMIZATION_STEPS);
		allowed_values.clear();
		allowed_values.push_back("0");
		allowed_values.push_back("10");
		option_category->addParameterDescription(Option::POST_OPTIMIZATION_STEPS, "max. translation opt. steps", BALL::INT, &allowed_values);

		option_category->setDefaultInteger(Option::MIN_INHIBITOR_ATOMS, Default::MIN_INHIBITOR_ATOMS);
		allowed_values.clear();
		allowed_values.push_back("0");
		allowed_values.push_back("100");
		option_category->addParameterDescription(Option::MIN_INHIBITOR_ATOMS, "min. atoms in ref. lig. area", BALL::INT, &allowed_values);

		option_category->setDefaultInteger(Option::ITERATIONS, Default::ITERATIONS);
		allowed_values.clear();
		allowed_values.push_back("1");
		allowed_values.push_back("20");
		option_category->addParameterDescription(Option::ITERATIONS, "max. IMG iterations", BALL::INT, &allowed_values);

		option_category->setDefaultBool(Option::DECREASE_STEPWIDTH, Default::DECREASE_STEPWIDTH);
		allowed_values.clear();
		allowed_values.push_back("true");
		allowed_values.push_back("false");
		option_category->addParameterDescription(Option::DECREASE_STEPWIDTH, "decrease step-width in each iterations", BALL::STRING, &allowed_values);

		option_category->setDefaultBool(Option::SUPERPOSE_LIGAND, Default::SUPERPOSE_LIGAND);
		option_category->addParameterDescription(Option::SUPERPOSE_LIGAND, "superpose ligands with ref.-ligand", BALL::STRING, &allowed_values);

		option_category->setDefaultBool("output_failed_dockings", false);
		option_category->addParameterDescription("output_failed_dockings", "output erroneous molecules", BALL::STRING, &allowed_values);
	}


	void IMGDock::setup(System& receptor, System& ligand)
	{
		ligand_ = ligand.getMolecule(0);
		receptor_ = receptor.getProtein(0);
		name_ = "IterativeMultiGreedyDocking";

		// == == set the default options == == == == //
		step_width_ = 30;
		no_solutions_ = 2;
		global_rotation_ = 0;
		post_optimization_steps_ = 0;
		min_inhibitor_atoms_ = 0;
		scoring_type_ = options.setDefault(Option::SCORING_TYPE, Default::SCORING_TYPE);
		superpose_ligand_ = 0;
		iterations_ = 1;
		// == == == == == == == == == == == == == //

		if (scoring_type_ == "MM")
		{
			scoring_function_ = new MMScoring(*receptor_, *ligand_, options);
		}
		else if (scoring_type_ == "GridedMM")
		{
			scoring_function_ = new GridedMM(*receptor_, *ligand_, options);
		}
		else if (scoring_type_ == "PLP")
		{
			scoring_function_ = new PLPScoring(*receptor_, *ligand_, options);
		}
		else if (scoring_type_ == "gridedPLP")
		{
			scoring_function_ = new GridedPLP(*receptor_, *ligand_, options);
		}
		else
		{
			String mess="ScoringFunction type \'"+scoring_type_+"\' unknown/unsupported!";
			throw Exception::GeneralException(__FILE__, __LINE__, "IMGDock error", mess);
		}

		scoring_function_->createStaticLigandFragments();

		saveBondInformation();

		ReferenceArea* rf = new ReferenceArea(ligand_, 0, min_inhibitor_atoms_, 1e10);
		rf->setName("reference ligand area");
		scoring_function_->constraints.push_back(rf);
		sidechain_optimizer_ = NULL;
		if (scoring_function_->hasFlexibleResidues())
		{
			sidechain_optimizer_ = new SideChainOptimizer(scoring_function_);
		}
	}


	void IMGDock::start()
	{
		DockingAlgorithm::start();
		dockLigand(*ligand_);
		finish();
	}


	double IMGDock::dockLigand(AtomContainer& ligand, bool verbose)
	{
		ligand_ = &ligand;

		scoring_function_->setLigand(ligand);

		if (scoring_function_->getStaticLigandFragments()->size() == 0)
		{
			// do only if not already by ScoringFunction (depended on options)
			scoring_function_->createStaticLigandFragments();
		}

		if (superpose_ligand_)
		{
			mapLigandOntoReferenceLigand();
		}
		else
		{
			Vector3 diff =  reference_center_ - scoring_function_->getLigandCenter();
			translateLigand(diff);
		}

	// 	AtomPairList mapped_atoms;
	// 	bool found=0;
	// 	Atom* const_Atom;
	// 	for(AtomIterator it1=ligand_->beginAtom(); +it1; it1++)
	// 	{
	// 		for(AtomIterator it2=reference_ligand_->beginAtom(); +it2; it2++)
	// 		{
	// 			if(it1->getPosition().getDistance(it2->getPosition()) < 0.5)
	// 			{
	// 				mapped_atoms.push_back(&*it1,&*it2);
	// 				const_Atom = &*it1;
	// 				Log<<it1->getFullName()<<"  "<<it1->getPosition()<<endl;
	// 				found=1;
	// 				break;
	// 			}
	// 		}
	// 		if(found) break;
	// 	}
	// 	Log<<"no of mapped atom = "<<mapped_atoms.size()<<endl;
	//	saveBondInformation(&mapped_atoms);
		saveBondInformation();

		Timer timer;
		timer.start();

		double old_score = 1e100;

		for (int a = 0; a < iterations_ && !abort_; a++)
		{
			if (a == 0)
			{
				no_steps_ = 360/step_width_;
				if (decrease_stepwidth_ && no_steps_%2 != 0) no_steps_++;
			}
			else if (decrease_stepwidth_) step_width_ /= 2;

			if (display_mode_ != NO_DISPLAY && display_mode_ != NO_INTERMEDIATE_POSES)
			{
				Log<<"---- iteration	"<<a<<" -------"<<endl;
				setVisualizationPose(score_);
			}

			startDock(verbose);

			if (scoring_function_->hasFlexibleResidues())
			{
				// Optimizes the placement of all flexible residues
				// and automatically recalculates the scoregrids for those residues afterwards
				sidechain_optimizer_->optimize();
			}

			if (score_ >= (old_score-0.01)) break;
			else old_score = score_;
		}

		update();

		// if started from BALLView, show start conformation
		if (display_mode_ != NO_DISPLAY && display_mode_ != ALL_INTERMEDIATE_POSES)
		{
			setVisualizationPose(score_);
			Log<<"score after completed docking = "<<score_<<endl;
		}

		timer.stop();
		Log.level(20)<<timer.getClockTime()<<" seconds"<<endl;

		return getScore();
	}


	void IMGDock::saveAtomPositions()
	{
		original_atom_positions_.resize(scoring_function_->getNoLigandAtoms());
		Size atom = 0;
		for (AtomIterator it = ligand_->beginAtom(); +it; it++, atom++)
		{
			original_atom_positions_[atom] = it->getPosition();
		}
	}


	void IMGDock::resetRotations()
	{
		current_conformation_ = vector < int > (current_conformation_.size(), 0);
		int atom = 0;
		for (AtomIterator it = ligand_->beginAtom(); +it; it++, atom++)
		{
			it->setPosition(original_atom_positions_[atom]);
		}
	}


	void IMGDock::startDock(bool verbose)
	{
		update();

		// if started from BALLView, show start conformation
		if (display_mode_ != NO_DISPLAY && display_mode_ != NO_INTERMEDIATE_POSES)
		{
			setVisualizationPose(score_);
			Log<<"score of start conformation = "<<score_<<endl;
		}

		max_rotated_pos_ = 0;
		best_conformations_.clear();

		Size no_bonds = current_conformation_.size();
		//Log<<"start conf="; displayConformation(current_conformation_, force_field_->getEnergy());

		/// reset current_conformation_ to zero, since we use the current ligand pose as starting conformation!
		current_conformation_ = vector < int > (current_conformation_.size(), 0);
		vector < int > c0 = current_conformation_; // starting conformation; all angles at 0 degrees

		saveAtomPositions();

		/// put first entries into list 'best_conformations' ...
		optimizeRotation(c0, best_conformations_, 0, false); // start at first inter-fragment bond and thus generate < no_solutions_ > entries for best_conformations
		if (verbose)
		{
			Log<<"i = 0"<<endl;
			for (PoseList::iterator it = best_conformations_.begin(); it != best_conformations_.end(); it++)
			{
				displayConformation(it->second, it->first);
			}
		}


		/// use existing entries of list 'best_conformations' in order to find other conformations and append those to the list (if score high enough or list still small enough) ...
		for (Size i = 1; i < no_bonds && !abort_; i++)
		{
			if (verbose) Log<<"i="<<i<<endl;
			PoseList best_conformations0 = best_conformations_;

			// Optimize the _current_ bond of _each_ of the < no_solutions_ > different ligand conformations
			for (PoseList::iterator it = best_conformations0.begin(); it != best_conformations0.end(); it++)
			{
				vector<int> conf = const_cast<vector<int>& >(it->second);
				if (verbose) displayConformation(conf, it->first);
				optimizeRotation(conf, best_conformations_, i, true);
			}

			// if started from BALLView, show best pose found so far
			if (display_mode_ == BEST_INTERMEDIATE_POSES)
			{
				double score = best_conformations_.begin()->first;
				applyConformation(best_conformations_.begin()->second);
				setVisualizationPose(score);
				Log<<"\rscore = "<<score<<endl;
			}
		}

		/// Apply the best conformation found in this iteration and do a local translation optimization.
		/// In the following iteration (next call of this method), this position of the ligand will be used as the starting conformation.
		applyConformation(best_conformations_.begin()->second);
		score_ = best_conformations_.begin()->first; // use previously calculated score if no transl. opt. is used

		if (post_optimization_steps_ > 0 && post_optimization_step_width_ > 0)
		{
			postDockOptimization(post_optimization_step_width_, post_optimization_steps_);

			// make sure first entry of ConformationSet will contain the correct score
			// (important only if getConformationSet() is used)
			pair < double, vector < int > > best_pose_copy = *best_conformations_.begin();
			best_pose_copy.first = score_;
			best_conformations_.erase(best_conformations_.begin());
			best_conformations_.insert(best_pose_copy);
		}

		// if started from BALLView, show start conformation
		if (display_mode_ != NO_DISPLAY && display_mode_ != NO_INTERMEDIATE_POSES)
		{
			setVisualizationPose(score_);
			Log<<"score after optimized translation = "<<score_<<endl;
		}
	}


	void IMGDock::postDockOptimization(double step_width, int no_steps)
	{
		double best_score = getScore();

		for (int a = 0; a <= no_steps; a++)
		{
			Vector3 best_translation;
			bool decrease = 0; // was there an descrease of energy ?

			for (int i = 0; i < 6; i++) // try moving the ligand a small step into each direction,
			{
				float x = 0; float y = 0; float z = 0;
				if (i == 0) x = step_width;
				else if (i == 1) x = -step_width;
				else if (i == 2) y = step_width;
				else if (i == 3) y = -step_width;
				else if (i == 4) z = step_width;
				else if (i == 5) z = -step_width;
				Vector3 v(x, y, z);
				translateLigand(v);
				update();
				double score = getScore();
				if (score < best_score)
				{
					best_score = score;
					best_translation = v;
					decrease = 1;
				}
				v = -v;
				translateLigand(v); // translate back to start position of step a
			}
			if (!decrease) // if there was no decrease of energy, then stop, since else we would
			{		// just check the same transformations as above again
				break;
			}
			else
			{
				translateLigand(best_translation);
				//Log<<"best translation=" <<best_translation<<endl;
			}
		}
		score_ = best_score;
	}


	void IMGDock::displayConformation(const vector < int > & conf, const double& energy)
	{
		for (Size j = 0; j < conf.size(); j++)
		{
			Log<<conf[j]<<" ";
		}
		Log<<" E="<<energy<<endl;
	}


	void IMGDock::saveBondInformation()
	{
		bond_information_.clear();
		Size no_bonds = 0;

		const vector<StaticLigandFragment*>* ligand_fragments = scoring_function_->getStaticLigandFragments();

		for (int i = -3*global_rotation_; i < (int)ligand_fragments->size(); i++)
		{
			if (i < 0) // rotation around global x-, y- or z-axis
			{
				vector<int> v(2);
				v[0] = -1; // indicate that this is no fragment rotation...
				v[1] = -1;
				bond_information_.push_back(v);
				no_bonds++;
				continue;
			}
			Size bonds_i = (*ligand_fragments)[i]->connections.size();
			no_bonds += bonds_i;
			for (Size j = 0; j < bonds_i; j++)
			{
				vector<int> v(2);
				v[0] = i; // id of current fragment
				v[1] = j; // id of bond of this fragment
				bond_information_.push_back(v);
			}
		}
		//vector < int > c0(no_bonds, 0); // starting conformation; all angles at 0°
		current_conformation_.clear();
		current_conformation_.resize(no_bonds, 0);
		max_rotated_pos_ = -1;
	}


	void IMGDock::saveBondInformation(AtomPairList* mapped_atoms)
	{
		if (mapped_atoms->size() == 0)
		{
			Log<<"[warning:] no ligand-atoms where mapped onto reference-ligand-atoms"<<endl;
			saveBondInformation();
			return;
		}

		set<int> frozen_fragments;
		set<int> processed_fragments;

		bool use_first = 1;
		//if (mapped_atoms->begin()->first->getMolecule() != ligand_) use_first = 0;
		const map<Atom*, int>* atom_map = scoring_function_->getAtomFragmentMap();

		for (AtomPairList::iterator it = mapped_atoms->begin(); it != mapped_atoms->end(); it++)
		{
			int frag;

			if (use_first)
			{
				map<Atom*, int>::const_iterator map_it = atom_map->find(it->first);
				if (map_it == atom_map->end()) continue;
				frag = map_it->second;
			}
			else
			{
				map<Atom*, int>::const_iterator map_it = atom_map->find(it->second);
				if (map_it == atom_map->end()) continue;
				frag = map_it->second;
			}
			frozen_fragments.insert(frag);
		}

		bond_information_.clear();
		global_rotation_ = 0;

		int frag = *frozen_fragments.begin();
		findUnfrozenFragments(frag, frozen_fragments, processed_fragments);

		//vector < int > c0(no_bonds, 0); // starting conformation; all angles at 0°
		current_conformation_.clear();
		current_conformation_.resize(bond_information_.size(), 0);
		max_rotated_pos_ = -1;
	}


	bool IMGDock::findUnfrozenFragments(int fragment, set<int> frozen_fragments, set<int>& processed_fragments)
	{
		processed_fragments.insert(fragment);

		bool is_frozen = (frozen_fragments.find(fragment)!=frozen_fragments.end());
		bool is_connected_to_frozen_frag = 0;

		vector<StaticLigandFragment::Connection>& connections((*scoring_function_->getStaticLigandFragments())[fragment]->connections);

		for (Size j = 0; j < connections.size(); j++)
		{
			int neighbor_fragment = connections[j].fragment->ID;

			// add rotation-axis information only if the neighboring fragment has not been visited before and the neighboring fragment is not located within a frozen part of the ligand
			if (processed_fragments.find(neighbor_fragment) != processed_fragments.end())
			{
				continue;
			}
			if (findUnfrozenFragments(neighbor_fragment, frozen_fragments, processed_fragments))
			{
				is_connected_to_frozen_frag = 1;
				continue;
			}

			// allow the *neighboring* fragment to rotate around the bond connecting it this fragment
			vector<int> v(2);
			int nb_bond_id = 0;
			vector<StaticLigandFragment::Connection>& nb_connections((*scoring_function_->getStaticLigandFragments())[fragment]->connections);
			for (Size b = 0; b < nb_connections.size(); b++)
			{
				if (nb_connections[b].fragment->ID == fragment)
				{
					nb_bond_id = b;
					break;
				}
			}
			v[0] = neighbor_fragment; // id of the neighboring fragment
			v[1] = nb_bond_id; // id of bond of neighboring fragment
			bond_information_.push_back(v);
		}

		return (is_connected_to_frozen_frag || is_frozen) ;
	}


	void IMGDock::optimizeRotation(vector < int > & conf, PoseList& best_conformations, Size bond, bool ignore_original_angle)
	{
		// just in order to produce exactly the same result as before (since the order of tested conformation does play a role)
		if (!decrease_stepwidth_)
		{
			int no_steps = no_steps_;
			if (ignore_original_angle) no_steps--;

			for (int i = 0; i < no_steps; i++)
			{
				conf[bond] = (conf[bond]+step_width_)%360;
				applyConformation(conf);

				update();
				double score = getScore();
				if (best_conformations.size() < no_solutions_ || score < best_conformations.rbegin()->first)
				{
					best_conformations.insert(make_pair(score, conf));
					if (best_conformations.size() > no_solutions_)
					{
						PoseList::iterator it = best_conformations_.end();
						it--;
						best_conformations.erase(it);
					}
				}
			}
		}

		else
		{
			int original_value = conf[bond];
			conf[bond] = (conf[bond]-(no_steps_/2)*step_width_)%360;
			if (conf[bond] < 0) conf[bond] = 360+conf[bond];
			//Log<<"conf[bond] ="<<conf[bond]<<"  ";

			for (int i = 0; i < no_steps_; i++)
			{
				conf[bond] = (conf[bond]+step_width_)%360;
				if (ignore_original_angle && conf[bond] == original_value) continue; // this conformation has been evaluated previously

				applyConformation(conf);

				update();
				double score = getScore();
				if (best_conformations.size() < no_solutions_ || score < best_conformations.rbegin()->first)
				{
					best_conformations.insert(make_pair(score, conf));
					if (best_conformations.size() > no_solutions_)
					{
						PoseList::iterator it = best_conformations_.end();
						it--;
						best_conformations.erase(it);
					}
				}
			}//Log<<endl;
		}
	}



	void IMGDock::applyConformation(const vector < int > & conf, bool verbose)
	{
		int frag = 0; int bond = 0;

		resetRotations(); // reset all angles to zero and apply given conformation AFTER doing this !!

		for (Size i = 0; i < conf.size(); i++)
		{
			int diff_angle = conf[i] - current_conformation_[i];
			if (diff_angle != 0)
			{
				frag = bond_information_[i][0];
				bond = bond_information_[i][1];
				if (!global_rotation_ || i >= 3)
				{
					rotateLigandFragment(frag, bond, diff_angle);
				}
				else
				{
					rotateLigand(i, diff_angle);
				}
				current_conformation_[i] += diff_angle;
			}
			if (current_conformation_[i] != 0)
			{
				max_rotated_pos_ = i; // largest index of already changed bond
			}
		}
		if (verbose)
		{
			update();
			printResults();
		}
	}


	void IMGDock::rotateLigand(int a, int degree)
	{
		TMatrix4x4<float> M;
		TAngle<float> angle(degree, false);
		Vector3 axis(a == 0, a == 1, a == 2);
		M.setRotation(angle, axis);
		const Vector3& origin = scoring_function_->getLigandCenter();

		for (AtomIterator it = ligand_->beginAtom(); it != ligand_->endAtom(); it++)
		{
			it->setPosition(M*(it->getPosition()-origin)+origin);
		}
	}


	void IMGDock::translateLigand(Vector3& v)
	{
		TMatrix4x4<float> M;
		M.setTranslation(v);

		// transform all atoms of the ligand
		for (AtomIterator it = ligand_->beginAtom(); it != ligand_->endAtom(); it++)
		{
			it->setPosition(M*it->getPosition());
		}
	}




	void IMGDock::rotateLigandFragment(Size fragment, Size bond, int degree)
	{
		const vector<StaticLigandFragment*>* ligand_fragments = scoring_function_->getStaticLigandFragments();

		if (ligand_fragments->size() < fragment)
		{
			String s="There are less than ";
			s = s+String(fragment+1)+" fragments!";
			throw Exception::GeneralException(__FILE__, __LINE__, "Rotation error", s);
			return;
		}
		StaticLigandFragment* lf = (*ligand_fragments)[fragment];
		if (lf->connections.size() < bond)
		{
			String s="Fragment has less than ";
			s = s+String(bond+1)+" bonds!";
			throw Exception::GeneralException(__FILE__, __LINE__, "Rotation error", s);
			return;
		}
		StaticLigandFragment::Connection* nc = &lf->connections[bond];

		TMatrix4x4<float> M;
		TAngle<float> angle(degree, false);
		Vector3 axis = nc->neighbor_atom->getPosition() - nc->atom->getPosition();
		M.setRotation(angle, axis);
		Vector3 origin = nc->atom->getPosition();

		rotateLigandFragment(lf, M, origin, nc->fragment->ID);
	}


	void IMGDock::rotateLigandFragment(StaticLigandFragment* lf, TMatrix4x4<float>& M, Vector3& origin, int static_fragment)
	{
		// rotate all atoms of this fragment
		for (list < Atom* > ::iterator it = lf->atoms.begin(); it != lf->atoms.end(); it++)
		{
			(*it)->setPosition(M*((*it)->getPosition()-origin)+origin);
		}
		// ... and now rotate all neighboring fragments, except <staticFragment> ( == already rotated fragment or one which is to be kept static)
		for (Size i = 0; i < lf->connections.size(); i++)
		{
			if (lf->connections[i].fragment->ID != static_fragment)
			{
				rotateLigandFragment(lf->connections[i].fragment, M, origin, lf->ID);
			}
		}
	}


	// void IMGDock::rotateAroundBond(const NeighborConnection* rotation_bond, int degree, int static_fragment)
	// {
	// 	TMatrix4x4<float> M;
	// 	TAngle<float> angle(degree,false);
	// 	M.setRotation(angle,rotation_bond->axis);
	// 	Vector3 origin = rotation_bond->atom->getPosition();
	//
	// 	// rotate all atoms of this fragment
	// 	for(list<Atom*>::iterator it=atoms.begin(); it!=atoms.end();it++)
	// 	{
	// 		(*it)->setPosition(M*((*it)->getPosition()-origin)+origin);
	// 	}
	// 	// ... and now rotate all neighboring fragments, except <staticFragment> (== already rotated fragment or one which is to be keept static)
	// 	for(Size i=0; i<neighbor_connections.size();i++)
	// 	{
	// 		if(neighbor_connections[i].fragment->ID!=static_fragment)
	// 		{
	// 			neighbor_connections[i].fragment->rotate(rotation_bond, degree, ID);
	// 		}
	// 	}
	// }



	void IMGDock::printResults(ostream& out)
	{
		if (scoring_function_ == NULL)
		{
			out<<"Not initialized!!"<<endl;
			return;
		}

	// 	double interact_energy = interaction_->getEnergy();
	// 	double total_energy = getEnergy();
	// 	double conf_energy = total_energy - interact_energy;
	//
	// 	out<<"ligand conformation = "<<conf_energy<<" kJ/mol"<<endl;
	// 	out<<" interaction energy = "<<interact_energy<<" kJ/mol"<<endl;
	// 	out<<"    ---------------------------------"<<endl;
	// 	out<<"       total energy = "<<total_energy<<" kJ/mol"<<endl;
	// 	out<<"       IMGDock score = "<<score_<<endl;

		Log.level(20)<<"score = "<<scoring_function_->getScore()<<endl;
	}

	void IMGDock::update()
	{
		scoring_function_->update();
		scoring_function_->updateScore();

		score_ = scoring_function_->getScore();

		if (display_mode_ == ALL_INTERMEDIATE_POSES)
		{
			setVisualizationPose(score_);
		}
	}


	double IMGDock::getEnergy()
	{
		return 0;
	}


	double IMGDock::getScore()
	{
		return score_;
	}


	void IMGDock::printFragments(ostream& out)
	{
		out<<"fragment-no\t#atoms\tneighboring fragments\tatoms\n";

		const vector<StaticLigandFragment*>* ligand_fragments = scoring_function_->getStaticLigandFragments();

		for (Size i = 0; i < ligand_fragments->size(); i++)
		{
			out<<i<<"\t"<<(*ligand_fragments)[i]->atoms.size()<<"\t";
			for (Size j = 0; j < (*ligand_fragments)[i]->connections.size(); j++)
			{
				out<<(*ligand_fragments)[i]->connections[j].fragment->ID<<" ";
			}
			out<<"\t";
			for (list < Atom* > ::iterator it = (*ligand_fragments)[i]->atoms.begin(); it != (*ligand_fragments)[i]->atoms.end(); it++)
			{
				out<<(*it)->getName()<<" ";
			}
			out<<endl;
		}

		out<<endl;

	// 	for(int i=0; i<bond_information_.size();i++)
	// 	{
	// 		for(int j=0; j<bond_information_[i].size();j++)
	// 		{
	// 			out<<bond_information_[i][j]<<"\t";
	// 		}
	// 		out<<endl;
	// 	}
	}


	void IMGDock::storeForces()
	{
		int i = 0;
		for (AtomIterator it = ligand_->beginAtom(); !it.isEnd(); it++)
		{
			old_forces_[i] = it->getForce();
			i++;
		}
	}


	void IMGDock::reassignForces()
	{
		int i = 0;
		for (AtomIterator it = ligand_->beginAtom(); !it.isEnd(); it++)
		{
			it->setForce(old_forces_[i]);
			i++;
		}
	}


	const AtomContainer* IMGDock::getLigand()
	{
		return ligand_;
	}



	// ConformationSet IMGDock::getConformationSet(Index total_conformations)
	// {
	// 	ConformationSet conf_set;
	// 	best_conformations_.front();
	// 	if(total_conformations==0) total_conformations=10;
	// 	for(Size i=0; i<total_conformations && best_conformations_.hasNext(); i++)
	// 	{
	// 		const pair<double,vector<int> >& p(best_conformations_.next());
	// 		applyConformation(p.second,true);
	// 		conf_set.add(p.first,*ligand_);
	// 	}
	//
	// 	applyConformation(best_conformations_.front().second); // restore best conformation
	//
	// 	return conf_set;
	// }
}
