// $Id: fresnoRotation.C,v 1.1.2.2 2002/03/05 23:46:21 anker Exp $
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
			frozen_bonds_(),
			heavy_atom_fractions_(),
			receptor_grid_(0),
			receptor_grid_spacing_(0.0),
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
			frozen_bonds_(),
			heavy_atom_fractions_(),
			receptor_grid_(0),
			receptor_grid_spacing_(0.0),
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
			frozen_bonds_(fr.frozen_bonds_),
			heavy_atom_fractions_(fr.heavy_atom_fractions_),
			receptor_grid_(fr.receptor_grid_),
			receptor_grid_spacing_(fr.receptor_grid_spacing_),
			fresno_types_(fr.fresno_types_)
	{
	}


	FresnoRotation::~FresnoRotation()
		throw()
	{
		if (receptor_grid_ != 0)
		{
			delete receptor_grid_;
		}
		clear();
	}


	void FresnoRotation::clear()
		throw()
	{
		rotatable_bonds_.clear();
		N_rot_ = 0;
		frozen_bonds_.clear();
		heavy_atom_fractions_.clear();
		if (receptor_grid_ != 0) receptor_grid_->clear();
		receptor_grid_spacing_ = 0.0;
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
		if (receptor_grid_ != 0)
		{
			receptor_grid_->clear();
		}

		// ?????
		// if we make the system pointer const, we can't apply()...
		// const System* system = force_field->getSystem();
		System* system = force_field->getSystem();

		// quadratic run time. not nice.

		// see above
		Molecule* receptor = &*system->beginProtein();
		const Molecule* ligand = system->getMolecule(0);
		if (ligand == receptor) ligand = system->getMolecule(1);
		
		BoundingBoxProcessor bb_proc;
		receptor->apply(bb_proc);

		// ?????
		// parameter geraffels
		receptor_grid_spacing_ = 5.0;

		// create a grid for the receptor and insert all its atoms
		receptor_grid_ = new HashGrid3<const Atom*>
			(bb_proc.getLower(), bb_proc.getUpper(), receptor_grid_spacing_);
		AtomConstIterator atom_it = receptor->beginAtom();
		for (; +atom_it; ++atom_it)
		{
			receptor_grid_->insert(atom_it->getPosition(), &*atom_it);
		}

		// define and initialize all variables we need for extracting the
		// cycles (or rings)
		int dfs_count = 0;
		HashMap<const Atom*, int> dfs_num;
		HashSet<const Bond*> tree;
		Stack<const Bond*> possible_cycle_bonds;
		HashSet<const Bond*> cycle_bonds;
		int cycle_count = 0;

		const Atom* atom1 = &*ligand->beginAtom();
		const Atom* atom2;

		// initialize the hash map
		atom_it = ligand->beginAtom();
		for (; +atom_it; ++atom_it)
		{
			dfs_num.insert(pair<const Atom*, int>(&*atom_it, -1));
		}

		cycleDFS_(atom1, dfs_count, dfs_num, 
				tree, possible_cycle_bonds, cycle_bonds, cycle_count);

		cout << tree.size() << endl;
		// initialize the data structures for another dfs and count the heavy
		// atoms in the system 
		dfs_count = 0;
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
						dfs_count = 0;
						for (atom_it = ligand->beginAtom(); +atom_it; ++atom_it)
						{
							dfs_num[&*atom_it] = -1;
						}
						heavy_atom_count = 0;
						// tmp.clear();
						heavyAtomsDFS_(atom1, &**tree_it, dfs_count, dfs_num, 
								heavy_atom_count); // , tmp);
						double first_fraction = (double) heavy_atom_count / (double) n_heavy_atoms;
						double second_fraction = 1.0 - first_fraction;
						heavy_atom_fractions_.push_back
							(pair<double, double>(first_fraction, second_fraction));

						cout << dfs_num[atom1] << " " << heavy_atom_count << " "
							<< first_fraction << " "
							<< atom1->getFullName() << ":" << atom2->getFullName() << " " 
							<< dfs_num[atom2] << " " << n_heavy_atoms - heavy_atom_count 
							<< " " << second_fraction << endl;
					}
				}
			}
		}

		N_rot_ = rotatable_bonds_.size();

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

		// first find out which rotatable bonds are frozen
		for (Size i = 0; i < rotatable_bonds_.size(); ++i)
		{
			// ?????
			// we have to perform dfs in order to find those atoms that bind to the
			// receptor.
		}

		return E;

	}


	void FresnoRotation::updateForces()
		throw()
	{
	}

	void FresnoRotation::cycleDFS_(const Atom* atom,
			int& dfs_count, HashMap<const Atom*, int>& dfs_num, 
			HashSet<const Bond*>& tree,
			Stack<const Bond*>& possible_cycle_bonds,
			HashSet<const Bond*>& cycle_bonds,
			int& cycle_count)
		throw()
	{

		// assign this node's dfs number and increase the count
		dfs_num[atom] = dfs_count++;

		// now iterate over all bonds of this atom (i. e. this node)
		AtomBondConstIterator it = atom->beginBond();
		for (; +it; ++it)
		{
			const Bond* bond = &*it;
			const Atom* partner = it->getPartner(*atom);

			// if our partner atom wasn't visited yet, start dfs() with it
			if (dfs_num[partner] == -1)
			{

				// save this bond as a candidate for a cycle bond
				possible_cycle_bonds.push(bond);

				// save this bond in the tree we are building
				tree.insert(bond);

				// apply dfs() to the partner atom
				cycleDFS_(partner, dfs_count, dfs_num, tree,
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
			int& dfs_count,
			HashMap<const Atom*, int>& dfs_num,
			int& heavy_atom_count)
//			HashSet<const Atom*>& atoms_on_this_side)
		throw()
	{

		// assign this node's dfs number and increase the count
		dfs_num[atom] = dfs_count++;

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
				if (dfs_num[partner] == -1)
				{
					// apply dfs() to the partner atom
					heavyAtomsDFS_(partner, bond, dfs_count, dfs_num,
							heavy_atom_count); // , atoms_on_this_side);
				}
			}
		}
	}	

} // namespace BALL
