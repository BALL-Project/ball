#include <iostream>
#include <map>

#ifndef ATOMTYPES_H
#include <BALL/STRUCTURE/atomtypes.h>
#endif

#ifndef BONDORDERASSIGNER_H
#include <BALL/STRUCTURE/bondorderAssigner.h>
#endif 
 
#ifndef VSGENERATOR_H
#include <BALL/STRUCTURE/vSgenerator.h>
#endif	

// BALL includes
#include <BALL/kernel.h>
#include <BALL/SYSTEM/timer.h>

// For lp_solve
#include <lp_lib.h>

#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/COMMON/limits.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/FORMAT/resourceFile.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>

using namespace std;

namespace BALL 
{	
	//const char* AssignBondOrderProcessor::Option::OVERWRITE_UNKNOWN_BOND_ORDERS = "overwrite_unknown_bond_orders";
	//const bool  AssignBondOrderProcessor::Default::OVERWRITE_UNKNOWN_BOND_ORDERS = true;
	
	const char* AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS = "overwrite_single_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_SINGLE_BOND_ORDERS = false;
	
	const char* AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS = "overwrite_double_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_DOUBLE_BOND_ORDERS = false;
	
	const char* AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS = "overwrite_triple_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_TRIPLE_BOND_ORDERS = false;

	const char* AssignBondOrderProcessor::Option::OVERWRITE_QUADRUPLE_BOND_ORDERS = "overwrite_quadruple_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_QUADRUPLE_BOND_ORDERS = false;

	const char* AssignBondOrderProcessor::Option::OVERWRITE_AROMATIC_BOND_ORDERS = "overwrite_aromatic_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_AROMATIC_BOND_ORDERS = false;

	const char* AssignBondOrderProcessor::Option::OVERWRITE_CHARGES = "overwrite_existing_charges";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_CHARGES = false;
	
	const char* AssignBondOrderProcessor::Option::ASSIGN_CHARGES = "assign_charges";
	const bool  AssignBondOrderProcessor::Default::ASSIGN_CHARGES = false;


	AssignBondOrderProcessor::AssignBondOrderProcessor()
		: UnaryProcessor<AtomContainer>(),
			options(),
			num_bonds_(0)	
	{
		setDefaultOptions();
	}

	AssignBondOrderProcessor::AssignBondOrderProcessor(const AssignBondOrderProcessor& abop)
		:	UnaryProcessor<AtomContainer>(abop),
			options(),
			num_bonds_(0)
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
		num_bonds_ = 0;
		return true;
	}

	Size AssignBondOrderProcessor::getNumberOfBondOrdersSet()
	{
		return num_bonds_;
	}

	Processor::Result AssignBondOrderProcessor::operator () (AtomContainer& ac)
	{
		// speed up the code by temporarily store the options locally 
		bool osbo = options.getBool(Option::OVERWRITE_SINGLE_BOND_ORDERS);
		bool odbo = options.getBool(Option::OVERWRITE_DOUBLE_BOND_ORDERS);
		bool otbo = options.getBool(Option::OVERWRITE_TRIPLE_BOND_ORDERS);
		bool oqbo = options.getBool(Option::OVERWRITE_QUADRUPLE_BOND_ORDERS);
		bool oabo = options.getBool(Option::OVERWRITE_AROMATIC_BOND_ORDERS);


		// what kind of composite do we have?
		if (RTTI::isKindOf<Molecule>(ac))
		{
			/*	// consider  all bonds of type  TYPE__COVALENT (especially not TYPE__DISULPHIDE_BRIDGE and not TYPE__HYDROGEN)
					AtomIterator ait;
					Atom::BondIterator bit;
					BALL_FOREACH_BOND(ac, ait, bit)
					{
					if ((bit->getType() == Bond::TYPE__COVALENT) || (bit->getType() == Bond::TYPE__UNKNOWN))
					{
			// add to the list 
			//bit->getSecondAtom()
			//bit;
			}
			}*/

			// Check number of variables with prefix 'x'
			// and map bonds onto its variables
			Position total_no_bonds = ac.countBonds();

			// Number of atoms in the system
			Position no_atoms = ac.countAtoms();

			// Mapping from bonds onto bond variable indices
			map<Bond*, Position> bond_map;

			// Vector for mapping from variable indices onto bonds
			std::vector<Bond*> ind_bond(total_no_bonds, (Bond*)0);

			// Vector for storing fixed atom valences
			std::vector<Position> fixed_val(total_no_bonds, 0);

			// Generate penalty values
			VSgenerator gen;
			gen.GenerateAPS(dynamic_cast<Molecule*>(&ac));
			vector<vector<pair<int, int> > >& av_vec(gen.atomic_penalty_scores_);

			// Count the choice constraints
			Position no_y = 0;

			// Count the number of bonds whose orders should be calculated
			Position no_x = 0;

			// Maximum number of row entries (in penalty table)
			Position max_no_ent = 0;

			for (Position i = 0; i < no_atoms; ++i)
			{
				Atom* at1 = ac.getAtom(i);
				Position fixed = 0;

				bool consider_atom = false;
				for (Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
				{
					Bond* bnd = &(*bit);

					// accoring to the options and the given bond order 
					// the acutal bond is a free variable of the ILP or not
					// YES: add a variable in the bond side constraint + ???? 
					// NO: equality in the bond side constraint +  ?????
					bool consider_bond = true;
					switch (bnd->getOrder())
					{
						case Bond::ORDER__SINGLE:
							{
								if (!osbo)
								{
									fixed += 1;
									consider_bond = false;
								}
								break;
							}
						case Bond::ORDER__DOUBLE:
							{
								if (!odbo)
								{
									fixed += 2;
									consider_bond = false;
								}
								break;
							}
						case Bond::ORDER__TRIPLE:
							{
								if (!otbo)
								{
									fixed += 3;
									consider_bond = false;
								}
								break;
							}
						case Bond::ORDER__QUADRUPLE:
							{
								if (!oqbo)
								{
									fixed += 4;
									consider_bond = false;
								}
								break;
							}
						case Bond::ORDER__AROMATIC:
							{
								if (!osbo)
								{
									fixed += 1; ///????????????
									consider_bond = false;
								}
								break;
							}
					}

					// In all other cases we should calculate the bond order
					if (consider_bond)
					{
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

				if (consider_atom)
				{
					no_y += av_vec[i].size();
					if (av_vec[i].size() > max_no_ent)
					{
						max_no_ent = av_vec[i].size();
					}
				}

				fixed_val[i] = fixed;
			}

			Position no_vars = no_x + no_y;

			//cout << no_vars << endl;

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
			std::vector<int> colno(5 + max_no_ent,0);
			std::vector<REAL> row(5 + max_no_ent,0);
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
					for(Position j = 0; j < av_vec[i].size(); ++j, ++ind_y, ++count_vars)
					{
						// Choice variables have indices > no_x in variable vector for ILP
						obj_colno[ind_y]   = ind_y + no_x + 1;
						colno[count_vars] = obj_colno[ind_y];

						// Set valences
						row[count_vars]   = av_vec[i][j].first;

						// Set penalties
						obj_row[ind_y]     = av_vec[i][j].second;
					}
					
					// Add valence constraint
					if (!add_constraintex(lp, count_vars, &row[0], &colno[0], EQ, 
								fixed_val[i]))
					{
						Log.error() << "Setting valence constraint for ILP failed" << endl;
					}

					// Add choice constraint
					if (!add_constraintex(lp, av_vec[i].size(), &choices[0], &colno[count_b], 
								EQ, 1))
					{
						Log.error() << "Setting choice constraint for ILP failed" << endl;
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
			if (ret != OPTIMAL)
			{
				Log.error() << "ILP could not be solved successfully" << endl;
			}

			// Get variables
			REAL *vars;
			get_ptr_variables(lp, &vars);

			// count the assigned bond orders
			num_bonds_ = 0;
			// Do the assignment of the bond orders
			for(Position i = 0; i < no_x; ++i)
			{
				if (fabs(vars[i] - 1.) < 1.e-10)
				{
					ind_bond[i]->setOrder(Bond::ORDER__SINGLE);
					num_bonds_++;
					continue;
				}
				if (fabs(vars[i] - 2.) < 1.e-10)
				{
					ind_bond[i]->setOrder(Bond::ORDER__DOUBLE);	
					num_bonds_++;
					continue;
				}
				if (fabs(vars[i] - 3.) < 1.e-10)
				{
					ind_bond[i]->setOrder(Bond::ORDER__TRIPLE);	
					num_bonds_++;
					continue;
				}
				if (fabs(vars[i] - 4.) < 1.e-10)
				{
					ind_bond[i]->setOrder(Bond::ORDER__QUADRUPLE);
					num_bonds_++;
					continue;
				}
			}

			delete_lp(lp);


		}

		return Processor::CONTINUE;
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
	}
	
} // namespace BALL
