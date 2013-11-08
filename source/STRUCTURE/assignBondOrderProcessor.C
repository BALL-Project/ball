// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <iostream>
#include <map>

// BALL includes
#include <BALL/kernel.h>
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
#include <BALL/SYSTEM/timer.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/FORMAT/parameters.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/QSAR/aromaticityProcessor.h>
#include <BALL/KERNEL/expression.h>

// Qt
#include <QtXml/QtXml>
#include <QtXml/qdom.h>

#include <BALL/STRUCTURE/BONDORDERS/AStarBondOrderStrategy.h>
#include <BALL/STRUCTURE/BONDORDERS/branchAndBoundBondOrderStrategy.h>
#include <BALL/STRUCTURE/BONDORDERS/FPTBondOrderStrategy.h>
#include <BALL/STRUCTURE/BONDORDERS/KGreedyBondOrderStrategy.h>
#ifdef BALL_HAS_LPSOLVE
# include <BALL/STRUCTURE/BONDORDERS/ILPBondOrderStrategy.h>
#endif

//#define DEBUG 1
#undef DEBUG

//#define DEBUG_READ 1
#undef DEBUG_READ

//#define DEBUG_ESTIMATE 1
#undef DEBUG_ESTIMATE

//#define DEBUG_ADD_HYDROGENS 1
#undef DEBUG_ADD_HYDROGENS

//#define UNNORMALIZED_DEBUG 1
#undef UNNORMALIZED_DEBUG

//#define DEBUG_RULES
#undef DEBUG_RULES

//#define DEBUG_PRINT_RULES
#undef DEBUG_PRINT_RULES

//#define DEBUG_BOND_LENGTH
#undef DEBUG_BOND_LENGTH

//#define DEBUG_TIMER
#undef DEBUG_TIMER

#define INFINITE_PENALTY 1e5

using namespace std;

namespace BALL
{
	const String AssignBondOrderProcessor::Algorithm::A_STAR = "a_star";
	const String AssignBondOrderProcessor::Algorithm::ILP = "ilp";
	const String AssignBondOrderProcessor::Algorithm::K_GREEDY = "k_greedy";
	const String AssignBondOrderProcessor::Algorithm::BRANCH_AND_BOUND = "branch_and_bound";
	const String AssignBondOrderProcessor::Algorithm::FPT = "fpt";

	const char* AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS = "overwrite_single_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_SINGLE_BOND_ORDERS = true;

	const char* AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS = "overwrite_double_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_DOUBLE_BOND_ORDERS = true;

	const char* AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS = "overwrite_triple_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_TRIPLE_BOND_ORDERS = true;

	const char* AssignBondOrderProcessor::Option::OVERWRITE_SELECTED_BONDS = "overwrite_selected_bonds";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_SELECTED_BONDS = false;

	const char* AssignBondOrderProcessor::Option::ADD_HYDROGENS = "add_hydrogens_by_processor";
	const bool  AssignBondOrderProcessor::Default::ADD_HYDROGENS = false;

	const char* AssignBondOrderProcessor::Option::COMPUTE_ALSO_CONNECTIVITY = "compute_also_connectivity";
	const bool AssignBondOrderProcessor::Default::COMPUTE_ALSO_CONNECTIVITY = false;

	const char* AssignBondOrderProcessor::Option::CONNECTIVITY_CUTOFF = "bond_connectivity_cutoff";
	const float AssignBondOrderProcessor::Default::CONNECTIVITY_CUTOFF = 2.4;

	const char* AssignBondOrderProcessor::Option::USE_FINE_PENALTY = "use_fine_penalty";
	const bool  AssignBondOrderProcessor::Default::USE_FINE_PENALTY = true;

	const char* AssignBondOrderProcessor::Option::KEKULIZE_RINGS = "kekulize_aromatic_rings";
	const bool  AssignBondOrderProcessor::Default::KEKULIZE_RINGS = true;

	const char* AssignBondOrderProcessor::Option::INIFile = "iniFile";
	const String  AssignBondOrderProcessor::Default::INIFile = "/bond_lengths/BondOrder.xml";

	const char* AssignBondOrderProcessor::Option::MAX_BOND_ORDER = "max_bond_order";
	const int  AssignBondOrderProcessor::Default::MAX_BOND_ORDER = 3;

	const char* AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS = "max_number_of_solutions";
	const int  AssignBondOrderProcessor::Default::MAX_NUMBER_OF_SOLUTIONS = 10;

	const char* AssignBondOrderProcessor::Option::MAX_PENALTY = "max_penalty_score";
	const int  AssignBondOrderProcessor::Default::MAX_PENALTY = -1;

	const char* AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS = "compute_also_non_optimal_solutions";
	const bool  AssignBondOrderProcessor::Default::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS = false;

	const char* AssignBondOrderProcessor::Option::ALGORITHM = "algorithm";
	const String AssignBondOrderProcessor::Default::ALGORITHM = AssignBondOrderProcessor::Algorithm::A_STAR;

	const char* AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING = "bond_length_weighting";
	const float AssignBondOrderProcessor::Default::BOND_LENGTH_WEIGHTING = 0.;

	const char* AssignBondOrderProcessor::Option::APPLY_FIRST_SOLUTION = "apply_first_solution";
	const bool  AssignBondOrderProcessor::Default::APPLY_FIRST_SOLUTION = true;


	AssignBondOrderProcessor::AssignBondOrderProcessor()
		: UnaryProcessor<AtomContainer>(),
			options(),
			valid_(true),
			evaluation_mode_(false),
			bond_fixed_(),
			free_bonds_(),
			bond_to_index_(),
			index_to_bond_(),
			number_of_virtual_hydrogens_(),
			virtual_bond_index_to_number_of_virtual_hydrogens_(),
			num_of_virtual_bonds_(0),
			virtual_bond_index_to_atom_(),
			atom_to_virtual_bond_index_(),
			virtual_bond_(NULL),
			total_num_of_bonds_(0),
			num_of_free_bonds_(0),
			fixed_val_(),
			solutions_(),
			starting_configuration_(),
			atom_type_normalization_factor_(0.),
			bond_length_normalization_factor_(0.),
			last_applied_solution_(-1),
			ac_(0),
			max_bond_order_(),
			alpha_(),
			max_number_of_solutions_(),
			max_penalty_(-1),
			compute_also_non_optimal_solutions_(),
			add_missing_hydrogens_(),
			compute_also_connectivity_(),
			use_fine_penalty_(),
			penalties_(),
			block_to_start_idx_(),
			block_to_length_(),
			block_to_start_valence_(),
			block_definition_(),
			atom_to_block_(),
			bond_lengths_penalties_(),
			timer_()
	{
		strategies_["AStar"] = boost::shared_ptr<BondOrderAssignmentStrategy>(new AStarBondOrderStrategy(this));
		strategies_["KGreedy"] = boost::shared_ptr<BondOrderAssignmentStrategy>(new KGreedyBondOrderStrategy(this));
		strategies_["BranchAndBound"] = boost::shared_ptr<BondOrderAssignmentStrategy>(new BranchAndBoundBondOrderStrategy(this));
		strategies_["FPT"] = boost::shared_ptr<BondOrderAssignmentStrategy>(new FPTBondOrderStrategy(this));
#ifdef BALL_HAS_LPSOLVE
		strategies_["ILP"] = boost::shared_ptr<BondOrderAssignmentStrategy>(new ILPBondOrderStrategy(this));
#endif

		setDefaultOptions();
	}

	AssignBondOrderProcessor::~AssignBondOrderProcessor()
	{
		clear();

		delete(virtual_bond_);
	}

	void AssignBondOrderProcessor::clear()
	{
		for (StringHashMap<boost::shared_ptr<BondOrderAssignmentStrategy> >::Iterator s_it = strategies_.begin();
		     s_it != strategies_.end();
				 ++s_it)
		{
			s_it->second->clear();
		}

		//NOTE: options should remain!!
		valid_ = true;
		evaluation_mode_ = false;
		bond_fixed_.clear();
		free_bonds_.clear();
		bond_to_index_.clear();
		index_to_bond_.clear();
		number_of_virtual_hydrogens_.clear();
		virtual_bond_index_to_number_of_virtual_hydrogens_.clear();
		num_of_virtual_bonds_ = 0;
		virtual_bond_index_to_atom_.clear();
		atom_to_virtual_bond_index_.clear();

		delete(virtual_bond_);
		virtual_bond_ = NULL;

		total_num_of_bonds_ = 0;
		num_of_free_bonds_ = 0;

		fixed_val_.clear();
		solutions_.clear();
		starting_configuration_.clear();
		atom_type_normalization_factor_ = 0.;
		bond_length_normalization_factor_ = 0.;

		last_applied_solution_ = -1;
		ac_ = 0;

		penalties_.clear();
		block_to_start_idx_.clear();
		block_to_length_.clear();
		block_to_start_valence_.clear();
		block_definition_.clear();
		atom_to_block_.clear();
		bond_lengths_penalties_.clear();
		timer_.clear();
	}

	bool AssignBondOrderProcessor::readOptions_()
	{
		bool ret = true;

		for (StringHashMap<boost::shared_ptr<BondOrderAssignmentStrategy> >::Iterator s_it = strategies_.begin();
		     s_it != strategies_.end();
				 ++s_it)
		{
			ret &= s_it->second->readOptions(options);
		}

		max_bond_order_            = options.getInteger(Option::MAX_BOND_ORDER);
		alpha_                     = options.getReal(Option::BOND_LENGTH_WEIGHTING);
		max_number_of_solutions_   = options.getInteger(Option::MAX_NUMBER_OF_SOLUTIONS);
		max_penalty_               = options.getInteger(Option::MAX_PENALTY);
		compute_also_non_optimal_solutions_ = options.getBool(Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS);
		add_missing_hydrogens_     = options.getBool(Option::ADD_HYDROGENS);
		compute_also_connectivity_ = options.getBool(Option::COMPUTE_ALSO_CONNECTIVITY);
		use_fine_penalty_          = options.getBool(Option::USE_FINE_PENALTY);

		if (max_bond_order_ <= 0)
		{
			Log.error() << __FILE__ << " " << __LINE__
				          << " : Error in options! Please check the option Option::MAX_BOND_ORDER."  << endl;
			ret = false;
		}
		if (max_number_of_solutions_ < 0)
		{
			Log.error() << __FILE__ << " " << __LINE__
				          << " : Error in options! Please check the option Option::MAX_NUMBER_OF_SOLUTIONS."  << endl;
			ret = false;
		}
		if (max_penalty_ < -1)
		{
			Log.error() << __FILE__ << " " << __LINE__
				          << " : Error in options! Please check the option Option::MAX_PENALTY."  << endl;
			ret = false;
		}
		if ((alpha_ < 0) || ((alpha_ > 1)))
		{
			Log.error() << __FILE__ << " " << __LINE__
				          << " : Error in options! Please check the option Option::BOND_LENGTH_WEIGHTING."  << endl;
			ret = false;
		}

		if (options.getReal(Option::CONNECTIVITY_CUTOFF) < 0)
		{
			Log.error() << __FILE__ << " " << __LINE__
				          << " : Error in options! Please check the option Option::CONNECTIVITY_CUTOFF."  << endl;
			ret = false;
		}

		// the FPT extra cases:
		if (   (options.get(Option::ALGORITHM) == Algorithm::FPT)
				&& (  // (options.getBool(Option::USE_FINE_PENALTY) == true) ||
						   (options.getReal(Option::BOND_LENGTH_WEIGHTING) > 0.)
						|| (options.getBool(Option::ADD_HYDROGENS) == true)
						|| (options.getBool(Option::COMPUTE_ALSO_CONNECTIVITY) == true)
						|| (options.getBool(Option::OVERWRITE_SELECTED_BONDS)  == true)
						|| (options.getBool(Option::OVERWRITE_SINGLE_BOND_ORDERS) == false)
						|| (options.getBool(Option::OVERWRITE_DOUBLE_BOND_ORDERS) == false)
						|| (options.getBool(Option::OVERWRITE_TRIPLE_BOND_ORDERS) == false) )
			 )
		{
			Log.error() << __FILE__ << " " << __LINE__
				          << " : Error in options! FPT cannot be used with these option(s): ";
			if (options.getReal(Option::BOND_LENGTH_WEIGHTING) > 0.)
				Log.error() << "BOND_LENGTH_WEIGHTING ";
			if (options.getBool(Option::ADD_HYDROGENS))
				Log.error() << "ADD_HYDROGENS ";
			if (options.getBool(Option::COMPUTE_ALSO_CONNECTIVITY))
				Log.error() << "COMPUTE_ALSO_CONNECTIVITY ";
			if (options.getBool(Option::OVERWRITE_SELECTED_BONDS))
				Log.error() << "OVERWRITE_SELECTED_BONDS ";
			if (!options.getBool(Option::OVERWRITE_SINGLE_BOND_ORDERS))
				Log.error() << "OVERWRITE_SINGLE_BOND_ORDERS ";
			Log.error() << endl;

			Log.error() << " Consider switch to solution strategy ASTAR by setting Option::ALGORITHM to Algorithm::ASTAR." << endl
									<< " Abort." << endl;
			ret = false;
		}

		valid_ = ret;
		return ret;
	}

	bool AssignBondOrderProcessor::start()
	{
		clear();
		valid_ = readAtomPenalties_();
		evaluation_mode_ = false;
		return true;
	}

	Processor::Result AssignBondOrderProcessor::operator () (AtomContainer& ac)
	{
#if defined DEBUG_TIMER	
		timer_.start();
#endif

#ifdef DEBUG 
cout << "  OPTIONS:" << endl;
cout << " \t Algorithm: " <<  options[Option::ALGORITHM] << endl;
cout << " \t Heuristic: " <<  options[Option::HEURISTIC] << endl;

cout << " \t Overwrite bonds (single, double, triple, selected):"
		 << options.getBool(Option::OVERWRITE_SINGLE_BOND_ORDERS) << " "
		 << options.getBool(Option::OVERWRITE_DOUBLE_BOND_ORDERS) << " "
		 << options.getBool(Option::OVERWRITE_TRIPLE_BOND_ORDERS) << " "
		 << options.getBool(Option::OVERWRITE_SELECTED_BONDS) << " "
		<< endl;

cout << " \t Add hydrogens : " << options.getBool(Option::ADD_HYDROGENS) << endl;
cout << " \t Use fine penalty : " << options.getBool(Option::USE_FINE_PENALTY) << endl;
cout << " \t Kekulizer: " << options.getBool(Option::KEKULIZE_RINGS)  << endl;
cout << " \t Penalty file " << options[Option::INIFile] << endl;
cout << " \t alpha: " << options[Option::BOND_LENGTH_WEIGHTING] << endl;
cout << " \t max bond order: " << options[Option::MAX_BOND_ORDER] << endl;
cout << " \t max number of solutions " << options[Option::MAX_NUMBER_OF_SOLUTIONS] << endl;
cout << " \t max penalty " << options[Option::MAX_PENALTY] << endl;
cout << " \t compute also non-optimal solutions: " << options.getBool(Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS) << endl;
cout << " \t compute also connectivity: " << options.getBool(Option::COMPUTE_ALSO_CONNECTIVITY) << endl;
cout << " \t connectivity cutoff: " << options[Option::CONNECTIVITY_CUTOFF] << endl;
cout << " \t apply first solution: " <<  options.getBool(Option::APPLY_FIRST_SOLUTION) << endl;
cout << " \t size of the greedy priority queue: " << options[Option::GREEDY_K_SIZE] << endl;
cout << " \t branch_and_bound_cutoff: " << options[Option::BRANCH_AND_BOUND_CUTOFF] << endl;
cout << " \t valid : " << valid_ << endl;
cout << endl;
#endif

		// Do we have bonds in the molecule at all?
		if (ac.countBonds() == 0)
			return Processor::CONTINUE;

		// check if single connected component, otherwise treedecomposition crashes
		ConnectedComponentsProcessor ccp;
		ac.apply(ccp);
		if (ccp.getNumberOfConnectedComponents() > 1)
		{
			Log.error() << "Error: Given AtomContainer contains more than one connected component, but only one is expected! Abort. " <<
			               __FILE__ << " " << __LINE__<< std::endl;
			return Processor::ABORT;
		}

		// Is the processor in a valid state?
		if (readOptions_() && valid_)
		{
			// Speed up the code by temporarily storing the options locally 
			bool overwrite_single_bonds   = options.getBool(Option::OVERWRITE_SINGLE_BOND_ORDERS);
			bool overwrite_double_bonds   = options.getBool(Option::OVERWRITE_DOUBLE_BOND_ORDERS);
			bool overwrite_triple_bonds   = options.getBool(Option::OVERWRITE_TRIPLE_BOND_ORDERS);
			bool overwrite_selected_bonds = options.getBool(Option::OVERWRITE_SELECTED_BONDS);

			// check the options

			// What kind of composite do we have?
			// Do we have a molecule? (Nothing else is allowed)
			if (RTTI::isKindOf<Molecule>(ac))
			{
				// Store the AtomContainer
				ac_ = &ac;

				// Store the original bond orders as a "special" solution
				storeOriginalConfiguration_();

				// Store the total number of bonds
				total_num_of_bonds_ = ac.countBonds();

				// Store the Bonds indices 
				index_to_bond_.resize(total_num_of_bonds_, (Bond*)0);

				// Number of fixed bonds
				Size num_fixed_bonds = 0;

				// Number of atoms in the system
				Position no_atoms = ac.countAtoms();

				// Vector for storing fixed atom valences
				fixed_val_ = std::vector<Position>(no_atoms, 0);

				// Enumerate all bonds
				Index bond_index_counter = 0;

				// Find out, which bonds should be considered in general
				for (Position i = 0; i < no_atoms; ++i)
				{
					Atom* at1 = ac.getAtom(i);
					Position fixed = 0;

					for (Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
					{
						Bond* bnd = &(*bit);

						// To speed up the computations, assign each bond a unique index
						if (bond_to_index_.find(bnd) == bond_to_index_.end())
						{
							bond_to_index_[bnd] = bond_index_counter;
							index_to_bond_[bond_index_counter] = bnd;
							bond_index_counter++;
						}

						if (overwrite_selected_bonds)
						{
							if (bnd->isSelected())
							{
								bond_fixed_[bnd] = 0;
							}
							else
							{
								fixed += 1;
								bond_fixed_[bnd] = 1;
								num_fixed_bonds++;
							}
						}
						else
						{
							// According to the options and current bond order 
							// a bond is a free variable of the ILP or not.
							// YES: add a variable in the bond side constraint   
							// NO: equality in the bonds side constraint 
							switch (bnd->getOrder())
							{
								case Bond::ORDER__SINGLE:
								{
									if (overwrite_single_bonds)
									{
										bond_fixed_[bnd] = 0;
									}
									else
									{
										fixed += 1;
										bond_fixed_[bnd] = 1;
										num_fixed_bonds++;
									}
									break;
								}
								case Bond::ORDER__DOUBLE:
								{
									if (overwrite_double_bonds)
									{
										bond_fixed_[bnd] = 0;
									}
									else
									{
										fixed += 2;
										bond_fixed_[bnd] = 2;
										num_fixed_bonds++;
									}
									break;
								}
								case Bond::ORDER__TRIPLE:
								{
									if (overwrite_triple_bonds)
									{
										bond_fixed_[bnd] = 0;
									}
									else
									{
										fixed += 3;
										bond_fixed_[bnd] = 3;
										num_fixed_bonds++;
									}
									break;
								}
								case Bond::ORDER__QUADRUPLE:
								{
									fixed += 4;
									bond_fixed_[bnd] = 4;
									num_fixed_bonds++;
									break;
								}
								default: //Bond::ORDER__UNKNOWN:
								{
									bond_fixed_[bnd] = 0;
								}
							}
						}
					}

					fixed_val_[i] = fixed;
				}

				// NOTE: we count each bond twice (one for each atom), hence we divide num_fixed_bonds by 2
				num_of_free_bonds_ = total_num_of_bonds_ - num_fixed_bonds/2;

#if defined DEBUG || DEBUG_READ
cout << "preassignPenaltyClasses_:" << preassignPenaltyClasses_() << " precomputeBondLengthPenalties_:" << precomputeBondLengthPenalties_() << endl;
#endif

				// Generate penalty values for all atoms in the AtomContainer ac
				if (preassignPenaltyClasses_() && precomputeBondLengthPenalties_())
				{
#if defined DEBUG_TIMER	
					cout << "TIMER: Rule-Assignment: " << timer_.getClockTime() << endl;
					timer_.reset();
#endif

					BondOrderAssignmentStrategy* strategy = NULL;
					if (options.get(Option::ALGORITHM) == Algorithm::A_STAR)
					{
						strategy = strategies_["AStar"].get();
					}
					else if (options.get(Option::ALGORITHM) == Algorithm::K_GREEDY)
					{
						strategy = strategies_["KGreedy"].get();
					}
					else if (options.get(Option::ALGORITHM) == Algorithm::BRANCH_AND_BOUND)
					{
						strategy = strategies_["BranchAndBound"].get();
					}
					else if (options.get(Option::ALGORITHM) == Algorithm::FPT)
					{
						strategy = strategies_["FPT"].get();
					}
					else if (options.get(Option::ALGORITHM) == Algorithm::ILP)
					{
#ifdef BALL_HAS_LPSOLVE
						strategy = strategies_["ILP"].get();
#else
						Log.error() << "Error: BALL was configured without lpsolve support! Try A_STAR or FPT instead!" <<
												__FILE__ << " " << __LINE__<< std::endl;

						return Processor::ABORT;
#endif
					}

					if (!strategy)
					{
						Log.error() << __FILE__ << " " << __LINE__ << ": no valid algorithm specified." << endl;

						return Processor::ABORT;
					}

					// Initialize the strategy
					strategy->init();

					// Try to find a first solution
					boost::shared_ptr<BondOrderAssignment> solution = strategy->computeNextSolution();

					// Do we have a solution? 
					if (!solution || !solution->valid)
					{
						Log.info() << "AssignBondOrderProcessor: No valid bond order assignment found!" << endl;
#if defined DEBUG_TIMER					
						timer_.stop();
						cout << "TIMER: Algorithm: " << timer_.getClockTime() << endl;
#endif
					}
					else
					{
						// store the one we found
						solutions_.push_back(*solution);

						// Do we have to find more solutions?
						bool found_another        = true;
						bool last_sol_is_optimal  = true;
						bool consider_max_penalty = (max_penalty_ > -1);
						double last_penalty       = getTotalPenalty(0);

						while (    found_another
						        && ((getNumberOfComputedSolutions() < max_number_of_solutions_) || (!max_number_of_solutions_))
						        && (last_sol_is_optimal || (compute_also_non_optimal_solutions_))
										&& ((last_penalty <= max_penalty_) || (!consider_max_penalty))
						      )
						{
							found_another = computeNextSolution(options.getBool(Option::APPLY_FIRST_SOLUTION));
							last_sol_is_optimal &= (fabs(getTotalPenalty(0) - getTotalPenalty(solutions_.size()-1)) < 1.e-4);
							last_penalty  = getTotalPenalty(solutions_.size()-1);
						}
#if defined DEBUG_TIMER		
					timer_.stop();
					cout << "TIMER: Algorithm: " << timer_.getClockTime() << endl;
#endif
					}

					if (!solutions_.empty())
					{
						if (options.getBool(Option::APPLY_FIRST_SOLUTION))
						{
							apply(0);
						}
					}
				} // end of if preassign worked out
			} // end of if molecule
		}

		return Processor::CONTINUE;
	}

	bool AssignBondOrderProcessor::finish()
	{
		return true;
	}


	bool AssignBondOrderProcessor::readAtomPenalties_()
		throw(Exception::FileNotFound())
	{
		// Open parameter file
		Path    path;
		String  inifilename(path.find(options[Option::INIFile]));

		if (inifilename == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, options[Option::INIFile]);
		}

		QString errorStr;
		int errorLine;
		int errorColumn;

		QFile file((inifilename.c_str()));
		if (!file.open(QFile::ReadOnly | QFile::Text))
		{
			Log.error() << "Error: cannot read file " << inifilename <<" " << __FILE__ << " " << __LINE__ << std::endl;
			Log.error() << "Reason was: " << file.errorString().toAscii().constData() << std::endl;
			return 1;
		}

		// read the document
		QDomDocument domDocument;
		if (!domDocument.setContent(&file, true, &errorStr, &errorLine,
					&errorColumn))
		{
			Log.error() << "Parse error in line " << errorLine << " column " << errorColumn <<  " of file " << inifilename << endl;
			Log.error() << "Reason was: " << errorStr.toAscii().constData() << std::endl;
			return 1;
		}

		// get the root element...
		QDomElement root = domDocument.documentElement();

		// ... and get all entries
		QDomNodeList entries = domDocument.elementsByTagName("entry");
		for (unsigned int i= 0; i < entries.length(); i++)
		{
			pair<String, String> tmp;
			int start_valence;
			QDomNodeList penalties;
			Position start_idx;

			// get the element type (tag elementstring) 
			// NOTE: each entry should have just ONE element tag)
			QDomNodeList elementstrings = entries.item(i).toElement().elementsByTagName("elementstring");
			if (elementstrings.length() ==1)
			{
				// read the element type
				QDomNode element = elementstrings.item(0);
				tmp.first = element.firstChild().nodeValue().toAscii().constData();

				// read the SMARTS-string
				QDomNodeList smartstring =  entries.item(i).toElement().elementsByTagName("smartstring");
				if (smartstring.length() == 1)
				{
					tmp.second = smartstring.item(0).toElement().firstChild().nodeValue().toAscii().constData();
				}
				else if (smartstring.length() == 0)
				{
					Log.warn() << "In file " << inifilename << " : no SMARTS-string found for element " << element.firstChild().nodeValue().toAscii().constData() << endl;
				}
				else
				{
					Log.error() <<  "Parse error in file " << inifilename << " : more than on3 SMARTS-string for element " << element.firstChild().nodeValue().toAscii().constData() << endl;
					return false;
				}

				// now read the penalties
				penalties =  entries.item(i).toElement().elementsByTagName("penalty");
				if (penalties.length() > 0)
				{
					start_valence = (penalties.item(0).toElement().attribute("valence")).toInt();
					start_idx = penalties_.size();
					for (unsigned int k = 0; k < penalties.length(); k++)
					{
						// NOTE: we assume, that the valences come "en block" 
						// without any leftouts 
						penalties_.push_back((penalties.item(k).toElement().firstChild().nodeValue()).toInt());
					}
				}
				else
				{
					Log.error() << "In file " << inifilename << " : no penalties found for element " << element.firstChild().nodeValue().toAscii().constData() << endl;
					return false;
				}
			}
			else
			{
				Log.error() << "Parse error in file " << inifilename << endl;
				return false;
			}

			// now store the entry
			block_definition_.push_back(tmp);
			block_to_length_.push_back(penalties.length());
			block_to_start_idx_.push_back(start_idx);
			block_to_start_valence_.push_back(start_valence);

		} // next block

		return true;
	}

	bool AssignBondOrderProcessor::precomputeBondLengthPenalties_()
	{
#if defined DEBUG || defined DEBUG_ESTIMATE
cout << " AssignBondOrderProcessor::precomputeBondLengthPenalties_:   " << endl;
#endif

		// this is a heuristic value that worked quite ok in our tests
		float diff_cutoff = 0.5;

		// get the relevant options
		// this is also done in the start-method, 
		// but when performing an evalution the start-method is not called!
		max_bond_order_ = options.getInteger(Option::MAX_BOND_ORDER);
		alpha_ = options.getReal(Option::BOND_LENGTH_WEIGHTING);

		// initialize the inverse bond length penalty normalization factor
		bond_length_normalization_factor_ = 0.;

		if (!valid_)
		{
			Log.error() << "Error: The processors state is invalid. " << __FILE__ << " " << __LINE__ << endl;
			return false;
		}

		// determine for each bond its average bond lengths	
		// and store them in bond_lengths_penalties_
		if (ac_)
		{
			// read the bond length parameters
			Path path;
			String pfilename(path.find("bond_lengths/bond_lengths_mmff94.ini"));

			Parameters parameter_file(pfilename);

			ParameterSection bond_lengths;
			bond_lengths.extractSection(parameter_file, "BondLengths");

			AtomIterator a_it = ac_->beginAtom();
			Atom::BondIterator b_it = a_it->beginBond();
			BALL_FOREACH_BOND(*ac_, a_it, b_it)
			{
				// the precomputed bond length penalties
				vector<float> penalties(max_bond_order_ + 1, 0.);
				// the bond atoms 
				String atom_type_1 = b_it->getFirstAtom()->getElement().getSymbol();
				String atom_con_1(b_it->getFirstAtom()->countBonds());

				String atom_type_2 = b_it->getSecondAtom()->getElement().getSymbol();
				String atom_con_2(b_it->getSecondAtom()->countBonds());

				String key = atom_type_1+" "+atom_con_1+" "+atom_type_2+" "+atom_con_2+" ";

				// the current max and min bond length deviation
				// bonds without predefined lenght penalty will get penalty zero!
				// This is no problem for the heuristic, since this bond will
				// get zero in all PQEntries.
				float max_bond_length_deviation = 0.;
				float min_bond_length_deviation = std::numeric_limits<float>::max();

				bool complete = true;
				max_bond_length_deviation = 0.;

				// the current bond length
				float bond_length = b_it->getLength();
				Size min_order = 0;

#ifdef  DEBUG_BOND_LENGTH
cout << b_it->getSecondAtom()->getFullName() << "-" << b_it->getOrder() <<"-" <<  b_it->getFirstAtom()->getFullName() << endl;
#endif
				// for all possible bond orders, precompute the penalties
				// here we try square deviation
				// NOTE: we omit the aromatic bonds!
				for (Position i = 1; i <= (Position)max_bond_order_; i++)
				{
					String current_key = key+String(i);

					if (bond_lengths.has(current_key, "r0"))
					{
						const String& bond_length_string = bond_lengths.getValue(current_key, "r0");
						penalties[i] = pow((bond_length - bond_length_string.toFloat()),(int)2);

#ifdef  DEBUG_BOND_LENGTH
cout << "        order " << i << " : " << penalties[i] << endl;
#endif
						if (max_bond_length_deviation < penalties[i])
						{
							max_bond_length_deviation = penalties[i];
						}
						if (min_bond_length_deviation > penalties[i])
						{
							min_bond_length_deviation = penalties[i];
							min_order = i;
						}
					}
					else
					{
						complete = false;
						penalties[i] = -1.;
					}
				}

				// store the min at position zero
				penalties[0] = min_bond_length_deviation;

				// in case we got no complete set, we assume 
				// the missing bond orders are really unlikely
				// and we set a penalty to 2*max_deviation_found (for this bond)
				if (!complete)
				{
#ifdef  DEBUG_BOND_LENGTH
	cout << "   NOT COMPLETE" << endl;
#endif
					max_bond_length_deviation *= 2.;

					for (Position i = 1; i <= (Position)max_bond_order_; i++)
					{
						if (penalties[i] < 0)
						{
							penalties[i] = max_bond_length_deviation;
						}
					}

					// filter our irrelevant penalty differences 
					// like aromatic bonds stuck between order 1 and 2
					for (Position i = 1; i < (Position)max_bond_order_; i++)
					{
						if (fabs(penalties[i]-penalties[i+1]) < diff_cutoff * (penalties[i]+penalties[i+1]))
						{
#ifdef	DEBUG_BOND_LENGTH
							Log.info() << "     penalty for order " << i+1 << " changed to that of order " << i << endl;
#endif
							penalties[i+1] = penalties[i];
						}
					}
				}

				//store the b_it's penalties in the hashmap
				//else: since we have no information, we handle 
				//every bond order the same, namely assign 0. :-)	
				bond_lengths_penalties_[&(*b_it)] = penalties;

				// add the bonds max deviation to the inverse bond length normalization factor
				bond_length_normalization_factor_ += max_bond_length_deviation;
			} // end of for all bonds
		}
		else
		{
			valid_ = false;
			return false;
		}

		return true;
	}

	// finds the index of the __first__ matching penalty definition 
	// returns -1 otherwise 
	int AssignBondOrderProcessor::getPenaltyClass_(Atom* atom)
	{
		// find the __first__ matching atom definition, whose index gives the block
		for (Size j = 0; j < block_definition_.size(); j++)
		{
			Expression exp(block_definition_[j].second);
			if (   (atom->getElement().getSymbol() ==  block_definition_[j].first)
					&& (exp(*atom)))
			{

#ifdef DEBUG_READ
cout << "getPenaltyClass_ HIT   " << atom->getFullName() << " with index " << atom->getIndex() << " assigned to block: " << j+1 << " : "  << block_definition_[j].first << "  "	<<  block_definition_[j].second << endl;
#endif
				// NOTE: we start counting at 0!
				return j;
			}
		}
		Log.info() << "Warning: No penalty type found for atom "
		           << atom->getFullName() << " in " << __FILE__ << " " <<  __LINE__ << endl;

		return -1;
	}

	bool AssignBondOrderProcessor::preassignPenaltyClasses_()
	{
#if defined DEBUG_READ 
cout << " AssignBondOrderProcessor::preassignPenaltyClasses_()" << endl;
#endif

		if (!valid_)
		{
			Log.error() << "AssignBondOrderProcessor: The processors state is invalid." << endl;
			return false;
		}

		// Determine for each atom the corresponding valence- and penalty-block	
		if (ac_)
		{
			// get the number of atoms
			Position no_atoms = ac_->countAtoms();

			// resize the data structure
			atom_to_block_.clear(); // needed in case this function is called twice!
			vector<int> tmp(1,-1);
			atom_to_block_.resize(no_atoms,tmp);

			// initialize the inverse atom type penalty normalization factor 
			atom_type_normalization_factor_ = 0.;

			// create the dummy bond needed for all virtual bonds 
			if (add_missing_hydrogens_)
			{
				if (!virtual_bond_)
				{
					virtual_bond_ = new Bond;
					virtual_bond_->setProperty("VIRTUAL__BOND", true);
					index_to_bond_.push_back(virtual_bond_);
					bond_to_index_[virtual_bond_] = index_to_bond_.size()-1;
					bond_fixed_[virtual_bond_] = 0;
				}

				// clear the virtual bond stuff
				num_of_virtual_bonds_ = 0;
				number_of_virtual_hydrogens_.clear();
				virtual_bond_index_to_number_of_virtual_hydrogens_.clear();
				virtual_bond_index_to_atom_.clear();
				atom_to_virtual_bond_index_.clear();
			}

			// for each atom
			for (Position i = 0; i < no_atoms; ++i)
			{
				Atom* at = ac_->getAtom(i);

				// for reasons only known to BALL, getIndex yields completely strange results for
				// the atoms here, so we need to add a Property which we can later read out
				ac_->getAtom(i)->setProperty("AssignBondOrderProcessorAtomNo", i);
				bool found = false;

				// add maximal penalty to the inverse atom type normalization factor
				float max_penalty_per_atom = 0;

				// find the __first__ matching atom definition, whose index gives the block
				for (Size j = 0; !found && (j < block_definition_.size()); j++)
				{
					Expression exp(block_definition_[j].second);

					if (   (at->getElement().getSymbol() ==  block_definition_[j].first)
							&& (exp(*at)))
					{
#ifdef DEBUG_READ
cout << "preassignPenaltyClasses_() HIT : " << at->getFullName() << " with index " << at->getIndex() << " assigned to block  "<< j+1 << " : " << block_definition_[j].first << "   "
		<<  block_definition_[j].second << endl;
#endif
#ifdef DEBUG_PRINT_RULES
cout << at->getFullName() << " " << at->getPosition() << "  assigned to block  " << j+1 << " : " << block_definition_[j].first << "   " <<  block_definition_[j].second << " --- penalties: (" ;
int current_start_index   = block_to_start_idx_[j];
int current_start_valence = block_to_start_valence_[j];

if (current_start_valence !=0)
{
	cout << "-";
}

for(Size k=1; k < current_start_valence; k++)
{
	cout << ", - ";
}
for(Size k = 0; k < block_to_length_[j]; k++)
{
	cout << ", " << penalties_[k + block_to_start_idx_[j]];
}
cout << ") " <<  endl;
#endif

#ifdef DEBUG_RULES
	int valence = 0;
	Atom::BondIterator b_it;
	for (b_it=at->beginBond(); b_it != at->endBond(); b_it++)
	{
		valence+=b_it->getOrder();
	}

	int current_start_index   = block_to_start_idx_[j];
	int current_start_valence = block_to_start_valence_[j];

	if (penalties_[current_start_index + valence  - current_start_valence] > 0)
	{
		cout <<   at->getFullName()  << " :  Rule " << j+1 << "  penalty: " << penalties_[current_start_index + valence  - current_start_valence] << endl;
	}
#endif

						// store the penalty block's index
						// NOTE: we start counting at 0!
						atom_to_block_[i][0]=j;
						found = true;

						for(Size k = 0; k < block_to_length_[j]; k++)
						{
							float current_pen = penalties_[k + block_to_start_idx_[j]];
							max_penalty_per_atom = std::max(max_penalty_per_atom, current_pen);
						}

					}
				}

				if (add_missing_hydrogens_)
				{
					float max_virtual_penalty = computeVirtualHydrogens_(ac_->getAtom(i));

					max_penalty_per_atom = std::max(max_virtual_penalty, max_penalty_per_atom);
				}

				if (max_penalty_per_atom >= 0)
					atom_type_normalization_factor_+= max_penalty_per_atom;
				else
					found = false;


				if (!found)
				{
					Log.error() << "AssignBondOrderProcessor: No penalty type found for atom "
					            << at->getFullName() << endl;

					atom_to_block_[i][0]=-1;
					return false;
				}
			}

#ifdef UNNORMALIZED_DEBUG
atom_type_normalization_factor_ = 0.;
#endif

#if defined DEBUG_ADD_HYDROGENS || defined DEBUG
cout << " ~~~~~~~~ added hydrogen dump ~~~~~~~~~~~~~~~ " << endl;
for (Size i=0; i < atom_to_block_.size() ; i++)
{
	Atom* at = ac_->getAtom(i);
	cout << at-> getFullName()<< " : ";
	for (Size j=0; j < atom_to_block_[i].size(); j++)
	{
		cout << atom_to_block_[i][j] << " ";
	}
	cout << endl;
}
cout << " ~~~~~~~~ added hydrogen dump ~~~~~~~~~~~~~~~~" << endl;
#endif

		}
		else
		{
			Log.error() << "Error: No valid AtomContainer. " << __FILE__ << " " << __LINE__ << endl;
			return false;
		}
		return true;
	}


	// NOTE: add the moment we assume, that this method is called 
	//       AFTER preassignPenaltyClasses() and precomputeBondLengthPenalties()
	float AssignBondOrderProcessor::computeVirtualHydrogens_(Atom* atom)
	{
	#if defined DEBUG_ADD_HYDROGENS || defined DEBUG
		cout << " AssignBondOrderProcessor::computeVirtualHydrogens_" << endl;
	#endif

		float max_penalty = 0;

		// iterate over all atoms and add a hydrogen atom and bond 
		// 	if suggested by the free valences. 
		int free_valences = 0;

		vector<Atom*> atoms_to_delete;

		int num_of_atoms = ac_->countAtoms() +1;

		if (  (atom->getElement() == PTE[Element::H])
				||(atom->getElement() == PTE[Element::He]))
			free_valences = atom->getElement().getGroup() - atom->countBonds();
		else
			free_valences = 18 - atom->getElement().getGroup() - atom->countBonds();

		if (free_valences > 0)
		{
			//
			//  insert this virtual bond into the processors speed-up data structures
			//
			// NOTE: a virtual bond represents all possible hydrogen bond for an atom
			num_of_virtual_bonds_++;
			// store the max number of possible additional hydrogens 
			number_of_virtual_hydrogens_[atom] = free_valences;
			virtual_bond_index_to_number_of_virtual_hydrogens_.push_back(free_valences);
			// connect the virtual bond to this atom via its index
			virtual_bond_index_to_atom_.push_back(atom);
			atom_to_virtual_bond_index_[atom] = num_of_virtual_bonds_ -1;

			// NOTE: these datastructures are for both original and virtual bonds!
			index_to_bond_.push_back(virtual_bond_);
			free_bonds_.push_back(virtual_bond_);

			for (int i = 1; i <= free_valences; i++)
			{
				// add a hydrogen atom  
				Atom* hydrogen = new Atom;
				hydrogen->setElement(PTE[Element::H]);
				hydrogen->setName((PTE[Element::H].getSymbol())+String(num_of_atoms));
				num_of_atoms++;
				hydrogen->setProperty("VIRTUAL__ATOM", true);
				if (atom->getFragment())
				{
					atom->getFragment()->insert(*hydrogen);
				}
				else
				{
					atom->getMolecule()->insert(*hydrogen);
				}
				atoms_to_delete.push_back(hydrogen);

				// and a bond to the atomcontainer
				Bond* new_bond = atom->createBond(*hydrogen);
				new_bond->setProperty("VIRTUAL__BOND", true);
				new_bond->setOrder(1);

				// update the penalty classes! 
				int penalty_class = getPenaltyClass_(atom);
				int atom_index = atom->getProperty("AssignBondOrderProcessorAtomNo").getUnsignedInt();
				atom_to_block_[atom_index].push_back(penalty_class);

				if (penalty_class != -1)
				{
					for(Size k = 0; k < block_to_length_[penalty_class]; k++)
					{
						float current_pen = penalties_[k + block_to_start_idx_[penalty_class]];
						max_penalty = std::max(max_penalty, current_pen);
					}
				}
			}

			// delete all virtual atoms and bonds
			for (Size i=0; i < atoms_to_delete.size(); i++)
			{
				delete(atoms_to_delete[i]);
			}
		}

		return max_penalty;
	}

	void AssignBondOrderProcessor::storeOriginalConfiguration_()
	{
		starting_configuration_.push_back(BondOrderAssignment(this));
		starting_configuration_.back().valid = true;
		starting_configuration_.back().ac = ac_;

		AtomIterator a_it = ac_->beginAtom();
		Atom::BondIterator b_it;
		BALL_FOREACH_BOND(*ac_, a_it, b_it)
		{
			starting_configuration_.back().bond_order_map[&*b_it] = b_it->getOrder();
		}
	}

	void AssignBondOrderProcessor::resetBondOrders()
	{
		for (Position i=0; i<starting_configuration_.size(); ++i)
			starting_configuration_[i].apply();
		last_applied_solution_ = -1;
	}

	void AssignBondOrderProcessor::setDefaultOptions()
	{
		for (StringHashMap<boost::shared_ptr<BondOrderAssignmentStrategy> >::Iterator s_it = strategies_.begin();
		     s_it != strategies_.end();
				 ++s_it)
		{
			s_it->second->setDefaultOptions();
		}

		options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS,
		                       AssignBondOrderProcessor::Default::OVERWRITE_SINGLE_BOND_ORDERS);

		options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS,
		                       AssignBondOrderProcessor::Default::OVERWRITE_DOUBLE_BOND_ORDERS);

		options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS,
		                       AssignBondOrderProcessor::Default::OVERWRITE_TRIPLE_BOND_ORDERS);

		options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_SELECTED_BONDS,
		                       AssignBondOrderProcessor::Default::OVERWRITE_SELECTED_BONDS);

		options.setDefaultBool(AssignBondOrderProcessor::Option::ADD_HYDROGENS,
		                       AssignBondOrderProcessor::Default::ADD_HYDROGENS);

		options.setDefaultBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_CONNECTIVITY,
		                       AssignBondOrderProcessor::Default::COMPUTE_ALSO_CONNECTIVITY);

		options.setDefaultReal(AssignBondOrderProcessor::Option::CONNECTIVITY_CUTOFF,
		                       AssignBondOrderProcessor::Default::CONNECTIVITY_CUTOFF);

		options.setDefaultBool(AssignBondOrderProcessor::Option::USE_FINE_PENALTY,
		                       AssignBondOrderProcessor::Default::USE_FINE_PENALTY);

		options.setDefaultBool(AssignBondOrderProcessor::Option::KEKULIZE_RINGS,
		                       AssignBondOrderProcessor::Default::KEKULIZE_RINGS);

		options.setDefault(AssignBondOrderProcessor::Option::INIFile,
		                   AssignBondOrderProcessor::Default::INIFile);

		options.setDefaultInteger(AssignBondOrderProcessor::Option::MAX_BOND_ORDER,
		                          AssignBondOrderProcessor::Default::MAX_BOND_ORDER);

		options.setDefaultInteger(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS,
		                          AssignBondOrderProcessor::Default::MAX_NUMBER_OF_SOLUTIONS);

		options.setDefaultInteger(AssignBondOrderProcessor::Option::MAX_PENALTY,
		                          AssignBondOrderProcessor::Default::MAX_PENALTY);

		options.setDefaultBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS,
		                       AssignBondOrderProcessor::Default::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS);

		options.setDefault(AssignBondOrderProcessor::Option::ALGORITHM,
		                   AssignBondOrderProcessor::Default::ALGORITHM);

		options.setDefaultReal(AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING,
		                       AssignBondOrderProcessor::Default::BOND_LENGTH_WEIGHTING);

		options.setDefaultBool(AssignBondOrderProcessor::Option::APPLY_FIRST_SOLUTION,
		                       AssignBondOrderProcessor::Default::APPLY_FIRST_SOLUTION);
	}

	bool AssignBondOrderProcessor::apply(Position i)
	{
		bool result = false;
		if (i < solutions_.size())
		{
			result = solutions_[i].apply();

			if (result)
			{
				// denote the application of this solution
				last_applied_solution_ = i;

				if (!options.getBool(AssignBondOrderProcessor::Option::KEKULIZE_RINGS))
				{
					// find all rings
					vector<vector<Atom*> > rings;
					RingPerceptionProcessor rpp;
					rpp.calculateSSSR(rings, *solutions_[i].ac);

					// set the aromatic rings	
					AromaticityProcessor ap;
					ap.options.setBool(AromaticityProcessor::Option::OVERWRITE_BOND_ORDERS, true);
					ap.aromatize(rings, *solutions_[i].ac);
				}
				else
				{
					// to be on the safe side, we have to mark all bonds we may have changed as non-aromatic
					for (Position i=0; i<free_bonds_.size(); ++i)
						free_bonds_[i]->clearProperty(Bond::IS_AROMATIC);
				}
			}
		}

		return result;
	}


	const System& AssignBondOrderProcessor::getSolution(Position i)
		throw(Exception::IndexOverflow)
	{
		if (i >= solutions_.size())
		{
			//Log.error() << "AssignBondOrderProcessor: No solution with index " << i << std::endl;
			Exception::IndexOverflow e(__FILE__, __LINE__, i, solutions_.size());

			throw(e);
		}
		else
		{
			// apply the i-th solution
			apply(i);

			// What kind of composite do we have?
			if (RTTI::isKindOf<System>(*solutions_[i].ac))
			{
				return *(RTTI::castTo<System>(*solutions_[i].ac));
			}
			else if (RTTI::isKindOf<Molecule>(*solutions_[i].ac))
			{
				Molecule* m = RTTI::castTo<Molecule>(*solutions_[i].ac);
				return *(m->getSystem());
			}
			else
			{
				Exception::GeneralException e(__FILE__, __LINE__);
				throw(e);
			}
		}
	}


	bool AssignBondOrderProcessor::computeNextSolution(bool apply_solution)
	{
		bool found_a_sol = false;

		if (add_missing_hydrogens_)
		{
			// the method apply() may have been called before
			// such that we have to delete the virtual hydrogens
			if (last_applied_solution_ >= 0)
			{
				for (Size j=0; j < solutions_[last_applied_solution_].atoms_to_delete.size(); j++)
				{
					//NOTE: all adajacent bonds of these atoms will be deleted automatically
					solutions_[last_applied_solution_].atoms_to_delete[j]->destroy();
				}
			}
		}

		BondOrderAssignmentStrategy* strategy = NULL;

		if (options.get(Option::ALGORITHM) == Algorithm::A_STAR)
		{
			strategy = strategies_["AStar"].get();
		}
		else if (options.get(Option::ALGORITHM) == Algorithm::K_GREEDY)
		{
			strategy = strategies_["KGreedy"].get();
		}
		else if (options.get(Option::ALGORITHM) == Algorithm::BRANCH_AND_BOUND)
		{
			strategy = strategies_["BranchAndBound"].get();
		}
		else if (options.get(Option::ALGORITHM) == Algorithm::FPT)
		{
			strategy = strategies_["FPT"].get();
		/*	Log.info() << "AssignBondOrderProcessor: strategy FPT does not support computeNextSolution(). " << endl
								 << "Please use the options Option::MAX_NUMBER_OF_SOLUTIONS or Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS " << endl
								 << "to compute additional solutions." << endl; */

		}
		else if (options.get(Option::ALGORITHM) == Algorithm::ILP)
		{
#ifdef BALL_HAS_LPSOLVE
			strategy = strategies_["ILP"].get();
#else
			Log.error() << "Error: BALL was configured without lpsolve support! Try A_STAR instead!" <<
									__FILE__ << " " << __LINE__<< std::endl;

			return Processor::ABORT;
#endif
		}

		if (!strategy)
		{
			Log.error() << __FILE__ << " " << __LINE__ << ": no valid algorithm specified." << endl;

			return Processor::ABORT;
		}

		boost::shared_ptr<BondOrderAssignment> solution = strategy->computeNextSolution();

		if (solution && solution->valid)
		{
			solutions_.push_back(*solution);
			found_a_sol = true;
		}
		else
		{
			found_a_sol = false;
		}

		if (found_a_sol)
		{
			if (apply_solution)
				apply(solutions_.size()-1);

			return true;
		}

		return false;
	}

	//--------------------------------------------------
	//
	//                the nested classes
	//
	//---------------------------------------------------


	// For testing 
	float AssignBondOrderProcessor::evaluatePenalty(AtomContainer* ac)
	{
		// NOTE: this function is *never* allowed to add missing hydrogens 
		//  => store its flag temporarily and set the flag again in the end
		bool add_missing_hydrogens_backup = add_missing_hydrogens_;
		add_missing_hydrogens_ = false;

		float ret = -1;

		evaluation_mode_ = true;
		if (valid_ && readAtomPenalties_())
		{
			// Assign penalty classes
			ac_ = ac;
			if (preassignPenaltyClasses_() && precomputeBondLengthPenalties_())
			{
				// set the static variables in the PQ entries for faster access

				PartialBondOrderAssignment entry(this);

				AtomIterator a_it = ac_->beginAtom();
				Atom::BondIterator b_it = a_it->beginBond();
				bond_to_index_.clear();
				index_to_bond_.clear();
				index_to_bond_.resize(ac->countBonds(),0);
				entry.bond_orders.resize(ac->countBonds(),-1);
				int i=0;

				BALL_FOREACH_BOND(*ac, a_it, b_it)
				{
					if (bond_to_index_.find(&(*b_it)) == bond_to_index_.end())
					{
						bond_to_index_[&(*b_it)] = i;
						index_to_bond_[i] = &(*b_it);
						entry.bond_orders[i] = (short)b_it->getOrder();
						i++;
					}
				}

				entry.last_bond = ac->countBonds()-1;

				if (entry.estimatePenalty_(false))
				{
					evaluation_mode_ = false;
					ret = entry.coarsePenalty();

#ifdef  DEBUG_BOND_LENGTH
cout << "coarse: " << ret << "  fine" << entry.finePenalty() << endl;
#endif
				}
				else
				{
					Log.info() << "AssignBondOrderProcessor estimatePenalty() returned an error!" << __LINE__ << endl;
					evaluation_mode_ = false;
					ret = -1.;
				}
			}
		}
		else
		{
			Log.info() << "AssignBondOrderProcessor: error in evaluatePenalty(), valid = " <<  valid_ << ", readAtomPenalties = " << readAtomPenalties_() << endl;
		}

		evaluation_mode_ = false;

		add_missing_hydrogens_ = add_missing_hydrogens_backup;

		return ret;
	}


} // namespace BALL
