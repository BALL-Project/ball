// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/residueChecker.h>

#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/DATATYPE/hashSet.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/KERNEL/residue.h>

using namespace std;

namespace BALL
{

	ResidueChecker::ResidueChecker()
		:	fragment_db_(0),
			status_(true),
			tests_(ResidueChecker::NUMBER_OF_TESTS),
			selection_(false)
	{
		// Enable all tests by default except for overlapping atoms.
		tests_.fill();
		disable(OVERLAPPING_ATOMS);
	}
	
	ResidueChecker::ResidueChecker(FragmentDB& fragment_db)
		:	UnaryProcessor<Residue>(),
			fragment_db_(&fragment_db),
			status_(true),
			tests_(ResidueChecker::NUMBER_OF_TESTS),
			selection_(false)
	{
		// Enable all tests by default except for overlapping atoms.
		tests_.fill();
		disable(OVERLAPPING_ATOMS);
	}

	
	ResidueChecker::ResidueChecker(const ResidueChecker& residue_checker)
		:	UnaryProcessor<Residue>(),
			fragment_db_(residue_checker.fragment_db_),
			status_(residue_checker.status_),
			tests_(residue_checker.tests_),
			selection_(residue_checker.selection_)
	{
	}

	bool ResidueChecker::isEnabled(ResidueChecker::TestType t) const 
	{
		return tests_.getBit((Position)t);
	}

	void ResidueChecker::enable(ResidueChecker::TestType t, bool enable)
	{
		tests_.setBit((Position)t, enable);
	}

	void ResidueChecker::disable(ResidueChecker::TestType t)
	{
		tests_.setBit((Position)t, false);
	}
	
	ResidueChecker::~ResidueChecker()
	{
	}

	bool ResidueChecker::getStatus() const
	{
		return status_;
	}
	
	bool ResidueChecker::start()
	{
	 	status_ = true;
		return true;
	}
	
	bool ResidueChecker::finish()
	{
		return true;
	}

	Processor::Result ResidueChecker::operator () (Residue& residue)
	{
		String res_name;
		if ((residue.getChain() != 0) && (residue.getChain()->getName() != BALL_CHAIN_DEFAULT_NAME))
		{
			res_name = residue.getChain()->getName() + ":";
		}
		res_name += residue.getName() + ":" + residue.getID();

		// Check charges.
		status_ &= checkCharge(residue, res_name);

		// Check atom positions.
		status_ &= checkAtomPositions(residue, res_name);

		// if a fragment data base is defined, check for completeness
		// of the residue
		if (fragment_db_ != 0)
		{
			const Residue* reference = dynamic_cast<const Residue*>(fragment_db_->getReferenceFragment(residue));
			if (reference == 0)
			{
				if (isEnabled(UNKNOWN_RESIDUES))
				{
					Log.warn() << "ResidueChecker: didn't find a reference fragment for " << res_name << endl;
					status_ = false;

					// If selection is enabled, mark the whol residue
					if (selection_)
					{
						residue.select();
					}
				}
			} 
			else 
			{
				status_ &= checkCompleteness(residue, *reference, res_name);
				status_ &= checkTemplate(residue, *reference, res_name);
			}
		}

		return Processor::CONTINUE;
	}

	bool ResidueChecker::checkCharge(const Residue& residue, const String& res_name)
	{
		// Make sure we are suppose to do this.
		if (!isEnabled(LARGE_NET_CHARGE) && !isEnabled(LARGE_CHARGES) && !isEnabled(NON_INTEGRAL_NET_CHARGE))
		{
			return true;
		}

		bool result = true;

		// checking charge: charge should be integral and -2 <= charge <= 2
		float total_charge = 0.0;
		AtomConstIterator atom_it = residue.beginAtom();
		for (; +atom_it; ++atom_it)
		{
			total_charge += atom_it->getCharge();
			// warn for too large charges (above +/- 4 e0)
			if (isEnabled(LARGE_CHARGES) && (fabs(atom_it->getCharge()) > 4.0))
			{
				Log.warn() << "ResidueChecker: suspect charge of " << atom_it->getCharge()
					<< " for " << atom_it->getName() << " in " << res_name << std::endl;
				result = false;
				// If selection is enabled, mark the atom
				if (selection_)
				{
					const_cast<Atom&>(*atom_it).select();
				}
			}
		}

		// check for very large absolute charges
		if (isEnabled(LARGE_NET_CHARGE) && (total_charge < -2.0))
		{
			Log.warn() << "ResidueChecker: in residue " << res_name << ": total charge of " 
								 << total_charge << " is too negative." << endl;
			result = false;
			// If selection is enabled, mark the residue
			if (selection_)
			{
				const_cast<Residue&>(residue).select();
			}
		}

		if (isEnabled(LARGE_NET_CHARGE) && (total_charge > 2.0))
		{
			Log.warn() << "ResidueChecker: in residue " << res_name << ": total charge of " 
								 << total_charge << " is too positive." << endl;
			result = false;
			// If selection is enabled, mark the residue
			if (selection_)
			{
				const_cast<Residue&>(residue).select();
			}
		}

		// check for integrality of charges
		float tmp = fabs(fabs(total_charge) - (float)((int)(fabs(total_charge) + 0.5)));
		if (isEnabled(NON_INTEGRAL_NET_CHARGE) && (tmp > 0.05))
		{
			Log.warn() << "ResidueChecker: in residue " << res_name << ": residue total charge of " 
								 << total_charge << " is not integral." << endl;
			result = false;
			// If selection is enabled, mark the residue
			if (selection_)
			{
				const_cast<Residue&>(residue).select();
			}
		}
	
		return result;
	}

	bool ResidueChecker::checkCompleteness
		(const Residue& residue, const Residue& reference, const String& res_name)
	{
		// Make sure we are suppose to do this.
		if (!isEnabled(EXTRA_ATOMS) && !isEnabled(MISSING_ATOMS))
		{
			return true;
		}

		bool result = true;

		// First, check for completeness
		HashSet<String> reference_names;
		AtomConstIterator atom_it;
		for (atom_it = reference.beginAtom(); +atom_it; ++atom_it)
		{
			reference_names.insert(atom_it->getName());
		}
		
		// Check for extra atoms in the residue.
		for (atom_it = residue.beginAtom(); +atom_it; ++atom_it)
		{
			if (reference_names.has(atom_it->getName()))
			{
				reference_names.erase(atom_it->getName());
			} 
			else if (isEnabled(EXTRA_ATOMS))
			{
				Log.warn() << "ResidueChecker: did not find atom " << atom_it->getName() << " of " 
									 << res_name  << " in the reference residue " << reference.getName() << endl;
				result = false;

				// If selection is enabled, mark the residue
				if (selection_)
				{
					const_cast<Residue&>(residue).select();
				}
			}
		}

		// Check for missing atoms in the residue.
		if (isEnabled(MISSING_ATOMS) && (reference_names.size() > 0))
		{
			Log.warn() << "ResidueChecker: did not find the following atoms in " << res_name << " : ";
			HashSet<String>::Iterator set_it = reference_names.begin();
			for (; set_it != reference_names.end(); ++set_it)
			{
				Log.warn() << *set_it << " ";
			}
			Log.warn() << " (template was " << reference.getName() << ")" << endl;
			result = false;

			// If selection is enabled, mark the residue
			if (selection_)
			{
				const_cast<Residue&>(residue).select();
			}
		}

		return result;
	}

	bool ResidueChecker::checkAtomPositions(const Residue& res, const String& res_name)
	{
		// Make sure we are suppose to do this.
		if (!isEnabled(NAN_POSITIONS) && !isEnabled(STRONGLY_OVERLAPPING_ATOMS) 
				&& !isEnabled(OVERLAPPING_ATOMS) && !isEnabled(DUPLICATE_ATOM_NAMES))
		{
			return true;
		}

		bool result = true;

		AtomConstIterator atom_it;
		for (atom_it = res.beginAtom(); +atom_it; ++atom_it)
		{
			// Check for illegal atom positions (NaNs in any of the coordinates).
			if (isEnabled(NAN_POSITIONS) 
					&& (Maths::isNan(atom_it->getPosition().x)
					|| Maths::isNan(atom_it->getPosition().y)
					|| Maths::isNan(atom_it->getPosition().z)))
			{
				Log.warn() << "ResidueChecker: illegal atom position (not a number) for atom "
									 << atom_it->getName() << " of " << res_name << endl;
				// Mark the atoms affected
				if (selection_)
				{
					const_cast<Atom&>(*atom_it).select();
				}
				result = false;
			}
			
			Vector3 pos(atom_it->getPosition());
			AtomConstIterator atom_it2;
			for (atom_it2 = atom_it, ++atom_it2; +atom_it2; ++atom_it2)
			{
	
				// Check for strongly overlapping atoms (closer than 0.2 Angstrom)
				if (isEnabled(STRONGLY_OVERLAPPING_ATOMS)
						&& (pos.getSquareDistance(atom_it2->getPosition()) < 0.04))
				{
					Log.warn() << "ResidueChecker: atoms far too close -- distance between " 
										 << atom_it->getName() << " and " << atom_it2->getName() 
										 << " in " << res_name << " is " << pos.getDistance(atom_it2->getPosition()) 
										 << " A." << std::endl;
					// Mark the atoms affected
					if (selection_)
					{
						const_cast<Atom&>(*atom_it).select();
						const_cast<Atom&>(*atom_it2).select();
					}
					result = false;
				}

				// Check for overlapping atoms (closer than vdW radii minus 0.5 Angstrom)
				if (isEnabled(OVERLAPPING_ATOMS) && (!atom_it->isBoundTo(*atom_it2)))
				{
					double radius1 = atom_it->getElement().getVanDerWaalsRadius();
					if (radius1 == 0.0)
					{
						radius1 = atom_it->getElement().getAtomicRadius();
					}
					double radius2 = atom_it2->getElement().getVanDerWaalsRadius();
					if (radius2 == 0.0)
					{
						radius2 = atom_it2->getElement().getAtomicRadius();
					}

					// Compute the square of the sum of the vdw radii minus 0.6 Angstrom
					// overlap.
					double min_dist = std::max(radius1 + radius2 - 0.6, 0.0);
					if (atom_it->isGeminal(*atom_it2))
					{
						min_dist = std::max(min_dist - 1.0, 0.0);
					}
					min_dist *= min_dist;


					// If the atoms are further apart, skip them.
					if (pos.getSquareDistance(atom_it2->getPosition()) > min_dist)
					{
						continue;
					}

					// Complain about the overlap and mark the two atoms.
					Log.warn() << "ResidueChecker: atoms too close -- distance between " 
										 << atom_it->getName() << " and " << atom_it2->getName() 
										 << " in " << res_name << " is " << pos.getDistance(atom_it2->getPosition()) 
										 << " A (min: " << sqrt(min_dist) << ")." << std::endl;

					// Mark the atoms affected
					if (selection_)
					{
						const_cast<Atom&>(*atom_it).select();
						const_cast<Atom&>(*atom_it2).select();
					}
					result = false;
				}

				// Check for identical names (OK, a bad place, but why not...)
				if (isEnabled(DUPLICATE_ATOM_NAMES)
						&& (atom_it->getName() == atom_it2->getName()))
				{
					Log.warn() << "ResidueChecker: duplicate atom name " << atom_it->getName()
										 << " in " << res_name << "." << std::endl;
					result = false;
					const_cast<Atom&>(*atom_it).select();
					const_cast<Atom&>(*atom_it2).select();
				}
			}
		}
										 			
		return result;
	}

	bool ResidueChecker::checkTemplate(const Residue& residue, const Residue& reference, const String& res_name)
	{
		// Make sure there's something to be done at all.
		if (!isEnabled(SUSPECT_BOND_LENGTHS) && !isEnabled(ELEMENTS))
		{
			return true;
		}

		bool result = true;

		// Check bond lengths (should be within +/- 15% of reference values).
		Atom::BondConstIterator bond_it;
		AtomConstIterator bond_atom_it;
		AtomConstIterator atom_it;
		BALL_FOREACH_BOND(reference, bond_atom_it, bond_it)
		{
			const Atom* first = 0;
			const Atom* second = 0;
			for (atom_it = residue.beginAtom(); +atom_it && (first == 0 || second == 0); ++atom_it)
			{
				if (atom_it->getName() == bond_it->getFirstAtom()->getName())
				{
					first = &*atom_it;
				}
				if (atom_it->getName() == bond_it->getSecondAtom()->getName())
				{
					second = &*atom_it;
				}
			}
			
			// if we found the bond atoms in residue, check the atom distance
			if ((first != 0) && (second != 0))
			{
				float distance = first->getPosition().getDistance(second->getPosition());
				float deviation = fabs(distance - bond_it->getLength()) / bond_it->getLength();
				if (isEnabled(SUSPECT_BOND_LENGTHS) && (deviation > 0.15))
				{
					Log.warn() << "ResidueChecker: in residue " << res_name << ": atom distance " 
										 << "between " << first->getName() << " and " << second->getName() << " suspect: " 
										 << distance << " A instead of " << bond_it->getLength() << " A" << endl;
					result = false;
					// If selection is enabled, mark the two atoms
					if (selection_)
					{
						const_cast<Atom*>(first)->select();
						const_cast<Atom*>(second)->select();
					}
				}

				// Check for the element type of each atom
				if (isEnabled(ELEMENTS)
						&& (first->getElement() != bond_it->getFirstAtom()->getElement()))
				{
					Log.warn() << "ResidueChecker: in residue " << res_name << ": atom "
										 << first->getName() << " is " 
										 << first->getElement().getSymbol() << " should be "
										 << bond_it->getFirstAtom()->getElement().getSymbol() << endl;
										 // This could be the anchor of a ResidueRectfifier...
										 // (const_cast<Atom*> (first))->setElement(bond_it->getFirstAtom()->getElement());
					result = false;
					// If selection is enabled, mark the atom
					if (selection_)
					{
						const_cast<Atom*>(first)->select();
					}
				}
				if (isEnabled(ELEMENTS)
						&& (second->getElement() != bond_it->getSecondAtom()->getElement()))
				{
					Log.warn() << "ResidueChecker: in residue " << res_name << ": atom "
										 << second->getName() << " is " 
										 << second->getElement().getSymbol() << " should be "
										 << bond_it->getSecondAtom()->getElement().getSymbol() << endl;
					result = false;
					// If selection is enabled, mark the atom
					if (selection_)
					{
						const_cast<Atom*>(second)->select();
					}
				}
			}
		}

		return result;
	}
					
} // namespace BALL
