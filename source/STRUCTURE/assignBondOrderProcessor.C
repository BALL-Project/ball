#include <iostream>
#include <map>


// BALL includes
#include <BALL/kernel.h>
#include <BALL/SYSTEM/timer.h>

#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/COMMON/limits.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/FORMAT/resourceFile.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/QSAR/aromaticityProcessor.h>

// For lp_solve
#include <lpsolve/lp_lib.h>

#define MAX__SOLUTIONS 100 // TODO: should be a option

using namespace std;

namespace BALL 
{	
	//const char* AssignBondOrderProcessor::Option::OVERWRITE_UNKNOWN_BOND_ORDERS = "overwrite_unknown_bond_orders";
	//const bool  AssignBondOrderProcessor::Default::OVERWRITE_UNKNOWN_BOND_ORDERS = true;
	
	const String AssignBondOrderProcessor::ComputeAllSolutions::DISABLED = "disabled";
	const String AssignBondOrderProcessor::ComputeAllSolutions::ONE_BOND_HEURISTIC = "one_bond_heuristic";
	const String AssignBondOrderProcessor::ComputeAllSolutions::ENUMERATION_TREE = "enumeration_tree";

	const char* AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS = "overwrite_single_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_SINGLE_BOND_ORDERS = true;
	
	const char* AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS = "overwrite_double_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_DOUBLE_BOND_ORDERS = true;
	
	const char* AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS = "overwrite_triple_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_TRIPLE_BOND_ORDERS = true;

	const char* AssignBondOrderProcessor::Option::OVERWRITE_QUADRUPLE_BOND_ORDERS = "overwrite_quadruple_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_QUADRUPLE_BOND_ORDERS = true;

	const char* AssignBondOrderProcessor::Option::OVERWRITE_AROMATIC_BOND_ORDERS = "overwrite_aromatic_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_AROMATIC_BOND_ORDERS = true;

	const char* AssignBondOrderProcessor::Option::OVERWRITE_CHARGES = "overwrite_existing_charges";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_CHARGES = false;
	
	const char* AssignBondOrderProcessor::Option::ASSIGN_CHARGES = "assign_charges";
	const bool  AssignBondOrderProcessor::Default::ASSIGN_CHARGES = false;
	
	const char* AssignBondOrderProcessor::Option::KEKULIZE_RINGS = "kekulize_aromatic_rings";
	const bool  AssignBondOrderProcessor::Default::KEKULIZE_RINGS = true;

	const char* AssignBondOrderProcessor::Option::ENFORCE_OCTETT_RULE = "enforce_octett_rule";
	const bool  AssignBondOrderProcessor::Default::ENFORCE_OCTETT_RULE = true;

	const char* AssignBondOrderProcessor::Option::COMPUTE_ALL_SOLUTIONS = "compute_all_solutions";
	const String AssignBondOrderProcessor::Default::COMPUTE_ALL_SOLUTIONS = AssignBondOrderProcessor::ComputeAllSolutions::DISABLED;
		
	AssignBondOrderProcessor::AssignBondOrderProcessor()
		: UnaryProcessor<AtomContainer>(),
			options(),
			atomic_penalty_scores_(),
			bond_free_(),
			fixed_val_(),
			solutions_()
	{
		setDefaultOptions();
	}

	AssignBondOrderProcessor::AssignBondOrderProcessor(const AssignBondOrderProcessor& abop)
		:	UnaryProcessor<AtomContainer>(abop),
			options(abop.options),
			atomic_penalty_scores_(abop.atomic_penalty_scores_),
			bond_free_(abop.bond_free_),
			fixed_val_(abop.fixed_val_),
			solutions_(abop.solutions_)

	{
	}

	AssignBondOrderProcessor::~AssignBondOrderProcessor()
	{
		setDefaultOptions();
	}

	AssignBondOrderProcessor& AssignBondOrderProcessor::operator = (const AssignBondOrderProcessor& abop)
	{
		return *this;
	}

	bool AssignBondOrderProcessor::start()
	{
		solutions_.clear();
		fixed_val_.clear();
		bond_free_.clear();
		atomic_penalty_scores_.clear();
		return true;
	}

	Size AssignBondOrderProcessor::getNumberOfBondOrdersSet()
	{
		if (   (solutions_.size() >= (Size)last_applied_solution_) 
				&&  solutions_[last_applied_solution_].valid 
				&& (last_applied_solution_ > (Position) -1)
			 )
			return solutions_[last_applied_solution_].num_bonds;
		return 0;
	}

	Processor::Result AssignBondOrderProcessor::operator () (AtomContainer& ac)
	{
		// speed up the code by temporarily store the options locally 
		bool osbo = options.getBool(Option::OVERWRITE_SINGLE_BOND_ORDERS);
		bool odbo = options.getBool(Option::OVERWRITE_DOUBLE_BOND_ORDERS);
		bool otbo = options.getBool(Option::OVERWRITE_TRIPLE_BOND_ORDERS);
		bool oqbo = options.getBool(Option::OVERWRITE_QUADRUPLE_BOND_ORDERS);
		bool oabo = options.getBool(Option::OVERWRITE_AROMATIC_BOND_ORDERS);
		bool enforce_octett_rule = options.getBool(Option::ENFORCE_OCTETT_RULE);

		// what kind of composite do we have?
		if (RTTI::isKindOf<Molecule>(ac))
		{	
			// Check number of variables with prefix 'x'
			Position total_no_bonds = ac.countBonds();

			// Number of fixed bonds
			Size num_fixed_bonds = 0; 

			// Number of atoms in the system
			Position no_atoms = ac.countAtoms();

			// Mapping from (free) bonds onto bond variable indices
			//map<Bond*, Position> bond_map;

			// Vector for mapping from variable indices onto bonds
			//std::vector<Bond*> ind_bond(total_no_bonds, (Bond*)0);

			// Vector for storing fixed atom valences
			fixed_val_ = std::vector<Position>(no_atoms, 0);
			
			// Count the choice constraints
			//Position no_y = 0;

			// Count the number of bonds whose orders should be calculated
			//Position no_x = 0;

			// Maximum number of row entries (in penalty table)
			//Position max_no_ent = 0;

			// Maximum number of bonds of an atom
			//Position max_no_atom_bonds = 0;

			Index bond_index_counter = 0;
		
			// Find out, which bonds should be considered in general
			for (Position i = 0; i < no_atoms; ++i)
			{
				Atom* at1 = ac.getAtom(i);
				Position fixed = 0;
				//Position consider_bonds = 0;
				
				for (Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
				{
					Bond* bnd = &(*bit);

					// for the recursive function
					if (bond_to_index_.find(bnd) == bond_to_index_.end())
					{
						bond_to_index_[bnd] = bond_index_counter;
						bond_index_counter++;
					}
					// according to the options and the given bond order 
					// the acutal bond is a free variable of the ILP or not
					// YES: add a variable in the bond side constraint + ???? 
					// NO: equality in the bond side constraint +  ?????
					switch (bnd->getOrder())
					{
						case Bond::ORDER__SINGLE:
						{
								if (!osbo)
								{
									fixed += 1;
									bond_free_[bnd] = false;
								}
								else
								{	
								//	++consider_bonds;	
									bond_free_[bnd] = true;
								}
								break;
						}
						case Bond::ORDER__DOUBLE:
						{
								if (!odbo)
								{
									fixed += 2;
									bond_free_[bnd] = false;
								}
								else
								{	
								//	++consider_bonds;
									bond_free_[bnd] = true;
								}

								break;
						}
						case Bond::ORDER__TRIPLE:
						{
								if (!otbo)
								{
									fixed += 3;
									bond_free_[bnd] = false;
								}
								else
								{	
								//	++consider_bonds;	
									bond_free_[bnd] = true;
								}

								break;
						}
						case Bond::ORDER__QUADRUPLE:
						{
								if (!oqbo)
								{
									fixed += 4;
									bond_free_[bnd] = false;

								}
								else
								{	
								//	++consider_bonds;
									bond_free_[bnd] = true;
								}

								break;
						}
						case Bond::ORDER__AROMATIC:
						{
								if (!osbo)
								{
									fixed += 1; ///????????????	
									bond_free_[bnd] = false;
								}
								else
								{	
							//		++consider_bonds;
									bond_free_[bnd] = true;
								}
								break;
						}	
						default: //Bond::ORDER__UNKNOWN:
						{
						//		++consider_bonds;	
								bond_free_[bnd] = true;
						}
					}
				}
				fixed_val_[i] = fixed;
				num_fixed_bonds += fixed;
			}

			// Generate penalty values for all atoms in the AtomContainer ac
			calculateAtomPenalties_(ac);
	
			// get a first solution
			solutions_.push_back(ILPSolution_(this, ac));
			if (solutions_[0].valid)
			{
				last_applied_solution_ = 0;
				apply(ac, 0);	
			}
			else
			{
				last_applied_solution_=-1;
			}

			if ( (solutions_.size() > 0) 
					&&  (total_no_bonds - num_fixed_bonds < MAX__SOLUTIONS) 
					&&  (solutions_[0].valid)
				 )
			{
				if (options.get(Option::COMPUTE_ALL_SOLUTIONS) == ComputeAllSolutions::ONE_BOND_HEURISTIC)
				{
					// now iterate over all free bonds and inhibit the order we already found
					map<Bond*, bool>::iterator it = bond_free_.begin();
					for (; it!= bond_free_.end(); it++)
					{
						if (it->second)
						{	
							// We want to inhibit the current bond to have the order, we already found
							// Since lpsolve does not offer constraints of type unequal, we have to check
							// if there is a solution with constraint x_bond LE order-1  AND x_bond GE order+1 
							// the type of the extra constraint is controlled by the last parameter value.

							ILPSolution_ solutionLE; 
							ILPSolution_ solutionGE; 

							// case LE order-1
							if (solutions_[0].bond_orders[it->first]-1 > 0)
							{	
								solutionLE = ILPSolution_(this, ac, it->first, solutions_[0].bond_orders[it->first], true);
							}
							// case GE order+1
							if (solutions_[0].bond_orders[it->first]+1 < 5)
							{	
								solutionGE = ILPSolution_(this, ac, it->first, solutions_[0].bond_orders[it->first], false);
							}

							if (solutionLE.valid)
							{
								if (solutionGE.valid)
								{
									if (solutionLE.penalty <= solutionGE.penalty)
									{
										solutions_.push_back(solutionLE);  
									}
									if (solutionLE.penalty >= solutionGE.penalty)
									{
										solutions_.push_back(solutionGE);  
									}
								}
								else
								{
									solutions_.push_back(solutionLE);  
								}
							} // solution_LE not valid
							else
							{
								if (solutionGE.valid)
								{			
									solutions_.push_back(solutionGE);  
								}
							}
						}
						else
						{
							Log.error() << "There is a unknown bond" << endl;
						}
					} // end of all bonds
					
					if (options.getBool(AssignBondOrderProcessor::Option::KEKULIZE_RINGS))
					{
					// find all rings
					vector<vector<Atom*> > rings;
					RingPerceptionProcessor rpp;
					rpp.calculateSSSR(rings, ac);

					// set the aromatic rings	
					AromaticityProcessor ap;
					ap.aromatize(rings, ac);
					}

					// set the last solution // TODO: Do we want to apply the LAST solution?
					last_applied_solution_ = solutions_.size()-1;
					apply(ac, solutions_.size()-1);
				}
				else if (options.get(Option::COMPUTE_ALL_SOLUTIONS) == ComputeAllSolutions::ENUMERATION_TREE)
				{
/*// print the optimal sol
HashMap<Bond*, int>::Iterator mit = solutions_[0].bond_orders.begin();
for (; mit != solutions_[0].bond_orders.end(); ++mit)
{
	cout << bond_to_index_[mit->first] << "---"<< mit->second <<  " " << 
		mit->first->getFirstAtom()->getName() << ".."<<	mit->first->getSecondAtom()->getName()   << std::endl;
}
cout << std::endl;*/
					// get the current optimum
					optimal_penalty_ = solutions_[0].penalty;
					
					// prepare the compare String
					map<Bond*, bool>::iterator it = bond_free_.begin();
					for (; it!= bond_free_.end(); it++)
					{
						current_orders_ +=" ";
					}
					it = bond_free_.begin();
					for (; it!= bond_free_.end(); it++)
					{
						if (it->second && !(  (it->first->getFirstAtom()->getElement() ==PTE[Element::H])
								 					      ||( it->first->getSecondAtom()->getElement()==PTE[Element::H]) 
												       )
						   )
						{	
							// denote the bond as fixed
							bond_free_[it->first]= false;
							for (int order = 1; order <= 4; order++)
							{	
//	//cout << "vorher: " << current_orders_ << "-----" << bond_to_index_[it->first] << std::endl;//"--" << order << "--" << char(order) << std::endl;
								// denote the choosen bond order in the order string
								current_orders_[bond_to_index_[it->first]] = '0'+order;
	//	//cout << "nachher: " << current_orders_ << "-----" << bond_to_index_[it->first] << "--" << (current_orders_[bond_to_index_[it->first]]) << "--" << std::endl;

								// prohibit the bonds order to get the order found on the last (optimal) solution
								if (   (solutions_[0].bond_orders[it->first] != order)
										&& (checked_.find(current_orders_) == checked_.end()))
								{

	//cout << "Bond " << bond_to_index_[it->first] << "--Order " << order << " situation: " << current_orders_ << std::endl;
																	
									// denote the choosen bond order in the atoms fixed valences
									fixed_val_[it->first->getFirstAtom()->getIndex()] += order;
									fixed_val_[it->first->getSecondAtom()->getIndex()] += order;
			
									//current_orders_.set(String(order), bond_to_index_[it->first], bond_to_index_[it->first]);
									// try to solve this bond order
									recursive_solve(ac, 0);

									// remember, that we already tried this combination
									checked_.insert(current_orders_);

									// clean up the order string 
									current_orders_[bond_to_index_[it->first]] = ' ';

									//current_orders_.set(" ", bond_to_index_[it->first], bond_to_index_[it->first]);
									
									// reset the fixed valences
									fixed_val_[it->first->getFirstAtom()->getIndex()] -= order;
									fixed_val_[it->first->getSecondAtom()->getIndex()] -= order;
								}
							}
							// reset the bond status
							bond_free_[it->first] = true;
						}
					}	

					// set the last solution // TODO: Do we want to apply the LAST solution?
					last_applied_solution_ = solutions_.size()-1;
					apply(ac, solutions_.size()-1);
				}
			}
			else
			{	
				cout << "No solution compted! -solsize:" <<  solutions_.size() << " - no free bonds:" <<  total_no_bonds - num_fixed_bonds << " - valid " << (solutions_.size()>0 ? solutions_[0].valid : false) << std::endl;
				//cout << "Too many bonds " << total_no_bonds - num_fixed_bonds << std::endl;
			}
		}	
		return Processor::CONTINUE;
	}

	void AssignBondOrderProcessor::recursive_solve(AtomContainer& ac, int depth)
	{
////cout << depth+1 << std::endl;
		ILPSolution_ sol = ILPSolution_(this, ac);
//cout << "penalty: " <<  sol.penalty << " "<< optimal_penalty_ ;//<< std::endl;
		bool last_bond = true;
		if (sol.valid && (sol.penalty == optimal_penalty_))
		{
////cout << "   has a opti sol with ";// << std::endl;
			// store this solution
			solutions_.push_back(sol);	
			last_applied_solution_ = solutions_.size()-1;
			// look for further free bonds
			map<Bond*, bool>::iterator it = bond_free_.begin();
			for (; it!= bond_free_.end(); it++)
			{
				if (it->second && !(  (it->first->getFirstAtom()->getElement() ==PTE[Element::H])
								 					||  (it->first->getSecondAtom()->getElement()==PTE[Element::H]) 
												  )
				   )
				{	
//cout << " opti sol: bond " <<  bond_to_index_[it->first] << " order " << sol.bond_orders[it->first] << " situation " << current_orders_ <<  std::endl;

					last_bond = false;
					// test whether there exist another solution at all
					ILPSolution_ antisol_le = ILPSolution_(this, ac, &(*it->first), sol.bond_orders[it->first], true);
					ILPSolution_ antisol_ge = ILPSolution_(this, ac, &(*it->first), sol.bond_orders[it->first], false);
				
					bond_free_[it->first]= false;

					if (antisol_le.valid || antisol_ge.valid)			
					{
					for (int order = 1; order <= 4; order++)
					{	
////cout << "vorher: " << current_orders_ << "-----" << bond_to_index_[it->first] << std::endl;

						// denote the choosen bond order in the order string
						//current_orders_.set(String(order), bond_to_index_[it->first], bond_to_index_[it->first]);
						//current_orders_[bond_to_index_[it->first]] = char(order);
						current_orders_[bond_to_index_[it->first]] = '0'+order;

						// prohibit the bonds order to get the order found on the last (optimal) solution
						if (   (sol.bond_orders[it->first]!= order)
								&& (checked_.find(current_orders_) == checked_.end())
								)
						{
////cout << "nachher: " << current_orders_ << "-----" << bond_to_index_[it->first] << std::endl;
//cout << "Bond " << bond_to_index_[it->first] << "--Order " << order << " situation: " << current_orders_ << std::endl;

							// denote the set bond order in the atoms fixed valences
							fixed_val_[it->first->getFirstAtom()->getIndex()] += order;
							fixed_val_[it->first->getSecondAtom()->getIndex()] += order;

							// try to solve
							recursive_solve(ac, depth + 1);

							// remember, that we already tried this combination
							checked_.insert(current_orders_);

							// reset the fixed valences
							fixed_val_[it->first->getFirstAtom()->getIndex()] -= order;
							fixed_val_[it->first->getSecondAtom()->getIndex()] -= order;
						}
						// clean up the order string 
						current_orders_[bond_to_index_[it->first]] = ' ';
						//current_orders_.set(" ", bond_to_index_[it->first], bond_to_index_[it->first]);
					}
					}
					// reset the bond status
					bond_free_[it->first] = true;
				}
			}
			// this was the last free bond and we got a optimal solution
			if (last_bond)
			{
				// store :-)
				solutions_.push_back(sol);		
				last_applied_solution_ = solutions_.size()-1;
			}
		}
		else
		{
//cout << " no opti sol" << endl;
		}
		// if no valid solution was found
		// set all further bond combinations to checked
		//setChecked_(current_orders_);
	}

/*
	bool AssignBondOrderProcessor::isChecked_(String orders)
	{
		if (checked_.find(current_orders_) == checked_.end())
			return true;
		else
		{
			
		}
	}*/

	void AssignBondOrderProcessor::setChecked_(String orders)
	{
		String tmp_orders(orders);
		for (Size i = 0; i< current_orders_.size(); i++)
		{
			if (orders[i] == ' ')
			{
				for (int j=0; j < 5; j++)
				{
					tmp_orders[i] = '0'+j;
					if (j==0)
						tmp_orders[i] = ' ';
					tmp_orders[i] = '0'+j;
					checked_.insert(current_orders_);
					setChecked_(tmp_orders);
				}
			}
		}
	}

	void AssignBondOrderProcessor::calculateAtomPenalties_(AtomContainer& ac)
	{
		// This method computes for every atom its possible atomic valences 
		// and the corresponding possible atomic penalty scores and stores them 
		// per atom in the vector atomic_penalty_scores_
		
		const Atom* partner_atom;	
		const Atom* sec_partner_atom;	
		const Atom* other_partner_atom;	
		AtomIterator atom_it;	//iterates over all atoms in the observed molecule
		Bond* bond;	//bond of the observed atom
		Bond* next_bond;	//another bond of the observed atom
		Bond* partner_bond;	//bond of the partner_atom
		Bond* next_partner_bond;	//next bond of the partner_atom	
		Atom::BondIterator bond_it;	//iterate over all bonds of one atom
		Atom::BondIterator bond_it2;
		RingPerceptionProcessor(ringPP);	//marks all atoms in a ring structure with property "InRing"
		pair <int, int > atomic_score;	//pair of atomic valence and corresponding atomic penalty score
		vector <pair <int, int > > aps_vector;	//vector of atomic_scores

		//mark all atoms in a ring structure with property "InRing"
		ac.apply(ringPP);	
		atom_it = ac.beginAtom();	

		//if bonds exist
		if (ac.countBonds() != 0)
		{
			//assign to each atom in molecule a penalty score if necessary
			for (;+atom_it;++atom_it)
			{
				aps_vector.clear();
				//Oxygen
				if (atom_it->getElement().getName()=="Oxygen")
				{
					//O(x1)
					if (atom_it->countBonds() == 1)
					{
						bond_it = atom_it->beginBond();
						if (bond_it != atom_it->beginBond())
						{
							partner_atom = bond_it->getBoundAtom(*atom_it);
							//O(x1) in pyridine-1-oxide
							if (((partner_atom->getElement().getName()=="Nitrate") or (partner_atom->getElement().getName()=="Nitrogen")) and partner_atom->getProperty("InRing").getBool())
							{
								atomic_score.first = 1;
								atomic_score.second = 0;
								aps_vector.push_back(atomic_score);

								atomic_score.first = 2;
								atomic_score.second = 1;
								aps_vector.push_back(atomic_score);
							}
							else 
							{
								atomic_score.first = 1;
								atomic_score.second = 1;
								aps_vector.push_back(atomic_score);

								atomic_score.first = 2;
								atomic_score.second = 0;
								aps_vector.push_back(atomic_score);

								atomic_score.first = 3;
								atomic_score.second = 64;
								aps_vector.push_back(atomic_score);
							}
						}
					}
					//O(x2)
					else if (atom_it->countBonds() == 2)
					{
						atomic_score.first = 1;
						atomic_score.second = 32;
						aps_vector.push_back(atomic_score);

						atomic_score.first = 2;
						atomic_score.second = 0;
						aps_vector.push_back(atomic_score);

						atomic_score.first = 3;
						atomic_score.second = 64;
						aps_vector.push_back(atomic_score);
					}
				}

				//H, F, CL, Br, I
				if ((atom_it->getElement().getName()=="Hydrogen") or (atom_it->getElement().getName()=="Fluorine") or 		(atom_it->getElement().getName()=="Iodine ") or (atom_it->getElement().getName()=="Chlorine") or 	(atom_it->getElement().getName()=="Bromine") )
				{
					atomic_score.first = 1;
					atomic_score.second = 0;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 0;
					atomic_score.second = 64;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 2;
					atomic_score.second = 64;
					aps_vector.push_back(atomic_score);
				}


				//Carbon
				if (atom_it->getElement().getName()=="Carbon")
				{
					if (atom_it->countBonds() ==1)
					{
						//C in CN-R
						bond_it = atom_it->beginBond();
						if (bond_it != atom_it->endBond())
						{
							partner_atom = bond_it->getBoundAtom(*atom_it);
							if ((partner_atom->getElement().getName()=="Nitrate") or (partner_atom->getElement().getName()=="Nitrogen"))
							{
								if (partner_atom->countBonds() == 2)
								{
									atomic_score.first = 3;
									atomic_score.second = 0;
									aps_vector.push_back(atomic_score);

									atomic_score.first = 4;
									atomic_score.second = 1;
									aps_vector.push_back(atomic_score);

									atomic_score.first = 5;
									atomic_score.second = 32;
									aps_vector.push_back(atomic_score);				
								}
							}
							//C(x1)		
							else 
							{
								atomic_score.first = 3;
								atomic_score.second = 1;
								aps_vector.push_back(atomic_score);

								atomic_score.first = 4;
								atomic_score.second = 0;
								aps_vector.push_back(atomic_score);

								atomic_score.first = 5;
								atomic_score.second = 32;
								aps_vector.push_back(atomic_score);
							}
						}
					}
					//C in COO-
					bond_it = atom_it->beginBond();
					if (bond_it != atom_it->endBond())
					{
						partner_atom = bond_it->getBoundAtom(*atom_it);
						if (partner_atom->getElement().getName() =="Oxygen")
						{
							sec_partner_atom = (++bond_it)->getBoundAtom(*atom_it);
							if (sec_partner_atom->getElement().getName() =="Oxygen")
							{
								if ((partner_atom->countBonds() == 1) || (sec_partner_atom->countBonds() ==1))
								{
									atomic_score.first = 5;
									atomic_score.second = 0;
									aps_vector.push_back(atomic_score);

									atomic_score.first = 4;
									atomic_score.second = 32;
									aps_vector.push_back(atomic_score);

									atomic_score.first = 6;
									atomic_score.second = 32;
									aps_vector.push_back(atomic_score);
								}		
							}
						}
						//if atom type still not found
						if (aps_vector.empty())
						{
							//C
							atomic_score.first = 2;
							atomic_score.second = 64;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 3;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 4;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 5;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 6;
							atomic_score.second = 64;
							aps_vector.push_back(atomic_score);
						}		
					}
				}

				// Si
				if ((atom_it->getElement().getName()== "Silicium") || (atom_it->getElement().getName()== "Silicon"))
				{
					atomic_score.first = 4;
					atomic_score.second = 0;
					aps_vector.push_back(atomic_score);
				}



				//N(x1) in N=N=R
				if (((atom_it->getElement().getName()=="Nitrate") || (atom_it->getElement().getName()=="Nitrogen")) && (atom_it->countBonds() ==1))
				{
					bond = &(*(atom_it->beginBond()));
					if (atom_it->beginBond() != atom_it->endBond())
					{
						partner_atom = bond->getBoundAtom(*atom_it);
						if ((partner_atom->getElement().getName()=="Nitrate") || (partner_atom->getElement().getName()=="Nitrogen"))
						{
							if (partner_atom->countBonds()==2)
							{  
								partner_bond = const_cast <Bond*> (&(*(partner_atom->beginBond())));
								next_partner_bond = const_cast <Bond*> (&(*(++(partner_atom->beginBond()))));
								const Atom* next_partner_atom = next_partner_bond->getBoundAtom(*partner_atom);
								if (next_partner_atom->countBonds() == 1)
								{
									atomic_score.first = 2;
									atomic_score.second = 0;
									aps_vector.push_back(atomic_score);

									atomic_score.first = 3;
									atomic_score.second = 0;
									aps_vector.push_back(atomic_score);
								}
							}
						}
						//N(x1)
						if (aps_vector.empty())
						{
							atomic_score.first = 2;
							atomic_score.second = 3;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 3;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 4;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);
						}	
					}		
				}
				//N(x2) in N=N=R
				if (((atom_it->getElement().getName()=="Nitrate") || (atom_it->getElement().getName()=="Nitrogen")) && (atom_it->countBonds()== 2))
				{
					bond = &(*(atom_it->beginBond()));
					if (atom_it->beginBond() != atom_it->endBond())
					{
						next_bond = const_cast<Bond*>(&(*(++(atom_it->beginBond()))));
						partner_atom = bond->getBoundAtom(*atom_it);
						other_partner_atom = next_bond->getBoundAtom(*atom_it);
						//if one of the neighbours is a Nitrate
						if ((partner_atom->getElement().getName()=="Nitrate") || (partner_atom->getElement().getName()=="Nitrogen"))
						{
							//check bonds (x1 of N)
							if (partner_atom->countBonds()==1)	
							{ 
								atomic_score.first = 3;
								atomic_score.second = 1;
								aps_vector.push_back(atomic_score);

								atomic_score.first = 4;
								atomic_score.second = 0;
								aps_vector.push_back(atomic_score);							
							}
						}
						if ((other_partner_atom->getElement().getName()=="Nitrate") || (other_partner_atom->getElement().getName()=="Nitrogen"))
						{
							//check bonds (x2 of N)
							if (other_partner_atom->countBonds()==1)
							{
								atomic_score.first = 3;
								atomic_score.second = 1;
								aps_vector.push_back(atomic_score);

								atomic_score.first = 4;
								atomic_score.second = 0;
								aps_vector.push_back(atomic_score);
							} 
						}
					}
					//N(x2)
					if (aps_vector.empty())
					{
						atomic_score.first = 2;
						atomic_score.second = 4;
						aps_vector.push_back(atomic_score);

						atomic_score.first = 3;
						atomic_score.second = 0;
						aps_vector.push_back(atomic_score);

						atomic_score.first = 4;
						atomic_score.second = 2;
						aps_vector.push_back(atomic_score);
					}
				}
				//N(x3) in nitro
				if (((atom_it->getElement().getName()=="Nitrate") || (atom_it->getElement().getName()=="Nitrogen")) && (atom_it->countBonds()==3))
				{
					bond_it = atom_it->beginBond();
					if (atom_it->countBonds() > 0)
					{
						int count_oxygen_atoms= 0;
						for (;bond_it != atom_it->endBond();++bond_it)
						{
							partner_atom = bond_it->getBoundAtom(*atom_it);
							if ((partner_atom->getElement().getName()=="Oxygen") && (partner_atom->countBonds()==1 ))
							{
								++count_oxygen_atoms; 
							}
						}
						if (count_oxygen_atoms==2)
						{
							atomic_score.first = 3;
							atomic_score.second = 64;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 4;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 5;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 6;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);
						}
						//N(x3) in pyridine-1-oxide
						else 
						{
							bond_it2 = atom_it->beginBond();
							if (atom_it->countBonds() > 0)
							{
								Size count_oxygen_atoms=0;
								Size countCarb=0;
								for (;bond_it2 != atom_it->endBond();++bond_it2)
								{
									partner_atom = bond_it2->getBoundAtom(*atom_it);
									if (partner_atom->getElement().getName()=="Carbon" and partner_atom->getProperty("InRing").getName()=="InRing")
									{
										++countCarb;
									}
									if (partner_atom->getElement().getName()=="Oxygen")
									{
										++count_oxygen_atoms;
									}
								}
								if (count_oxygen_atoms==1 and countCarb==2)	
								{
									atomic_score.first = 3;
									atomic_score.second = 1;
									aps_vector.push_back(atomic_score);

									atomic_score.first = 4;
									atomic_score.second = 0;
									aps_vector.push_back(atomic_score);
								}
							}
						}
						//N(x3)
						if (aps_vector.empty())
						{
							atomic_score.first = 2;
							atomic_score.second =32;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 3;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 4;
							atomic_score.second = 1;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 5;
							atomic_score.second = 2;
							aps_vector.push_back(atomic_score);
						}
					}	
				}
				//N(x4)
				if (((atom_it->getElement().getName()=="Nitrate") || (atom_it->getElement().getName()=="Nitrogen")) && (atom_it->countBonds()==4))
				{
					atomic_score.first = 2;
					atomic_score.second = 64;
					aps_vector.push_back(atomic_score);		

					atomic_score.first = 3;
					atomic_score.second = 0;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 4;
					atomic_score.second = 64;
					aps_vector.push_back(atomic_score);
				}


				//P(x1)
				if ((atom_it->getElement().getName()=="Phosphorus") && (atom_it->countBonds() ==1))
				{
					atomic_score.first = 2;
					atomic_score.second = 2;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 3;
					atomic_score.second = 0;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 4;
					atomic_score.second = 32;
					aps_vector.push_back(atomic_score);
				}
				//P(x2)
				if ((atom_it->getElement().getName()=="Phosphorus") && (atom_it->countBonds()==2))
				{
					atomic_score.first = 2;
					atomic_score.second = 4;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 3;
					atomic_score.second = 0;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 4;
					atomic_score.second = 2;
					aps_vector.push_back(atomic_score);
				}
				//P(x3)
				if ((atom_it->getElement().getName()=="Phosphorus") && (atom_it->countBonds()==3))
				{
					atomic_score.first = 2;
					atomic_score.second = 32;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 3;
					atomic_score.second = 0;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 4;
					atomic_score.second = 1;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 5;
					atomic_score.second = 2;
					aps_vector.push_back(atomic_score);
				}
				//P(x4) 
				if ((atom_it->getElement().getName()=="Phosphorus") && (atom_it->countBonds()==4))
				{
					bond_it = atom_it->beginBond();
					if (atom_it->countBonds() > 0)
					{
						Size count_oxygen_atoms = 0;
						Size count_sulfur_atoms = 0;
						for (;bond_it != atom_it->endBond(); ++bond_it)
						{
							partner_atom = bond_it->getBoundAtom(*atom_it);
							if ((partner_atom->getElement().getName()=="Oxygen") && (partner_atom->countBonds()== 1))
							{
								++count_oxygen_atoms;
							}
							else if (((partner_atom->getElement().getName()=="Sulphur") ||  (partner_atom->getElement().getName()=="Sulfur")) && (partner_atom->countBonds()== 1))
							{
								++count_sulfur_atoms;
							}					
						}
						//bonded to 2 O(x1) or 2 S(x1)
						if (count_oxygen_atoms==2 or count_sulfur_atoms==2)
						{
							atomic_score.first = 5;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 6;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 7;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);
						}	
						//bonded to 3 O(x1) or 3 S(x1)
						if (count_oxygen_atoms==3 or count_sulfur_atoms==3)
						{
							atomic_score.first = 6;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 7;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);
						}
						//P(x4)
						if (aps_vector.empty())
						{
							atomic_score.first = 3;
							atomic_score.second = 64;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 4;
							atomic_score.second = 1;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 5;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 6;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);
						}
					}
				}


				//S(x1)
				if (((atom_it->getElement().getName()=="Sulphur") ||  (atom_it->getElement().getName()=="Sulfur")) && (atom_it->countBonds()==1))
				{
					bond_it = atom_it->beginBond();
					if (bond_it != atom_it->endBond())
					{
						partner_atom = bond_it->getBoundAtom(*atom_it);
						//S(x1) in pyridine-1-thiol anion
						if (((partner_atom->getElement().getName()=="Nitrate") || (partner_atom->getElement().getName()=="Nitrogen")) && (partner_atom->getProperty("InRing").getName() =="InRing"))
						{
							//check if Nitrate is inRing of Carbons
							const Atom* partner_of_partner_atom;
							Size count_carbon_atoms = 0;

							for (;+bond_it2;++bond_it2)
							{
								partner_of_partner_atom = bond_it2->getBoundAtom(*partner_atom);
								if (partner_of_partner_atom->getElement().getName() == "Carbon")
								{
									++count_carbon_atoms;
								}
							}
							if ((partner_atom->countBonds()== 2) && (count_carbon_atoms >= 1))
							{
								atomic_score.first = 1;
								atomic_score.second = 0;
								aps_vector.push_back(atomic_score);

								atomic_score.first = 2;
								atomic_score.second = 1;
								aps_vector.push_back(atomic_score);
							}
						}
						//S(x1)
						if (aps_vector.empty())
						{
							atomic_score.first = 1;
							atomic_score.second = 2;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 2;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 3;
							atomic_score.second = 64;
							aps_vector.push_back(atomic_score);
						}
					}
				}

				//S(x2)
				if (((atom_it->getElement().getName()=="Sulphur") ||  (atom_it->getElement().getName()=="Sulfur")) && (atom_it->countBonds()==2))
				{
					atomic_score.first = 1;
					atomic_score.second = 32;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 2;
					atomic_score.second = 0;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 3;
					atomic_score.second = 32;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 4;
					atomic_score.second = 1;
					aps_vector.push_back(atomic_score);
				}

				//S(x3)
				if (((atom_it->getElement().getName()=="Sulphur") ||  (atom_it->getElement().getName()=="Sulfur")) && (atom_it->countBonds()==3))
				{
					atomic_score.first = 3;
					atomic_score.second = 1;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 4;
					atomic_score.second = 0;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 5;
					atomic_score.second = 2;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 6;
					atomic_score.second = 2;
					aps_vector.push_back(atomic_score);
				}



				//S(x4)
				if (((atom_it->getElement().getName()=="Sulphur") ||  (atom_it->getElement().getName()=="Sulfur")) && (atom_it->countBonds()==4))
				{
					bond_it = atom_it->beginBond();
					if (atom_it->countBonds() > 0)
					{
						Size count_oxygen_atoms = 0;
						Size count_sulfur_atoms = 0;
						for (;bond_it != atom_it->endBond(); ++bond_it){
							partner_atom = bond_it->getBoundAtom(*atom_it);
							if ((partner_atom->getElement().getName()=="Oxygen") && (partner_atom->countBonds()== 1))
							{
								++count_oxygen_atoms;
							}
							else if (  (  (partner_atom->getElement().getName()=="Sulphur") 
											    ||(partner_atom->getElement().getName()=="Sulfur") ) 
									    && (partner_atom->countBonds()== 1))
							{
								++count_sulfur_atoms;
							}					
						}
						//bonded to 2 O(x1) or S(x1)
						if (count_oxygen_atoms ==2 or count_sulfur_atoms==2)
						{
							atomic_score.first = 6;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 7;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);
						}	
						//bonded to 3 O(x1) or S(x1)
						if (count_oxygen_atoms== 3 or count_sulfur_atoms==3)
						{
							atomic_score.first = 6;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 7;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);
						}
						//bonded to 4 O(x1) or S(x1)
						if (count_oxygen_atoms==4 or count_sulfur_atoms==4)
						{
							atomic_score.first = 6;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 7;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);
						}		
						//S(x4)
						if (aps_vector.empty())
						{
							atomic_score.first = 4;
							atomic_score.second = 4;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 5;
							atomic_score.second = 2;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 6;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);
						}	
					}
				}
				atomic_penalty_scores_.push_back(aps_vector);
			}
		}
	}

	bool AssignBondOrderProcessor::finish()
	{		
		return true;
	}


	void AssignBondOrderProcessor::setDefaultOptions()
	{		
	
	 	options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, 
	 												 AssignBondOrderProcessor::Default::OVERWRITE_SINGLE_BOND_ORDERS); 
	
	 	options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, 
	 												 AssignBondOrderProcessor::Default::OVERWRITE_DOUBLE_BOND_ORDERS); 
	
	 	options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, 
	 												 AssignBondOrderProcessor::Default::OVERWRITE_TRIPLE_BOND_ORDERS); 

	 	options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_QUADRUPLE_BOND_ORDERS, 
	 												 AssignBondOrderProcessor::Default::OVERWRITE_QUADRUPLE_BOND_ORDERS); 

	 	options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_AROMATIC_BOND_ORDERS, 
	 												 AssignBondOrderProcessor::Default::OVERWRITE_AROMATIC_BOND_ORDERS); 

		options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_CHARGES,
												   AssignBondOrderProcessor::Default::OVERWRITE_CHARGES);	
		
		options.setDefaultBool(AssignBondOrderProcessor::Option::ASSIGN_CHARGES,
													 AssignBondOrderProcessor::Default::ASSIGN_CHARGES);	
	
		options.setDefaultBool(AssignBondOrderProcessor::Option::KEKULIZE_RINGS,
													 AssignBondOrderProcessor::Default::KEKULIZE_RINGS);	
		
		options.setDefaultBool(AssignBondOrderProcessor::Option::ENFORCE_OCTETT_RULE,
													 AssignBondOrderProcessor::Default::ENFORCE_OCTETT_RULE);	
	}

	bool AssignBondOrderProcessor::apply(AtomContainer& ac, Position i)
	{
		if (i < solutions_.size())
		{
			if (solutions_[i].valid)
			{
				// we assume, that the AtomContainer is valid and the correct one! //TODO: is the ok?
				AtomIterator a_it = ac.beginAtom();
				Atom::BondIterator b_it = a_it->beginBond();
				BALL_FOREACH_BOND(ac, a_it, b_it)
				{
					HashMap<Bond*, int>::Iterator it = solutions_[i].bond_orders.find(&*b_it);
					if (it != solutions_[i].bond_orders.end())
					{
						b_it->setOrder(it->second);
					}
				}
				last_applied_solution_ = i;

				if (options.getBool(AssignBondOrderProcessor::Option::KEKULIZE_RINGS))
				{
					// find all rings
					vector<vector<Atom*> > rings;
					RingPerceptionProcessor rpp;
					rpp.calculateSSSR(rings, ac);

					// set the aromatic rings	
					AromaticityProcessor ap;
					ap.aromatize(rings, ac);
				}
			}
			return solutions_[i].valid;
		}
		else return false;
	}

	//-----------------------------------------------------------------------------------
	
	AssignBondOrderProcessor::ILPSolution_::ILPSolution_()
		: valid(false),
			bond_orders(),
			penalty(0),
			num_bonds(0)
	{
	}

	AssignBondOrderProcessor::ILPSolution_::ILPSolution_(AssignBondOrderProcessor* ap, AtomContainer& ac, Bond* bond, int order, bool lessEqualConstraint)
	{		
			// Check number of variables with prefix 'x'
		Position total_no_bonds = ac.countBonds();

		// Number of atoms in the system
		Position no_atoms = ac.countAtoms();

		// Mapping from (free) bonds onto bond variable indices
		map<Bond*, Position> bond_map;

		// Vector for mapping from variable indices onto bonds
		std::vector<Bond*> ind_bond(total_no_bonds, (Bond*)0);

		// Vector for storing fixed atom valences
		//std::vector<Position> fixed_val_(no_atoms, 0);

		// Count the choice constraints
		Position no_y = 0;

		// Count the number of bonds whose orders should be calculated
		Position no_x = 0;

		// Maximum number of row entries (in penalty table)
		Position max_no_ent = 0;

		// Maximum number of bonds of an atom
		Position max_no_atom_bonds = 0;
	
		// compute the maps, variables ...
		for (Position i = 0; i < no_atoms; ++i)
		{
			Atom* at1 = ac.getAtom(i);
			//Position fixed = 0;
			Position consider_bonds = 0;

			bool consider_atom = false;
			for (Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
			{		
				Bond* bnd = &(*bit);
				map<Bond*, bool>::iterator it = ap->bond_free_.find(bnd);
				if (it != ap->bond_free_.end())
				{
					// If this atom has a free bond we should calculate the bond order
					if (it->second && (bond!=bnd))
					{
						++consider_bonds;	
						consider_atom = true;
						pair<map<Bond*, unsigned int>::iterator, bool> p 
							= bond_map.insert(make_pair(bnd, no_x+1));
						if (p.second)
						{
							ind_bond[no_x] = bnd;
							++no_x;
						}
					}
				}
			} // end of for all bonds of this atom

			if (consider_atom)
			{
				no_y += ap->atomic_penalty_scores_[i].size();
				if (ap->atomic_penalty_scores_[i].size() > max_no_ent)
				{
					max_no_ent = ap->atomic_penalty_scores_[i].size();
				}
			}

			if (consider_bonds > max_no_atom_bonds)
			{
				max_no_atom_bonds = consider_bonds;
			}

		} // end of for all atoms

		Position no_vars = no_x + no_y;
		//cout << "no vars " << no_vars << endl;

		// Create a new model with 'no_vars' variables 
		// (columns) and 0 rows
		lprec *lp = make_lp(0, no_vars);
		if (!lp)
		{
			Log.error() << "Creation of ILP failed" << endl;
		}

		// Set the binary and integer constraints
		for(Position i = 1; i <= no_x; ++i)
		{
			set_int(lp, i, TRUE);
		}

		for(Position i = no_x+1; i <= no_vars; ++i)
		{
			set_binary(lp, i, TRUE);
		}

		// Create space large enough for one row and the objective function,
		// use lp_solves internal types.
		std::vector<int> colno(max_no_atom_bonds + 1 + max_no_ent,0);
		std::vector<REAL> row(max_no_atom_bonds + 1 + max_no_ent,0);
		std::vector<int> obj_colno(no_y+1,0);
		std::vector<REAL> obj_row(no_y+1,0);

		// Factors for choice constraints
		std::vector<REAL> choices(max_no_ent, 1);

		// Makes building the model faster if it's done
		// row by row
		set_add_rowmode(lp, TRUE);

		// Set the lower and upper constraints for bonds
		for(Position i = 1; i <= no_x; ++i)
		{
			colno[0] = i;
			if (!add_constraintex(lp, 1, &choices[0], &colno[0], LE, 4))
			{
				Log.error() << "Setting upper bound for bond constraint failed" << endl;
			}
			if (!add_constraintex(lp, 1, &choices[0], &colno[0], GE, 1))
			{
				Log.error() << "Setting lower bound for bond constraint failed" << endl;
			}
		}

		// Set the extra bond constraint
		if (bond && (order-1 > -1) && (order+1 < 5))
		{	
			// get the bonds index 
			map<Bond*, unsigned int>::iterator it = bond_map.find(bond);
			if (it != bond_map.end())
			{
				Position i = it->second;
				colno[0] = i;
				if (lessEqualConstraint)
				{
					if (!add_constraintex(lp, 1, &choices[0], &colno[0], LE, order-1))
					{
						Log.error() << "Setting extra upper bound for bond constraint failed" << endl;
					}
				}
				else
				{
					if (!add_constraintex(lp, 1, &choices[0], &colno[0], GE, order+1))
					{
						Log.error() << "Setting extra lower bound for bond constraint failed" << endl;
					}
				}
			}
	 	}

		// Create all remaining constraints and the objective function

		// Atom indices
		Position i = 0;

		// Indices of the choice variables (y)
		Position ind_y = 0;

		for(; i < no_atoms; ++i)
		{
			// Find corresponding variables
			Atom* at1 = ac.getAtom(i);

			// Count the bonds of this atom whose
			// orders should be calculated
			Position count_b = 0;
			for(Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
			{
				Bond* bnd = &(*bit);
				map<Bond*, unsigned int>::iterator it = bond_map.find(bnd);

				if (it != bond_map.end())
				{
					colno[count_b] = it->second;
					row[count_b]   = -1;
					++count_b;
				}
			}

			if (count_b)
			{
				// This atom has bonds whose orders should be calculated

				// Create indices for the choice variables and set
				// the entries for the valence constraint and the 
				// objective function

				// Count the number of variables participating in the current row
				Position count_vars = count_b;
				for(Position j = 0; j < ap->atomic_penalty_scores_[i].size(); ++j, ++ind_y, ++count_vars)
				{
					// Choice variables have indices > no_x in variable vector for ILP
					obj_colno[ind_y]   = ind_y + no_x + 1;
					colno[count_vars] = obj_colno[ind_y];

					// Set valences
					row[count_vars]   = ap->atomic_penalty_scores_[i][j].first;

					// Set penalties
					obj_row[ind_y]     = ap->atomic_penalty_scores_[i][j].second;
				}

				// in case we got an empty penalty row
				if (ap->atomic_penalty_scores_[i].size() != 0)
				{
					// Add valence constraint
					if (!add_constraintex(lp, count_vars, &row[0], &colno[0], EQ, 
								ap->fixed_val_[i]))
					{
						Log.error() << "Setting valence constraint for ILP failed" << endl;
					}

					// Add choice constraint
					if (!add_constraintex(lp, ap->atomic_penalty_scores_[i].size(), &choices[0], &colno[count_b], 
								EQ, 1))
					{
						Log.error() << "Setting choice constraint for ILP failed" << endl;
					}

					if (ap->options.getBool(Option::ENFORCE_OCTETT_RULE))
					{
						// Add constraint for the octett-rule
						if ((at1->getElement()!= PTE[Element::HELIUM]) && at1->getElement().getGroup() > (short)3)
						{
							if (!add_constraintex(lp, count_b,  &row[0], &colno[0], 
										GE, -4))
							{
								Log.error() << "Setting octett constraint for ILP failed" << endl;
							} 
						}
					}
				}
			}
		}

		// Rowmode should be turned off again after building the model
		set_add_rowmode(lp, FALSE);

		// Set objective function
		if (!set_obj_fnex(lp, no_y, &obj_row[0], &obj_colno[0]))
		{
			Log.error() << "Setting objective function for ILP failed" << endl;
		}

		// Tell lp_solve that this problem is a minimization problem
		set_minim(lp);

		// We only want to see important messages on screen while solving
		set_verbose(lp, IMPORTANT);

		// Show the generated MIP
		//write_LP(lp, stdout);

		// Let lp_solve solve our problem
		int ret = solve(lp);

		// Check whether lp_solve could do its job successfully
		if (ret == OPTIMAL)
		{
			// Get the value of the objective function
			penalty = (int)get_objective(lp);

			// Get variables
			REAL *vars;
			get_ptr_variables(lp, &vars);

			// count the assigned bond orders
			num_bonds = 0;
			// Do the assignment of the bond orders
			for(Position i = 0; i < no_x; ++i)
			{
				if (fabs(vars[i] - 1.) < 1.e-10)
				{
					// TODO: should we really set the orders here?
					ind_bond[i]->setOrder(Bond::ORDER__SINGLE);
					num_bonds++;
					//orders_[ind_bond[i]] = Bond::ORDER__SINGLE;
					continue;
				}
				if (fabs(vars[i] - 2.) < 1.e-10)
				{
					ind_bond[i]->setOrder(Bond::ORDER__DOUBLE);	
					num_bonds++;
					//orders_[ind_bond[i]] = Bond::ORDER__DOUBLE;
					continue;
				}
				if (fabs(vars[i] - 3.) < 1.e-10)
				{
					ind_bond[i]->setOrder(Bond::ORDER__TRIPLE);	
					num_bonds++;	
					//orders_[ind_bond[i]] = Bond::ORDER__TRIPLE;
					continue;
				}
				if (fabs(vars[i] - 4.) < 1.e-10)
				{
					ind_bond[i]->setOrder(Bond::ORDER__QUADRUPLE);
					num_bonds++;	
					//orders_[ind_bond[i]] = Bond::ORDER__QUADRUPLE;
					continue;
				}
			}

			
			// we got a valid solution
			valid = true;

			// store the bond orders of _ALL_ bonds to offer a complete solution 
			AtomIterator a_it = ac.beginAtom();
			Atom::BondIterator b_it = a_it->beginBond();
			BALL_FOREACH_BOND(ac, a_it, b_it)
			{
				bond_orders[&(*b_it)] = (b_it->getOrder());
			}
		}
		else
		{
			//Log.error() << "ILP could not be solved successfully" << endl;
			valid = false;
		}

		// free
		delete_lp(lp);
	}

	AssignBondOrderProcessor::ILPSolution_::~ILPSolution_()
	{
	}
	
	void AssignBondOrderProcessor::ILPSolution_::clear()
	{
		valid = false;
		bond_orders.clear();
		penalty = 0;
		num_bonds = 0;
	}


} // namespace BALL
