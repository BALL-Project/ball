// $Id: fresnoRotation.C,v 1.1.2.12 2002/11/22 15:12:40 anker Exp $
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

#include <BALL/DATATYPE/stringHashMap.h>

using namespace std;

namespace BALL
{

	FresnoRotation::FresnoRotation()
		throw()
		:	ForceFieldComponent(),
			rotatable_bonds_(),
			N_rot_(0),
			is_frozen_(),
			algorithm_type_(0),
			heavy_atom_fractions_(),
			grid_(0),
			grid_spacing_(0.0),
			fresno_types_(0)
	{
		// set component name
		setName("Fresno RotationalEntropyLoss");
	}


	FresnoRotation::FresnoRotation(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field),
			rotatable_bonds_(),
			N_rot_(0),
			is_frozen_(),
			algorithm_type_(0),
			heavy_atom_fractions_(),
			grid_(0),
			grid_spacing_(0.0),
			fresno_types_(0)
	{
		// set component name
		setName("Fresno RotationalEntropyLoss");
	}


	FresnoRotation::FresnoRotation(const FresnoRotation& fr, bool deep)
		throw()
		:	ForceFieldComponent(fr, deep),
			rotatable_bonds_(fr.rotatable_bonds_),
			N_rot_(fr.N_rot_),
			is_frozen_(fr.is_frozen_),
			algorithm_type_(fr.algorithm_type_),
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
		algorithm_type_ = 0;
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
		system->apply(bb_proc);

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
		algorithm_type_
			= options.setDefaultInteger(FresnoFF::Option::ROT_ALGORITHM,
					FresnoFF::Default::ROT_ALGORITHM);
		Size verbosity
			= options.setDefaultInteger(FresnoFF::Option::VERBOSITY,
					FresnoFF::Default::VERBOSITY);

		// create a grid for the receptor and insert all its atoms
		// this grid is needed to find out whether an atom of the ligand is
		// bound to the protein
		grid_ = new HashGrid3<const Atom*>
			(bb_proc.getLower() - Vector3(1.0),
			 bb_proc.getUpper() - bb_proc.getLower() + Vector3(1.0),
			 grid_spacing_);

		// HashGridBox3<const Atom*>* box;
		AtomConstIterator atom_it = system->beginAtom();
		for (; +atom_it; ++atom_it)
		{
			grid_->insert(atom_it->getPosition(), &*atom_it);
		}


		// define the bondlengths for the bond guessing algorithm

		StringHashMap< pair<float, float> > bondlengths;

		// ?????
		// This is not nice and should be done using an INIFile
		pair<float, float> tmp;

		// we need shorter bondlenghts for C-C and C-N
		// tmp = pair<float, float>(1.54, 1.55);
		tmp = pair<float, float>(1.52, 1.55);
		bondlengths["C"] = tmp;
		// tmp = pair<float, float>(1.47, 1.48);
		tmp = pair<float, float>(1.44, 1.48);
		bondlengths["N"] = tmp;
		tmp = pair<float, float>(1.43, 1.47);
		bondlengths["O"] = tmp;
		// S is not taken into account by the original paper.
		// tmp = pair<float, float>(1.80, 1.84);
		// bondlengths["S"] = tmp;

		Size guessed_bonds = 0;

		String sym1, sym2;
		StringHashMap< pair<float, float> >::ConstIterator hash_it;

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

		if (verbosity >= 90) { Log.info() << "Tree size: " << tree.size() << endl; }

		// initialize the data structures for another dfs and count the heavy
		// atoms in the system 
		int heavy_atom_count = 0;
		int nonlip_heavy_atom_count = 0;
		HashSet<const Bond*>::ConstIterator tree_it = tree.begin();

		Sp2HybridizedPredicate isSp2;
		Sp3HybridizedPredicate isSp3;
		ConnectedToPredicate hasH3Group;
		hasH3Group.setArgument("(H)(H)(H)");
		ConnectedToPredicate hasH2Group;
		hasH2Group.setArgument("(H)(H)");
		ConnectedToPredicate hasF3Group;
		hasF3Group.setArgument("(F)(F)(F)");

		ElementPredicate isCarbon;
		isCarbon.setArgument("C");
		ElementPredicate isNitrogen;
		isNitrogen.setArgument("N");
		ConnectedToPredicate hasAromaticBondedOxygen;
		hasAromaticBondedOxygen.setArgument("(~O)");

		bool A_sp2;
		bool A_sp3;
		bool B_sp2;
		bool B_sp3;
		bool A_CO;
		bool B_CO;

		bool found_rotatable_bond = false;

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

				if (verbosity >= 100)
				{
					Log.info() << atom1->getFullName() << "---" << atom2->getFullName()
						<< endl;
					Log.info() << "hasH3Group(*atom1) " << hasH3Group(*atom1) << endl;
					Log.info() << "hasH2Group(*atom1) " << hasH2Group(*atom1) << endl;
					Log.info() << "hasF3Group(*atom1) " << hasF3Group(*atom1) << endl;
					Log.info() << "isCarbon(*atom1) " << isCarbon(*atom1) << endl;
					Log.info() << "isNitrogen(*atom1) " << isNitrogen(*atom1) << endl;
					Log.info() << "hasH3Group(*atom2) " << hasH3Group(*atom2) << endl;
					Log.info() << "hasH2Group(*atom2) " << hasH2Group(*atom2) << endl;
					Log.info() << "hasF3Group(*atom2) " << hasF3Group(*atom2) << endl;
					Log.info() << "isCarbon(*atom2) " << isCarbon(*atom2) << endl;
					Log.info() << "isNitrogen(*atom2) " << isNitrogen(*atom2) << endl;
				}

				if (!((hasH3Group(*atom1) & (isCarbon(*atom1) | isNitrogen(*atom1)))
							| (hasH2Group(*atom1) & isNitrogen(*atom1))
							| (hasF3Group(*atom1) & isCarbon(*atom1))
							| (hasH3Group(*atom2) & (isCarbon(*atom2) | isNitrogen(*atom2)))
							| (hasH2Group(*atom2) & isNitrogen(*atom2))
							| (hasF3Group(*atom2) & isCarbon(*atom2))))
				{

					if (algorithm_type_ == ALGORITHM__GUESS)
					{
						float dist = (atom1->getPosition() - atom2->getPosition()).getLength();
						sym1 = atom1->getElement().getSymbol();
						sym2 = atom2->getElement().getSymbol();

						if (sym1 == "C")
						{
							if (bondlengths.has(sym2))
							{
								if (verbosity >= 100)
								{
									if (sym2 == "N")
									{
										Log.info() << atom1->getFullName() << "---" 
											<< atom2->getFullName() << endl;
										Log.info() << "length: " << dist << endl;
									}
								}
								float lower = bondlengths[sym2].first;
								float upper = bondlengths[sym2].second;
								if ((dist > (lower * 0.975)) && (dist <= (upper * 1.025)))
								{
									if (verbosity >= 90)
									{
										Log.info() << "found rotatable bond:" << endl;
										Log.info() << atom1->getFullName() << "---" 
											<< atom2->getFullName() << endl;
										Log.info() << "length: " << dist << endl;
									}
									found_rotatable_bond = true;
									guessed_bonds++;
								}
							}
						}
						else
						{
							if (sym2 == "C")
							{
								if (bondlengths.has(sym1))
								{
									// DEBUG
									if (verbosity >= 100)
									{
										if (sym2 == "N")
										{
											Log.info() << atom1->getFullName() << "---" 
												<< atom2->getFullName() << endl;
											Log.info() << "length: " << dist << endl;
										}
									}
									// /DEBUG
									float lower = bondlengths[sym1].first;
									float upper = bondlengths[sym1].second;
									if ((dist > (lower * 0.975)) && (dist <= (upper * 1.025)))
									{
										if (verbosity >= 90)
										{
											Log.info() << "found rotatable bond:" << endl;
											Log.info() << atom1->getFullName() << "---" 
												<< atom2->getFullName() << endl;
											Log.info() << "length: " << dist << endl;
										}
										found_rotatable_bond = true;
										guessed_bonds++;
									}
								}
							}
						}
					}
					else
					{
						if (algorithm_type_ == ALGORITHM__DATABASE)
						{
							// ?????
							// make this more efficient
							A_sp2 = isSp2(*atom1);
							A_sp3 = isSp3(*atom1);
							B_sp2 = isSp2(*atom2);
							B_sp3 = isSp3(*atom2);
							A_CO = (isCarbon(*atom1) & hasAromaticBondedOxygen(*atom1));
							B_CO = (isCarbon(*atom2) & hasAromaticBondedOxygen(*atom2));

							// DEBUG
							if (verbosity >= 100)
							{
								Log.info() << "A SP2: " << A_sp2 << endl;
								Log.info() << "A SP3: " << A_sp3 << endl;
								Log.info() << "B SP2: " << B_sp2 << endl;
								Log.info() << "B SP3: " << B_sp3 << endl;
								Log.info() << "A CO: " << A_CO << endl;
								Log.info() << "B CO: " << B_CO << endl;
							}
							// /DEBUG

							if (((A_sp2 & B_sp3) | (B_sp2 & A_sp3) | (A_sp3 & B_sp3))
									| ((A_CO & B_sp3 & isCarbon(*atom2)) 
										| (A_sp3 & B_CO & isCarbon(*atom1))) == true)
							{
								// DEBUG
								if (verbosity >= 100)
								{
									Log.info() << "found possible rotatable bond: " 
										<< atom1->getFullName() << "---" << atom2->getFullName()
										<< endl;
								}
								// /DEBUG
								found_rotatable_bond = true;
							}
						}
						else
						{
							Log.error() << "Unknown algorithm type" << endl;
							return false;
						}
					}
				}
			}
			if (found_rotatable_bond == true)
			{
				found_rotatable_bond = false;
				rotatable_bonds_.push_back(*tree_it);
				visited.clear();
				heavy_atom_count = 0;
				nonlip_heavy_atom_count = 0;
				heavyAtomsDFS_(atom1, &**tree_it, visited,
						heavy_atom_count, nonlip_heavy_atom_count);
				double first_fraction = (double) nonlip_heavy_atom_count / (double) heavy_atom_count;

				if (verbosity >= 90)
				{
					Log.info() << heavy_atom_count << " "
						<< first_fraction << " "
						<< atom1->getFullName() << ":" << atom2->getFullName() << " " ;
				}

				visited.clear();
				heavy_atom_count = 0;
				nonlip_heavy_atom_count = 0;
				heavyAtomsDFS_(atom2, &**tree_it, visited,
						heavy_atom_count, nonlip_heavy_atom_count);
				double second_fraction = (double) nonlip_heavy_atom_count / (double) heavy_atom_count;

				heavy_atom_fractions_.push_back
					(pair<double, double>(first_fraction, second_fraction));

				if (verbosity >= 90)
				{
					Log.info() << heavy_atom_count 
						<< " " << second_fraction << endl;
				}
			}
		}

		N_rot_ = rotatable_bonds_.size();
		is_frozen_.resize(rotatable_bonds_.size());

		if (verbosity >= 9)
		{
			Log.info() << "FresnoRotation setup statistics:" << endl;
			Log.info() << "Found " << rotatable_bonds_.size() 
				<< " rotatable bonds" << endl << endl;
		}

		return true;

	}


	double FresnoRotation::updateEnergy()
		throw()
	{

		Size verbosity
			= getForceField()->options.setDefaultInteger(FresnoFF::Option::VERBOSITY,
					FresnoFF::Default::VERBOSITY);

		if (N_rot_ == 0)
		{
			energy_ = 0.0;
			return energy_;
		}
		else
		{

			energy_ = 0.0;
			float val;

			updateFrozenBonds_();

			for (Size i = 0; i < rotatable_bonds_.size(); ++i)
			{
				if (is_frozen_[i] == true)
				{
					val = heavy_atom_fractions_[i].first + heavy_atom_fractions_[i].second;
					val /= 2.0;
					if (verbosity >= 90)
					{
						Log.info() << "ROT: adding score of " << val << endl;
					}
					energy_ += val;
				}
			}

			energy_ *= (1 - 1/N_rot_);
			energy_ += 1.0;
			energy_ = factor_ * energy_;

			if (verbosity > 0)
			{
				Log.info() << "ROT: energy is " << energy_ << endl;
			}

			return energy_;
		}

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
											Log.info() << "found missing CYCLE bond: "
												<< (*set_it)->getFirstAtom()->getFullName() << ":" 
												<< (*set_it)->getSecondAtom()->getFullName() << endl;
										}
									}
									if ((*set_it)->getSecondAtom() == partner)
									{
										if (((*set_it)->getFirstAtom() == tmp->getFirstAtom())
											|| ((*set_it)->getFirstAtom() == tmp->getSecondAtom()))
										{
											Log.info() << "found missing CYCLE bond: "
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
			int& heavy_atom_count, int& nonlip_heavy_atom_count)
		throw()
	{

		// mark this atom as visited
		visited.insert(atom);

		// if this is a heavy atom, count it
		if (atom->getElement().getSymbol() != "H") 
		{
			heavy_atom_count++;

			if ((*fresno_types_)[atom] != FresnoFF::LIPOPHILIC)
			{
				nonlip_heavy_atom_count++;
			}
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
							heavy_atom_count, nonlip_heavy_atom_count); 
				}
			}
		}
	}	


	void FresnoRotation::updateFrozenBonds_()
		throw()
	{
		HashSet<const Atom*> visited;

		for (Size i = 0; i < rotatable_bonds_.size(); ++i)
		{
			const Bond* bond = rotatable_bonds_[i];
			bool result = (frozenBondsDFS_(bond->getFirstAtom(), visited)
					&& frozenBondsDFS_(bond->getSecondAtom(), visited));
			if (result == true)
			{
				is_frozen_[i] = true;
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

		HashGridBox3<const Atom*>* box = grid_->getBox(position);
		HashGridBox3<const Atom*>::ConstBoxIterator box_it;
		HashGridBox3<const Atom*>::ConstDataIterator data_it;

		if (box == 0)
		{
			Log.error() << "FresnoRotation::updateFrozenBonds_(): "
				<< "got an empty box for position " << position << endl;
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
								Log.info() << "Found bound atom " << atom->getFullName() 
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
