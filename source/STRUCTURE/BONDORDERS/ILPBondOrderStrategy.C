#include <BALL/STRUCTURE/BONDORDERS/ILPBondOrderStrategy.h>
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/KERNEL/forEach.h>

#include <lpsolve/lp_lib.h>

namespace BALL
{
	ILPBondOrderStrategy::ILPBondOrderStrategy(AssignBondOrderProcessor* parent)
		: BondOrderAssignmentStrategy(parent),
			valid_(false),
			index_to_free_bond_(),
			number_of_free_bonds_(),
			const_penalty_(0.f),
			first_solution_(true),
			ilp_(0)
	{
	}

	ILPBondOrderStrategy::~ILPBondOrderStrategy()
	{
		clear();
	}

	void ILPBondOrderStrategy::clear()
	{
		valid_ = false;

		index_to_free_bond_.clear();
		number_of_free_bonds_ = 0;
		const_penalty_ = 0.f;

		first_solution_ = true;

		if (ilp_)
			delete_lp(ilp_);

		ilp_ = 0;
	}

	bool ILPBondOrderStrategy::readOptions(const Options& /*options*/)
	{
		return true;
	}

	void ILPBondOrderStrategy::setDefaultOptions()
	{
	}

	void ILPBondOrderStrategy::init()
	{
		first_solution_ = true;

		// generate the ILP
		const_penalty_ = 0.f;
		
		// Check number of variables with prefix 'x'
		Position total_no_bonds = abop->ac_->countBonds();

		// Number of atoms in the system
		Position no_atoms = abop->ac_->countAtoms();

		// Mapping from (free) bonds onto bond variable indices
		std::map<Bond*, Position> bond_map;

		// Vector for mapping from variable indices onto bonds
		index_to_free_bond_.resize(total_no_bonds, (Bond*)0);

		// Count the choice constraints
		Position no_y = 0;

		// Count the number of bonds whose orders should be calculated
		number_of_free_bonds_ = 0;

		// Maximum number of row entries (in penalty table)
		// NOTE: we determine this number later in the code, since we do not
		//			 know yet which atoms have to be considered and which are fixed
		Position max_no_ent=0;

		// Maximum number of bonds of an atom
		Position max_no_atom_bonds = 0;
		
		// compute the maps, variables ...
		for (Position current_atom = 0; current_atom < no_atoms; ++current_atom)
		{
			Atom* at1 = abop->ac_->getAtom(current_atom);
			Position consider_bonds = 0;
			int valence = 0;
			for (Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
			{
				valence += bit->getOrder();
				if (!abop->bond_fixed_[&*bit]) // this bond is free => calculate bond order
				{
					++consider_bonds;
					if (bond_map.find(&*bit) == bond_map.end())
					{
						bond_map[&*bit] = number_of_free_bonds_;
						index_to_free_bond_[number_of_free_bonds_] = &*bit;
						++number_of_free_bonds_;
					}
				}
			} // end of for all bonds of this atom

			if (abop->add_missing_hydrogens_)
			{
				Size total_length = 0;
				for(Position j = 0; j < abop->atom_to_block_[current_atom].size(); ++j)
				{
					Size current_length = abop->block_to_length_[abop->atom_to_block_[current_atom][j]];
					no_y += current_length;
					total_length += current_length;
				}
				max_no_ent = std::max(max_no_ent, total_length);
			}
			else if (consider_bonds)
			{
				Size current_length = abop->block_to_length_[abop->atom_to_block_[current_atom][0]];
				
				no_y += current_length;
				max_no_ent = std::max(max_no_ent, current_length);
			}
			else
			{
				int pos = valence - abop->block_to_start_valence_[abop->atom_to_block_[current_atom][0]];
				if (pos >= 0)
				{
					const_penalty_ += abop->penalties_[abop->block_to_start_idx_[abop->atom_to_block_[current_atom][0]] + pos];
				}
			}
			
			max_no_atom_bonds = std::max(consider_bonds, max_no_atom_bonds);

		} // end of for all atoms
		
		// Decision variables for every bond order
		number_of_free_bonds_ *= abop->max_bond_order_;
		
		// Number of variables of the whole ILP
		Position no_vars = number_of_free_bonds_ + no_y;
		
		// Create a new model with 'no_vars' variables 
		// (columns) and 0 rows
		ilp_ = make_lp(0, no_vars);
		if (!ilp_)
		{
			Log.error() << "ILPBondOrderStrategy::init(): Creation of ILP failed" << std::endl;

			return;
		}

		// Set the binary and integer constraints
		// Variables for bond orders are integer
		/*for(Position i = 1; i <= ilp_number_of_free_bonds_; ++i)
		{
			set_int(ilp_, i, TRUE);
		}*/
		// Decision variables for atom valences are binary 
		//for(Position i = number_of_free_bonds_+1; i <= no_vars; ++i)
		for(Position i = 1; i <= no_vars; ++i)
		{
			set_binary(ilp_, i, TRUE);
		}

		// Create space large enough for one row and the objective function,
		// use lp_solves internal types.

		// Indices of the variables (constraints)
		std::vector<int> cons_indices(max_no_atom_bonds*abop->max_bond_order_ + 1 + max_no_ent,0); // colno
		// Prefactors (constraints)
		std::vector<REAL> cons_prefactors(max_no_atom_bonds*abop->max_bond_order_ + 1 + max_no_ent, 0.); // row
		// Indices of the variables (objective function)
		std::vector<int> obj_indices(no_y,0); // obj_colno
		// Prefactors (objective function)
		std::vector<REAL> obj_prefactors(no_y, 0.); // obj_row

		// Factors for choice constraints
		std::vector<REAL> choices(max_no_ent, 1.);

		// Makes building the model faster if it's done
		// row by row
		set_add_rowmode(ilp_, TRUE);

		// Set the choice constraints for bonds
		for(Position i = 1; i <= number_of_free_bonds_;)
		{
			for (int j = 0; j < abop->max_bond_order_; ++j, ++i)
			{
				cons_indices[j] = i;
			}
			if (!add_constraintex(ilp_, abop->max_bond_order_, &choices[0], &cons_indices[0], EQ, 1))
			{
				Log.error() << "ILPBondOrderStrategy::init(): Setting bond choice constraint for ILP failed" << std::endl;

				return;
			}
		}
		
		// Create all remaining constraints and the objective function
		// Indices of the choice variables (y)
		Position ind_y = 0;

		for(Position i = 0; i < no_atoms; ++i)
		{
			// Find corresponding variables
			Atom* at1 = abop->ac_->getAtom(i);

			// Count the bonds of this atom whose
			// orders should be calculated
			Position count_b = 0;
			for(Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
			{
				std::map<Bond*, unsigned int>::iterator it = bond_map.find(&(*bit));
				if (it != bond_map.end())
				{
					for(int k = 1; k <= abop->max_bond_order_; ++k, ++count_b)
					{
						cons_indices[count_b] = it->second*abop->max_bond_order_ + k;
						cons_prefactors[count_b] = -k;
					}
				}
			}

			if (abop->add_missing_hydrogens_)
			{
				// Create indices for the choice variables and set
				// the entries for the valence constraint and the
				// objective function

				// Count the number of variables participating in the current cons_prefactors
				Position count_vars = count_b;
				Position total_length = 0;
				for(Position h = 0; h < abop->atom_to_block_[i].size(); ++h)
				{
					total_length += abop->block_to_length_[abop->atom_to_block_[i][h]];
					for(Position j = 0; j < abop->block_to_length_[abop->atom_to_block_[i][h]]; ++j, ++ind_y, ++count_vars)
					{
						// Choice variables have indices > ilp_number_of_free_bonds_ in variable vector for ILP
						obj_indices[ind_y]  = ind_y + number_of_free_bonds_ + 1;
						cons_indices[count_vars] = obj_indices[ind_y];
						
						// Set valences
						cons_prefactors[count_vars] = std::max(((REAL)abop->block_to_start_valence_[abop->atom_to_block_[i][h]]) + j - h, 0.);
						
						// Set penalties 
						obj_prefactors[ind_y] = (abop->penalties_[abop->block_to_start_idx_[abop->atom_to_block_[i][h]] + j]);
					}
				}
				
				// in case we got an empty penalty cons_prefactors 
				if (total_length != 0)
				{
					// Add valence constraint
					if (!add_constraintex(ilp_, count_vars, &cons_prefactors[0], &cons_indices[0], EQ,
							 abop->fixed_val_[i]))
					{
						Log.error() << "ILPBondOrderStrategy::init(): Setting valence constraint for ILP failed" << std::endl;

						return;
					}
					// Add choice constraint 
					if (!add_constraintex(ilp_, total_length, &choices[0], &cons_indices[count_b], EQ, 1))
					{
						Log.error() << "ILPBondOrderStrategy::init(): Setting valence choice constraint for ILP failed" << std::endl;

						return;
					}
				}
				
			}
			else if (count_b)
			{
				// This atom has bonds whose orders should be calculated

				// Create indices for the choice variables and set
				// the entries for the valence constraint and the
				// objective function

				// Count the number of variables participating in the current cons_prefactors
				Position count_vars = count_b; 
				for(Position j = 0; j < abop->block_to_length_[abop->atom_to_block_[i][0]]; ++j, ++ind_y, ++count_vars)
				{
					// Choice variables have indices > ilp_number_of_free_bonds_ in variable vector for ILP
					obj_indices[ind_y]  = ind_y + number_of_free_bonds_ + 1;
					cons_indices[count_vars] = obj_indices[ind_y];

					// Set valences  
					cons_prefactors[count_vars] = abop->block_to_start_valence_[abop->atom_to_block_[i][0]] + j;

					// Set penalties 
					obj_prefactors[ind_y]       = (abop->penalties_[abop->block_to_start_idx_[abop->atom_to_block_[i][0]] + j]);
				}

				// in case we got an empty penalty cons_prefactors 
				if (abop->block_to_length_[abop->atom_to_block_[i][0]] != 0)
				{
					// Add valence constraint
					if (!add_constraintex(ilp_, count_vars, &cons_prefactors[0], &cons_indices[0], EQ,
								abop->fixed_val_[i]))
					{
						Log.error() << "ILPBondOrderStrategy::init(): Setting valence constraint for ILP failed" << std::endl;

						return;
					}
					// Add choice constraint 
					if (!add_constraintex(ilp_, abop->block_to_length_[abop->atom_to_block_[i][0]], &choices[0], &cons_indices[count_b], EQ, 1))
					{
						Log.error() << "ILPBondOrderStrategy::init(): Setting choice constraint for ILP failed" << std::endl;

						return;
					}
				}
			}
		}

		// Rowmode should be turned off again after building the model
		set_add_rowmode(ilp_, FALSE);

		// Set objective function
		if (!set_obj_fnex(ilp_, obj_prefactors.size(), &obj_prefactors[0], &obj_indices[0]))
		{
			Log.error() << "ILPBondOrderStrategy::init(): Setting objective function for ILP failed" << std::endl;
		}

		// Tell lp_solve that this problem is a minimization problem
		set_minim(ilp_);

		// We only want to see important messages on screen while solving
		set_verbose(ilp_, IMPORTANT);

		// Show the generated MIP
		//write_LP(ilp_, stdout);
		valid_ = true;
	}

	boost::shared_ptr<BondOrderAssignment> ILPBondOrderStrategy::computeNextSolution()
	{
		boost::shared_ptr<BondOrderAssignment> solution;

		// if this is not the first time we try to solve the ilp, we need to add additional
		// constraints to reach the next optimal solution
		if (!first_solution_)
		{
			// NOTE: lpsolve5.5 despite claiming does not support this!
			// Thus, we include this feature by incorporating constraints which forbid old solutions

			// Get old variables
			REAL *vars;
			get_ptr_variables(ilp_, &vars);

			set_add_rowmode(ilp_, TRUE);

			// Indices of the variables (constraints)
			std::vector<int> cons_indices(number_of_free_bonds_+1); // colno
			// Prefactors (constraints)
			std::vector<REAL> cons_prefactors(number_of_free_bonds_+1,1.); // row

			Position j = 0;
			for(Position i = 0; i < number_of_free_bonds_; ++i)
			{
				if (fabs(vars[i]-1.) < 1.e-5)
				{
					cons_indices[j] = i+1;
					++j;
				}
			}

			if (!add_constraintex(ilp_, j, &cons_prefactors[0], &cons_indices[0], LE, (REAL)j-1))
			{
				Log.error() << "AssignBondOrderProcessor: Adding constraint for next solution failed" << std::endl;

				return solution;
			}

			set_add_rowmode(ilp_, FALSE);

			// Reset the basis to reinitialize the solve process
			reset_basis(ilp_);
		}

		first_solution_ = false;

		// Let lp_solve solve our problem
		int ret = solve(ilp_);

		// Check whether lp_solve could do its job successfully
		if (ret == OPTIMAL)
		{
			solution = boost::shared_ptr<BondOrderAssignment>(new BondOrderAssignment(abop));

			solution->ac = abop->ac_;
			// we got a valid solution
			solution->valid = true;

			// store the bond orders of _ALL_ bonds to offer a complete solution 
			AtomIterator a_it = abop->ac_->beginAtom();
			Atom::BondIterator b_it = a_it->beginBond();
			BALL_FOREACH_BOND(*abop->ac_, a_it, b_it)
			{
				solution->bond_order_map[&(*b_it)] = (b_it->getOrder());
			}

			// Get variables
			REAL *vars;
			get_ptr_variables(ilp_, &vars);

			solution->estimated_atom_type_penalty = get_objective(ilp_) + const_penalty_;

			// Do the assignment of the bond orders
			for(Position i = 0; i < number_of_free_bonds_; )
			{
				for(int j = 1; j <= abop->max_bond_order_; ++j, ++i)
				{
					if (fabs(vars[i] - 1.) < 1.e-10)
					{
						switch (j)
						{
							case 1:
							{
								solution->bond_order_map[index_to_free_bond_[i/abop->max_bond_order_]] = Bond::ORDER__SINGLE;
								break;
							}
							case 2:
							{
								solution->bond_order_map[index_to_free_bond_[i/abop->max_bond_order_]] = Bond::ORDER__DOUBLE;
								break;
							}
							case 3:
							{
								solution->bond_order_map[index_to_free_bond_[i/abop->max_bond_order_]] = Bond::ORDER__TRIPLE;
								break;
							}
							default:
							{
								solution->bond_order_map[index_to_free_bond_[i/abop->max_bond_order_]] = Bond::ORDER__QUADRUPLE;
							}
						}
					}
				}
			}

			if (abop->add_missing_hydrogens_)
			{
				// Store the atoms which need additional hydrogens
				Position no_atoms = abop->ac_->countAtoms();
				Position current_var = number_of_free_bonds_;
				for (Position current_atom = 0; current_atom < no_atoms; ++current_atom)
				{
					current_var += abop->block_to_length_[abop->atom_to_block_[current_atom][0]];

					// Note: If there are no hydrogens to be added there is no need to check the variables
					for(Position j = 1; j < abop->atom_to_block_[current_atom].size(); ++j)
					{
						Size current_length = abop->block_to_length_[abop->atom_to_block_[current_atom][j]];

						for(Position k = 0; k < current_length; ++k, ++current_var)
						{
							if (fabs(vars[current_var] - 1.) < 1.e-10)
							{
								solution->number_of_virtual_hydrogens[abop->ac_->getAtom(current_atom)] = j;
							}
						}
					}
				}
			}
		}
		else
		{
			Log.error() << "ILPBondOrderStrategy::computeNextSolution(): ILP could not be solved successfully, reason: " << ret << std::endl;
		}

		return solution;
	}

}
