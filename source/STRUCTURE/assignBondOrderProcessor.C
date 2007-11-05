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
	const char* AssignBondOrderProcessor::Option::OVERWRITE_UNKNOWN_BOND_ORDERS = "overwrite_unknown_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_UNKNOWN_BOND_ORDERS = true;
	
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
		bool oubo = options.getBool(Option::OVERWRITE_UNKNOWN_BOND_ORDERS);
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
			int total_no_bonds = ac.countBonds();

			// Number of atoms in the system
			int no_atoms = ac.countAtoms();

			// Mapping from bonds onto bond variable indices
			map<Bond*, unsigned int> bond_map;

			// Vector for mapping from variable indices onto bonds
			std::vector<Bond*> ind_bond(total_no_bonds, (Bond*)0);

			// Vector for storing fixed valences
			std::vector<Position> fixed_val(no_atoms, 0);

			Timer timer;
			timer.start();

			// count the changeable bonds
			int counter = 0;
			for(int i = 0; i < no_atoms; ++i)
			{
				Atom* at1 = ac.getAtom(i);
				Position fixed = 0;
				for(Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
				{
					Bond* bnd = &(*bit);

					// accoring to the options and the given bond order 
					// the acutal bond is a free variable of the ILP or not
					// YES: add a variable in the bond side constraint + ???? 
					// NO: equality in the bond side constraint +  ?????
					bool change = true;
					switch (bnd->getOrder())
					{
						case Bond::ORDER__SINGLE:
						{
							if (!osbo)
							{
								fixed += 1;
								change = false;
							}
							break;
						}
						case Bond::ORDER__DOUBLE:
						{
							if (!odbo)
							{
								fixed += 2;
								change = false;
							}
							break;
						}
						case Bond::ORDER__TRIPLE:
						{
							if (!otbo)
							{
								fixed += 3;
								change = false;
							}
							break;
						}
						case Bond::ORDER__QUADRUPLE:
						{
							if (!oqbo)
							{
								fixed += 4;
								change = false;
							}
							break;
						}
						case Bond::ORDER__AROMATIC:
						{
							if (!osbo)
							{
								fixed += 1; ///????????????
								change = false;
							}
							break;
						}
					}
					
					// In all other cases we should calculate the bond order
					if (change)
					{
						pair<map<Bond*, unsigned int>::iterator, bool> p 
							= bond_map.insert(make_pair(bnd, counter));
						if (p.second)
						{
							ind_bond[counter] = bnd;
							++counter;
						}
					}
				}
				fixed_val[i] = fixed;
			}
			
			Position no_x = counter;
			ind_bond.resize(no_x);

			//cout << "Time used to build bond map: " << timer.getCPUTime() << endl;

			timer.reset();

			// Generate penalty values
			VSgenerator gen;
	

			gen.GenerateAPS(dynamic_cast<Molecule*>(&ac));
			vector<vector<pair<int, int> > >& av_vec(gen.atomic_penalty_scores_);

			//cout << "Time used to build penalty vector: " << timer.getCPUTime() << endl;

			timer.reset();

			// Count variables
			int no_vars = no_x;

			//cout << "Number of bonds: " << no_x << endl;

			// 'y' variables
			for(int i = 0; i < no_atoms; ++i)
			{

				no_vars += av_vec[i].size();
			}

			//cout << "Number of variables: " << no_vars << endl;

			// Create a new model with 'no_vars' variables 
			// (columns) and 0 rows
			lprec *lp = make_lp(0,no_vars);


			if (!lp)
			{
				cout << "Something went wrong" << endl;
			}

			// Set the binary and integer constraints
			for(int i = 1; i <= no_x; ++i)
			{
				set_int(lp, i, TRUE);
			}
			for(int i = no_x+1; i <= no_vars; ++i)
			{
				set_binary(lp, i, TRUE);
			}

			// Create space large enough for one row,
			// use lp_solves internal types.
			std::vector<int> colno(no_vars-no_x+1);
			std::vector<REAL> row(no_vars-no_x+1);



			// Makes building the model faster if it's done
			// row by row
			set_add_rowmode(lp, TRUE);

			// Set the lower and upper constraints for bonds
			for(int i = 1; i <= no_x; ++i)
			{
				colno[0] = i;
				row[0]   = 1;

				if (!add_constraintex(lp, 1, &row[0], &colno[0], LE, 4))
				{
					cout << "Something went wrong" << endl;
				}
				if (!add_constraintex(lp, 1, &row[0], &colno[0], GE, 1))
				{
					cout << "Something went wrong" << endl;
				}
			}	

			// Create space large enough for one row,
			// use lp_solves internal types.
			std::vector<int> obj_colno(no_vars-no_x+1);
			std::vector<REAL> obj_row(no_vars-no_x+1);


			// Create valence constraints
			for(int i = 0, k = no_x + 1; i < no_atoms; ++i)
			{
				int r = 0;
				int k_save = k;
				for(unsigned int j = 0; j < av_vec[i].size(); ++j, ++k, ++r)
				{
					colno[r] = k;
					row[r]   = av_vec[i][j].first;
				}

				// Find corresponding variables
				Atom* at1 = ac.getAtom(i);
				cout << at1->getElement() << "-------------";
				Size num_bonds = 0;
				for(Atom::BondIterator bit = at1->beginBond(); +bit; ++bit, ++r)
				{
					Bond* bnd = &(*bit);
					map<Bond*, unsigned int>::iterator it = bond_map.find(bnd);

					if (it != bond_map.end())
					{
						++num_bonds;
						colno[r] = it->second;
						row[r]   = -1;
					}
				}
				//cout << num_bonds << endl;

				if (num_bonds)
				{
					if (!add_constraintex(lp, r, &row[0], &colno[0], EQ, fixed_val[i]))
					{
						cout << "Something went wrong" << endl;
					}

					// Choice constraints
					for(unsigned int j = 0, t = k_save; j < av_vec[i].size(); ++j, ++t)
					{
						colno[j] = t;
						row[j]   = 1;
						obj_colno[j] = t;
					}
					if (!add_constraintex(lp, av_vec[i].size(), &row[0], &colno[0], EQ, 1))
					{
						cout << "Something went wrong" << endl;
					}
				}
			}


			// Rowmode should be turned off again after building the model
			set_add_rowmode(lp, FALSE);

			// Set objective function
			for(int i = 0, k = no_x + 1, r = 0; i < no_atoms; ++i)
			{
				for(unsigned int j = 0; j < av_vec[i].size(); ++j, ++r, ++k)
				{
					colno[r] = k;
					row[r]   = av_vec[i][j].second;
				}
			}
			if (!set_obj_fnex(lp, no_vars-no_x, &row[0], &colno[0]))
			{
				cout << "Something went wrong" << endl;
			}

			// Tell lp_solve that this problem is a minimization problem
			set_minim(lp);

			// We only want to see important messages on screen while solving
			set_verbose(lp, IMPORTANT);

			// Show the generated MIP
			//write_LP(lp, stdout);

			cout << "Time used to create MILP: " << timer.getCPUTime() << endl;

			timer.reset();

			// Let lp_solve solve our problem
			int ret = solve(lp);

			cout << "Time used to solve MILP: " << timer.getCPUTime() << endl;

			// Check whether lp_solve could do its job successfully
			if (ret == OPTIMAL)
			{
				cout << "Hurray" << endl;
			}
			else
			{
				cout << "Bullshit" << endl;
			}

			// Get variables
			REAL *vars;
			get_ptr_variables(lp, &vars);

			// Do the assignment of the bond orders
			for(int i = 0; i < no_x; ++i)
			{
				if (fabs(vars[i] - 1.) < 1.e-10)
				{
					ind_bond[i]->setOrder(Bond::ORDER__SINGLE);
					continue;
				}
				if (fabs(vars[i] - 2.) < 1.e-10)
				{
					ind_bond[i]->setOrder(Bond::ORDER__DOUBLE);
					continue;
				}
				if (fabs(vars[i] - 3.) < 1.e-10)
				{
					ind_bond[i]->setOrder(Bond::ORDER__TRIPLE);
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


	/*	HashMap<String, Bond::BondOrder> name_to_order;
		name_to_order["single"] = Bond::ORDER__SINGLE;
		name_to_order["double"] = Bond::ORDER__DOUBLE;
		name_to_order["triple"] = Bond::ORDER__TRIPLE;
		name_to_order["aromatic"] = Bond::ORDER__AROMATIC;
	Bond::BondOrder order = Bond::ORDER__UNKNOWN;
*/
	void AssignBondOrderProcessor::setDefaultOptions()
	{		
	 	options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_UNKNOWN_BOND_ORDERS,
	 												 AssignBondOrderProcessor::Default::OVERWRITE_UNKNOWN_BOND_ORDERS);
	
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
