// $Id: fresnoRotation.C,v 1.1.2.1 2002/02/14 17:02:55 anker Exp $
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
		if (receptor_grid_ != 0)
		{
			receptor_grid_->clear();
		}

		// ?????
		// if we make the system pointer const, we can't apply()...
		// const System* system = force_field->getSystem();
		System* system = force_field->getSystem();

		// quadratic run time. not nice.

		ProteinPredicate isProtein;

		// see above
		Molecule* receptor = &*system->beginProtein();
		const Molecule* ligand = system->getMolecule(0);
		if (ligand == receptor) ligand = system->getMolecule(1);
		
		BoundingBoxProcessor bb_proc;
		receptor->apply(bb_proc);

		// ?????
		// parameter geraffels
		receptor_grid_spacing_ = 5.0;

		receptor_grid_ = new HashGrid3<const Atom*>
			(bb_proc.getLower(), bb_proc.getUpper(), receptor_grid_spacing_);
		AtomConstIterator atom_it = receptor->beginAtom();
		for (; +atom_it; ++atom_it)
		{
			receptor_grid_->insert(atom_it->getPosition(), &*atom_it);
		}

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

		const Atom* atom1;
		const Atom* atom2;

		AtomBondConstIterator bond_it;

		// a rotatable bond is *any* bond between sp2-sp3 or sp3-sp3 hybridized
		// atoms that are not in rings and not bonds to terminal NH3 or CH3
		BALL_FOREACH_BOND(*ligand, atom_it, bond_it)
		{
			atom1 = bond_it->getFirstAtom();
			atom2 = bond_it->getSecondAtom();
			HashSet<const Bond*> visited;

			// ?????
			// make this more efficient
			A_sp2 = isSp2(*atom1);
			A_sp3 = isSp3(*atom1);
			B_sp2 = isSp2(*atom2);
			B_sp3 = isSp3(*atom2);
			
			if ((((A_sp2 | A_sp3) & (B_sp2 | B_sp3)) & (!(A_sp2 & B_sp3))) == true)
			{
				visited.clear();
				visited.insert(&*bond_it);
				// if the dfs finds atom2 although the bond was already inserted into
				// the visited hash set, our bond is part of a ring.
				if (!ringDFS_(*atom1, *atom2, 8, visited))
				{
					// now that we know that this bond is not part of a ring, test
					// whether bond_it binds terminal CH3, CF3, NH2 or NH3 groups to the rest
					// of the molecule
					if (!(hasCH3Terminal(*atom1)
								| hasCF3Terminal(*atom1)
								| hasNH3Terminal(*atom1)
								| hasNH2Terminal(*atom1)
								| hasCH3Terminal(*atom2)
								| hasCF3Terminal(*atom2)
								| hasNH3Terminal(*atom2)
								| hasNH2Terminal(*atom2)))
					{
						rotatable_bonds_.push_back(&*bond_it);
					}
				}
			}
		}
		N_rot_ = rotatable_bonds_.size();

		heavy_atom_fractions_.resize(N_rot_);
		
		double fraction_first;
		double fraction_second;

		const Bond* bond;

		// now calculate the fractions of heavy atoms 
		for (Size i = 0; i < N_rot_; ++i)
		{
			bond = rotatable_bonds_[i];
			fraction_first = countHeavyAtoms_(bond->getFirstAtom(), bond);
			fraction_second = countHeavyAtoms_(bond->getSecondAtom(), bond);
			double tmp = fraction_first + fraction_second;
			fraction_first /= tmp;
			fraction_second /= tmp;
			heavy_atom_fractions_[i] = pair<double, double>(fraction_first, fraction_second);
		}

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


	bool FresnoRotation::ringDFS_(const Atom& atom, const Atom& first_atom,
			const Size limit, HashSet<const Bond*>& visited) const
		throw()
	{
		// the following recursive function performs an ad-hoc dfs and returns
		// true, if a ring was found and false otherwise.

		if (limit == 0) 
		{
			if (atom == first_atom) 
			{
				// Found first atom at limit
				return true;
			}
			else
			{
				// Reached limit without finding the first atom
				return false;
			}
		}
		Size i;
		const Bond* bond;
		Atom* descend;
		HashSet<const Bond*> my_visited(visited);

		// Now iterate over all Bonds an store the visited bonds.

		for (i = 0; i < atom.countBonds(); ++i)
		{
			bond = atom.getBond(i);
			if (!my_visited.has(bond))
			{
				descend = bond->getPartner(atom);
				my_visited.insert(bond);
				if (ringDFS_(*descend, first_atom, limit-1, my_visited))
				{
					return true;
				}
			}
		}
		// No partner matched
		return false;
	}


	void FresnoRotation::findDFS_(const Atom* atom, const Bond* bond,
			HashSet<const Atom*>& visited, double& min_distance) const
		throw()
	{
		if (visited.has(atom))
		{
			return;
		}

		visited.insert(atom);

		const Vector3 position = atom->getPosition();
		const HashGridBox3<const Atom*>* box = receptor_grid_->getBox(position);
		if (box == 0)
		{
			// ?????
			// what can we do if box doesn't exist? We need a box to iterate...
		}
		else
		{
			HashGridBox3<const Atom*>::ConstBoxIterator box_it = box->beginBox();
			HashGridBox3<const Atom*>::ConstDataIterator data_it;
			double distance;
			for (; +box_it; ++box_it)
			{
				for (data_it = box_it->beginData(); +data_it; ++data_it)
				{
					distance = ((*data_it)->getPosition() - position).getLength();
					if (distance < min_distance)
					{
						min_distance = distance;
					}
				}
			}
		}
		
		const Bond* tmp;

		for (Size i = 0; i < atom->countBonds(); ++i)
		{
			tmp = atom->getBond(i);
			if (tmp != bond)
			{
				findDFS_(tmp->getPartner(*atom), bond, visited, min_distance);
			}
		}
	}


	void FresnoRotation::countDFS_(const Atom* atom, const Bond* bond,
			HashSet<const Atom*>& visited, Size& heavy_count) const
		throw()
	{
		if (visited.has(atom))
		{
			return;
		}

		visited.insert(atom);

		if (atom->getElement().getSymbol() != "H")
		{
			// only count nonlipophilic heavy atoms
			if (fresno_types_->has(atom))
			{
				if ((*fresno_types_)[atom] != FresnoFF::LIPOPHILIC)
				{
					heavy_count++;
				}
			}
			else
			{
				Log.error() << "FresnoRotation::countDFS_(): "
					<< "Trying to use untyped atom" << endl;
			}
		}

		const Bond* tmp;

		for (Size i = 0; i < atom->countBonds(); ++i)
		{
			tmp = atom->getBond(i);
			if (tmp != bond)
			{
				countDFS_(tmp->getPartner(*atom), bond, visited, heavy_count);
			}
		}
	}

	Size FresnoRotation::countHeavyAtoms_(const Atom* atom, const Bond* bond) const
		throw()
	{
		HashSet<const Atom*> visited;
		Size heavy_atoms = 0;
		countDFS_(atom, bond, visited, heavy_atoms);
		return heavy_atoms;
	}
	
}
