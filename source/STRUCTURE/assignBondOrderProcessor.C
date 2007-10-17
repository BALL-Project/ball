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
	const char* AssignBondOrderProcessor::Option::DELETE_EXISTING_BOND_ORDERS = "delete_existing_bond_orders";
	const bool  AssignBondOrderProcessor::Default::DELETE_EXISTING_BOND_ORDERS = true;

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

		/*if (options.getBool(AssignBondOrderProcessor::Option::DELETE_EXISTING_BOND_ORDERS))
		{
			AtomIterator ait;
			BALL_FOREACH_ATOM(ac, ait)
			{
				ait->destroyBonds();
			}
		}*/
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
			int no_x = ac.countBonds();

			// Number of atoms in the system
			int no_atoms = ac.countAtoms();

			// Mapping from bonds onto bond variable indices
			map<Bond*, unsigned int> bond_map;

			// Vector for mapping from variable indices onto bonds
			std::vector<Bond*> ind_bond(no_x, (Bond*)0);

			Timer timer;
			timer.start();

			int counter = 1;
			for(int i = 0; i < no_atoms; ++i)
			{
				Atom* at1 = ac.getAtom(i);
				for(Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
				{
					Bond* bnd = &(*bit);
					pair<map<Bond*, unsigned int>::iterator, bool> p 
						= bond_map.insert(make_pair(bnd, counter));
					if (p.second)
					{
						ind_bond[counter-1] = bnd;
						++counter;
					}
				}
			}

			cout << "Time used to build bond map: " << timer.getCPUTime() << endl;

			if (counter-1 != no_x)
			{
				cout << "Something went wrong! Number of counted bonds does not match the number of returned bonds" << endl;
				exit(-2);
			}

			timer.reset();
	cout << "VOr Konstruktor " << ac.countBonds() << endl;

			// Generate penalty values
			VSgenerator gen;
	
	cout << " dazwischen" << ac.countBonds() << endl;

			gen.GenerateAPS(dynamic_cast<Molecule*>(&ac));
			vector<vector<pair<int, int> > >& av_vec(gen.atomic_penalty_scores_);
	cout << "nach Sabine" << ac.countBonds() << endl;

			cout << "Time used to build penalty vector: " << timer.getCPUTime() << endl;

			timer.reset();

			// Count variables
			int no_vars = no_x;

			cout << "Number of bonds: " << no_x << endl;

			// 'y' variables
			for(int i = 0; i < no_atoms; ++i)
			{
				no_vars += av_vec[i].size();
			}

			cout << "Number of variables: " << no_vars << endl;

			// Create a new model with 'no_vars' variables 
			// (columns) and 0 rows
			lprec *lp = make_lp(0,no_vars);

			cout << "1 " << ac.countBonds() << endl;

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
				if (!add_constraintex(lp, 1, &row[0], &colno[0], LE, 3))
				{
					cout << "Something went wrong" << endl;
				}
				if (!add_constraintex(lp, 1, &row[0], &colno[0], GE, 1))
				{
					cout << "Something went wrong" << endl;
				}
			}
     
			cout << "2 " << ac.countBonds() << endl;

			// Choice constraints
			for(int i = 0, k = no_x + 1; i < no_atoms; ++i)
			{
				for(unsigned int j = 0; j < av_vec[i].size(); ++j, ++k)
				{
					colno[j] = k;
					row[j]   = 1;
				}
				if (!add_constraintex(lp, av_vec[i].size(), &row[0], &colno[0], EQ, 1))
				{
					cout << "Something went wrong" << endl;
				}
			}
	
			cout << "3 " << ac.countBonds() << endl;


			// Create valence constraints
			for(int i = 0, k = no_x + 1; i < no_atoms; ++i)
			{
				int r = 0;
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
					num_bonds++;
					Bond* bnd = &(*bit);
					map<Bond*, unsigned int>::iterator it = bond_map.find(bnd);

					if (it == bond_map.end())
					{
						cout << "Something went wrong. Current bond not found!" << endl;
					}

					colno[r] = it->second;
					row[r]   = -1;
				}
				cout << num_bonds << endl;

				if (!add_constraintex(lp, r, &row[0], &colno[0], EQ, 0))
				{
					cout << "Something went wrong" << endl;
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
			write_LP(lp, stdout);

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
		options.setDefaultBool(AssignBondOrderProcessor::Option::DELETE_EXISTING_BOND_ORDERS,
													 AssignBondOrderProcessor::Default::DELETE_EXISTING_BOND_ORDERS);
	}
	
} // namespace BALL
