// $Id: fresnoRotation.C,v 1.1.2.4 2002/03/15 14:48:03 anker Exp $
// Molecular Mechanics: Fresno force field, lipophilic component

#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/STRUCTURE/geometricProperties.h>

#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/support.h>

#include <BALL/MOLMEC/FRESNO/fresno.h>
#include <BALL/MOLMEC/FRESNO/fresnoRotation.h>

using namespace std;

namespace BALL
{

	FresnoRotation::FresnoRotation()
		throw()
		:	ForceFieldComponent(),
			rotatable_bonds_(),
			N_rot_(0),
			is_frozen_(),
			heavy_atom_fractions_(),
			grid_(0),
			grid_spacing_(0.0),
			fresno_types_(0)
	{
		// set component name
		setName("Fresno Rotation");
	}


	FresnoRotation::FresnoRotation(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field),
			rotatable_bonds_(),
			N_rot_(0),
			is_frozen_(),
			heavy_atom_fractions_(),
			grid_(0),
			grid_spacing_(0.0),
			fresno_types_(0)
	{
		// set component name
		setName("Fresno Rotation");
	}


	FresnoRotation::FresnoRotation(const FresnoRotation& fr, bool deep)
		throw()
		:	ForceFieldComponent(fr, deep),
			rotatable_bonds_(fr.rotatable_bonds_),
			N_rot_(fr.N_rot_),
			is_frozen_(fr.is_frozen_),
			heavy_atom_fractions_(fr.heavy_atom_fractions_),
			grid_(fr.grid_),
			grid_spacing_(fr.grid_spacing_),
			fresno_types_(fr.fresno_types_)
	{
	}


	FresnoRotation::~FresnoRotation()
		throw()
	{
		if (grid_ != 0)
		{
			delete grid_;
		}
		clear();
	}


	void FresnoRotation::clear()
		throw()
	{
		rotatable_bonds_.clear();
		N_rot_ = 0;
		is_frozen_.clear();
		heavy_atom_fractions_.clear();
		if (grid_ != 0) grid_->clear();
		grid_spacing_ = 0.0;
		fresno_types_ = 0;
		// ?????
		// ForceFieldComponent does not comply with the OCI
		// ForceFieldComponent::clear();
	}


	bool FresnoRotation::setup()
		throw()
	{
		ForceField* force_field = getForceField();
		if (force_field == 0)
		{
			Log.error() << "FresnoRotation::setup(): "
				<< "component not bound to force field." << endl;
			return false;
		}

		FresnoFF* fff = dynamic_cast<FresnoFF*>(force_field);
		fresno_types_ = &(fff->getFresnoTypes());

		// clear the vector of lipophilic interactions
		rotatable_bonds_.clear();
		// clear the grid that contains the receptor
		if (grid_ != 0)
		{
			grid_->clear();
		}

		// ?????
		// if we make the system pointer const, we can't apply()...
		// const System* system = force_field->getSystem();
		System* system = force_field->getSystem();

		// quadratic run time. not nice.

		// see above
		receptor_ = &*system->beginProtein();
		const Molecule* ligand = system->getMolecule(0);
		if (ligand == receptor_) ligand = system->getMolecule(1);
		
		BoundingBoxProcessor bb_proc;
		receptor_->apply(bb_proc);

    Options& options = force_field->options;

		factor_
			= options.setDefaultReal(FresnoFF::Option::ROT,
					FresnoFF::Default::ROT);
		grid_spacing_ 
			= options.setDefaultReal(FresnoFF::Option::ROT_GRID_SPACING,
					FresnoFF::Default::ROT_GRID_SPACING);
		bind_distance_offset_
			= options.setDefaultReal(FresnoFF::Option::ROT_BIND_OFFSET,
					FresnoFF::Default::ROT_BIND_OFFSET);

		// create a grid for the receptor and insert all its atoms
		grid_ = new HashGrid3<const Atom*>
			(bb_proc.getLower(), bb_proc.getUpper(), grid_spacing_);
		AtomConstIterator atom_it = receptor_->beginAtom();
		for (; +atom_it; ++atom_it)
		{
			grid_->insert(atom_it->getPosition(), &*atom_it);
		}

		// define and initialize all variables we need for extracting the
		// cycles (or rings)
		HashSet<const Atom*> visited;
		HashSet<const Bond*> tree;
		Stack<const Bond*> possible_cycle_bonds;
		HashSet<const Bond*> cycle_bonds;
		int cycle_count = 0;

		const Atom* atom1 = &*ligand->beginAtom();
		const Atom* atom2;

		// initialize the hash map

		cycleDFS_(atom1, visited,
				tree, possible_cycle_bonds, cycle_bonds, cycle_count);

		cout << tree.size() << endl;
		// initialize the data structures for another dfs and count the heavy
		// atoms in the system 
		atom_it = ligand->beginAtom();
		Size n_heavy_atoms = 0;
		for (; +atom_it; ++atom_it)
		{
			if ((atom_it->getElement().getSymbol() != "H")
				&& ((*fresno_types_)[&*atom_it] != FresnoFF::LIPOPHILIC))
			{
				n_heavy_atoms++;
			}
		}

		int heavy_atom_count = 0;
		HashSet<const Bond*>::ConstIterator tree_it = tree.begin();

		Sp2HybridizedPredicate isSp2;
		Sp3HybridizedPredicate isSp3;
		ConnectedToPredicate hasCH3Terminal;
		hasCH3Terminal.setArgument("(C(H)(H)(H))");
		ConnectedToPredicate hasCF3Terminal;
		hasCF3Terminal.setArgument("(C(F)(F)(F))");
		ConnectedToPredicate hasNH3Terminal;
		hasNH3Terminal.setArgument("(N(H)(H)(H))");
		ConnectedToPredicate hasNH2Terminal;
		hasNH2Terminal.setArgument("(N(H)(H))");

		bool A_sp2;
		bool A_sp3;
		bool B_sp2;
		bool B_sp3;

		// HashSet<const Atom*> tmp;

		// a rotatable bond is *any* bond between sp2-sp3 or sp3-sp3 hybridized
		// atoms that are not in rings and not bonds to terminal NH3 or CH3

		// for every non-cylce bond in the tree find out whether it is
		// rotational and count the number of heavy atoms on either side of the
		// bond
		for (; +tree_it; ++tree_it)
		{
			atom1 = (*tree_it)->getFirstAtom();
			atom2 = (*tree_it)->getSecondAtom();
			if (!cycle_bonds.has(*tree_it))
			{
				// ?????
				// make this more efficient
				A_sp2 = isSp2(*atom1);
				A_sp3 = isSp3(*atom1);
				B_sp2 = isSp2(*atom2);
				B_sp3 = isSp3(*atom2);

				if (((A_sp2 & B_sp3) | (B_sp2 & A_sp3) | (A_sp3 & B_sp3)) == true)
				{
					if (!(hasCH3Terminal(*atom1)
								| hasCF3Terminal(*atom1)
								| hasNH3Terminal(*atom1)
								| hasNH2Terminal(*atom1)
								| hasCH3Terminal(*atom2)
								| hasCF3Terminal(*atom2)
								| hasNH3Terminal(*atom2)
								| hasNH2Terminal(*atom2)))
					{
						// DEBUG
						cout << "found possible rotatable bond: " 
							<< atom1->getFullName() << "---" << atom2->getFullName()
							<< endl;
						// /DEBUG
						rotatable_bonds_.push_back(*tree_it);
						visited.clear();
						heavy_atom_count = 0;
						// tmp.clear();
						heavyAtomsDFS_(atom1, &**tree_it, visited,
								heavy_atom_count); // , tmp);
						double first_fraction = (double) heavy_atom_count / (double) n_heavy_atoms;
						double second_fraction = 1.0 - first_fraction;
						heavy_atom_fractions_.push_back
							(pair<double, double>(first_fraction, second_fraction));

						cout << heavy_atom_count << " "
							<< first_fraction << " "
							<< atom1->getFullName() << ":" << atom2->getFullName() << " " 
							<< n_heavy_atoms - heavy_atom_count 
							<< " " << second_fraction << endl;
					}
				}
			}
		}

		// N_rot_ = rotatable_bonds_.size();
		is_frozen_.resize(rotatable_bonds_.size());

		// DEBUG
		cout << "FresnoRotation setup statistics:" << endl;
		cout << "Found " << rotatable_bonds_.size() 
			<< " rotatable bonds" << endl << endl;
		// /DEBUG

		return true;

	}


	double FresnoRotation::updateEnergy()
		throw()
	{

		double E = 0.0;
		double val;

		updateFrozenBonds_();

		for (Size i = 0; i < rotatable_bonds_.size(); ++i)
		{
			if (is_frozen_[i] == true)
			{
				val = heavy_atom_fractions_[i].first + heavy_atom_fractions_[i].second;
				val /= 2.0;
				cout << "ROT: adding score of " << val << endl;
				E += val;
			}
		}

		E *= (1 - 1/N_rot_);
		E += 1.0;
		energy_ = E;
		return E;

	}


	void FresnoRotation::updateForces()
		throw()
	{
	}

	void FresnoRotation::cycleDFS_(const Atom* atom,
			HashSet<const Atom*>& visited,
			HashSet<const Bond*>& tree,
			Stack<const Bond*>& possible_cycle_bonds,
			HashSet<const Bond*>& cycle_bonds,
			int& cycle_count)
		throw()
	{
		// mark this atom as visited
		visited.insert(atom);

		// iterate over all bonds of this atom (i. e. this node)
		AtomBondConstIterator it = atom->beginBond();
		for (; +it; ++it)
		{
			const Bond* bond = &*it;
			const Atom* partner = it->getPartner(*atom);

			// if our partner atom wasn't visited yet, start dfs() with it
			if (visited.has(partner) == false)
			{

				// save this bond as a candidate for a cycle bond
				possible_cycle_bonds.push(bond);

				// save this bond in the tree we are building
				tree.insert(bond);

				// apply dfs() to the partner atom
				cycleDFS_(partner, visited, tree,
						possible_cycle_bonds, cycle_bonds, cycle_count);

				// if the current bond wasn't pop()'d by the cycle recognizing
				// heuristic, remove it from the stack. We won't need it anymore
				if (possible_cycle_bonds.top() == bond) 
				{
					possible_cycle_bonds.pop();
				}
			}
			// if we already visited this atom and our bond is not in the tree, we
			// found a cycle. Let's look which bonds actually build the cycle.
			else
			{
				if (!tree.has(bond) && !cycle_bonds.has(bond))
				{
					cycle_count++;
					cycle_bonds.insert(bond);

					const Bond* tmp;
					bool keep_on_searching = true;

					do
					{
						// PARANOIA
						if (possible_cycle_bonds.isEmpty())
						{
							cerr << "Something's wrong here, no possible bonds left" 
								<< endl;
						}
						else
						{
						// /PARANOIA

							tmp = possible_cycle_bonds.top();

							// PARANOIA
							if (tmp == 0)
							{
								cerr << "ERROR: got a NULL ptr." << endl;
								// ?????
								// exception
							}
							// /PARANOIA

							// if we found a bond that itself is part of a cycle, we found
							// two cycles that are sharing a bond. We needn't look further,
							if (cycle_bonds.has(tmp))
							{
								/*
								// PARANOIA
								HashSet<const Bond*>::ConstIterator set_it = cycle_bonds.begin();
								for (; +set_it; ++set_it)
								{
									if ((*set_it)->getFirstAtom() == partner)
									{
										if (((*set_it)->getSecondAtom() == tmp->getFirstAtom())
											|| ((*set_it)->getSecondAtom() == tmp->getSecondAtom()))
										{
											cout << "found missing CYCLE bond: "
												<< (*set_it)->getFirstAtom()->getFullName() << ":" 
												<< (*set_it)->getSecondAtom()->getFullName() << endl;
										}
									}
									if ((*set_it)->getSecondAtom() == partner)
									{
										if (((*set_it)->getFirstAtom() == tmp->getFirstAtom())
											|| ((*set_it)->getFirstAtom() == tmp->getSecondAtom()))
										{
											cout << "found missing CYCLE bond: "
												<< (*set_it)->getFirstAtom()->getFullName() << ":" 
												<< (*set_it)->getSecondAtom()->getFullName() << endl;
										}
									}
								}
								// /PARANOIA
								*/
								keep_on_searching = false;
							}
							else
							{
								possible_cycle_bonds.pop();
								cycle_bonds.insert(tmp);
							}
						}
					}
					while ((tmp->getFirstAtom() != partner) 
							&& (tmp->getSecondAtom() != partner)
							&& (!possible_cycle_bonds.isEmpty())
							&& (keep_on_searching == true));

					possible_cycle_bonds.push(bond);
				}
			}
		}
	}


	void FresnoRotation::heavyAtomsDFS_(const Atom* atom, const Bond* bond,
			HashSet<const Atom*>& visited,
			int& heavy_atom_count)
		throw()
	{

		// mark this atom as visited
		visited.insert(atom);

		// if this is a heavy atom, count it
		if ((atom->getElement().getSymbol() != "H") 
				&& ((*fresno_types_)[atom] != FresnoFF::LIPOPHILIC))
		{
			heavy_atom_count++;
		}

		// now iterate over all bonds of this atom (i. e. this node)
		AtomBondConstIterator it = atom->beginBond();
		for (; +it; ++it)
		{
			if (&*it != bond)
			{
				const Atom* partner = it->getPartner(*atom);

				// if our partner atom wasn't visited yet, start dfs() with it
				if (visited.has(partner) == 0)
				{
					// apply dfs() to the partner atom
					heavyAtomsDFS_(partner, bond, visited,
							heavy_atom_count); // , atoms_on_this_side);
				}
			}
		}
	}	


	void FresnoRotation::updateFrozenBonds_()
		throw()
	{
		HashSet<const Atom*> visited;

		N_rot_ = 0;

		for (Size i = 0; i < rotatable_bonds_.size(); ++i)
		{
			const Bond* bond = rotatable_bonds_[i];
			bool result = (frozenBondsDFS_(bond->getFirstAtom(), visited)
					&& frozenBondsDFS_(bond->getSecondAtom(), visited));
			if (result == true)
			{
				is_frozen_[i] = true;
				N_rot_++;
			}
			else
			{
				is_frozen_[i] = false;
			}
		}
	}


	bool FresnoRotation::frozenBondsDFS_(const Atom* atom, 
			HashSet<const Atom*>& visited)
		throw()
	{

		// mark this atom as visited
		visited.insert(atom);

		// if this is atom is bound, then return true
		Vector3 position = atom->getPosition();
		float dist, bind_distance;

		const HashGridBox3<const Atom*>* box = grid_->getBox(position);
		HashGridBox3<const Atom*>::ConstBoxIterator box_it;
		HashGridBox3<const Atom*>::ConstDataIterator data_it;

		if (box == 0)
		{
			cerr << "got an empty box!" << endl;
		}
		else
		{
			for (box_it = box->beginBox(); +box_it; ++box_it)
			{
				for (data_it = box_it->beginData(); +data_it; ++data_it)
				{
					if ((*data_it)->getMolecule() == receptor_)
					{
						// Eldridge et al. says any two *heavy* atoms
						if ((*data_it)->getElement().getSymbol() != "H")
						{
							dist = (position - (*data_it)->getPosition()).getLength();
							bind_distance = atom->getRadius() + (*data_it)->getRadius() 
								+ bind_distance_offset_;
							if (dist < bind_distance)
							{
								// DEBUG
								cout << "Found bound atom " << atom->getFullName() 
									<< ". dist: " << dist << " bind_distance: "
									<< bind_distance << endl;
								// /DEBUG
								return true;
							}
						}
					}
				}
			}
		}

		// now iterate over all bonds of this atom (i. e. this node)
		AtomBondConstIterator it = atom->beginBond();
		for (; +it; ++it)
		{
			const Atom* partner = it->getPartner(*atom);

			// if our partner atom wasn't visited yet, start dfs() with it
			if (visited.has(partner) == false)
			{
				// apply dfs() to the partner atom
				bool result = frozenBondsDFS_(partner, visited);
				if (result == true)
				{
					return true;
				}
			}
		}
		return false;
	}	

} // namespace BALL
