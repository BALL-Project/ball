// $Id: residueChecker.C,v 1.18 2001/07/31 00:47:52 oliver Exp $

#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/DATATYPE/hashSet.h>

using namespace std;

namespace BALL
{

	ResidueChecker::ResidueChecker()
		:	fragment_db_(0),
			status_(true)
	{
	}
	
	ResidueChecker::ResidueChecker(FragmentDB& fragment_db)
		:	fragment_db_(&fragment_db),
			status_(true)
	{
	}
	
	ResidueChecker::ResidueChecker(const ResidueChecker& residue_checker , bool /* deep */)
		:	UnaryProcessor<Residue>(),
			fragment_db_(residue_checker.fragment_db_),
			status_(residue_checker.status_)
	{
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

		bool result = checkCharge(residue, res_name);
		status_ &= result;

		result = checkAtomPositions(residue, res_name);
		status_ &= result;

		// if a fragment data base is defined, check for completeness
		// of the residue
		if (fragment_db_ != 0)
		{
			const Residue* reference = dynamic_cast<const Residue*>(fragment_db_->getReferenceFragment(residue));
			if (reference == 0)
			{
				Log.warn() << "ResidueChecker: didn't find a reference fragment for " << res_name << endl;
				status_ = false;	
			} 
			else 
			{
				result = checkCompleteness(residue, *reference, res_name);
				status_ &= result;

				result = checkTemplate(residue, *reference, res_name);
				status_ &= result;
			}
		}

		return Processor::CONTINUE;
	}

	bool ResidueChecker::checkCharge(const Residue& residue, const String& res_name)
		throw()
	{
		bool result = true;
		// checking charge: charge should be integral and -2 <= charge <= 2
		float total_charge = 0.0;
		AtomConstIterator atom_it = residue.beginAtom();
		for (; +atom_it; ++atom_it)
		{
			total_charge += atom_it->getCharge();
			// warn for too large charges (above +/- 4 e0)
			if (fabs(atom_it->getCharge()) > 4.0)
			{
				Log.warn() << "ResidueChecker: suspect charge of " << atom_it->getCharge()
					<< " for " << atom_it->getName() << " in " << res_name << std::endl;
				result = false;
			}
		}

		// check for very large absolute charges
		if (total_charge < -2.0)
		{
			Log.warn() << "ResidueChecker: in residue " << res_name << ": total charge of " 
								 << total_charge << " is too negative." << endl;
			result = false;
		}

		if (total_charge > 2.0)
		{
			Log.warn() << "ResidueChecker: in residue " << res_name << ": total charge of " 
								 << total_charge << " is too positive." << endl;
			result = false;
		}

		// check for integrality of charges
		float tmp = fabs(fabs(total_charge) - (float)((int)(fabs(total_charge) + 0.5)));
		if (tmp > 0.05)
		{
			Log.warn() << "ResidueChecker: in residue " << res_name << ": residue total charge of " 
								 << total_charge << " is not integral." << endl;
			result = false;
		}
	
		return result;
	}

	bool ResidueChecker::checkCompleteness
		(const Residue& residue, const Residue& reference, const String& res_name)
		throw()
	{
		bool result = true;

		// first, check for completeness
		HashSet<String> reference_names;
		AtomConstIterator atom_it;
		for (atom_it = reference.beginAtom(); +atom_it; ++atom_it)
		{
			reference_names.insert(atom_it->getName());
		}
		
		for (atom_it = residue.beginAtom(); +atom_it; ++atom_it)
		{
			if (reference_names.has(atom_it->getName()))
			{
				reference_names.erase(atom_it->getName());
			} 
			else 
			{
				Log.warn() << "ResidueChecker: did not find atom " << atom_it->getName() << " of " 
									 << res_name  << " in the reference residue " << reference.getName() << endl;
				status_ = false;
			}
		}
		if (reference_names.size() > 0)
		{
			Log.warn() << "ResidueChecker: did not find the following atoms in " << res_name << " : ";
			HashSet<String>::Iterator set_it = reference_names.begin();
			for (; set_it != reference_names.end(); ++set_it)
			{
				Log.warn() << *set_it << " ";
			}
			Log.warn() << " (template was " << reference.getName() << ")" << endl;
			status_ = false;
		}

		return result;
	}

	bool ResidueChecker::checkAtomPositions(const Residue& res, const String& res_name)
		throw()
	{
		bool result = true;

		AtomConstIterator atom_it;
		for (atom_it = res.beginAtom(); +atom_it; ++atom_it)
		{
			// check for illegal atom positions (NaNs)
			if (Maths::isNan(atom_it->getPosition().x)
					||Maths::isNan(atom_it->getPosition().y)
					||Maths::isNan(atom_it->getPosition().z))
			{
				Log.warn() << "ResidueChecker: illegal atom position (not a number) for atom "
									 << atom_it->getName() << " of " << res_name << endl;
				result = false;
			}
			
			Vector3 pos(atom_it->getPosition());
			AtomConstIterator atom_it2;
			for (atom_it2 = atom_it, ++atom_it2; +atom_it2; ++atom_it2)
			{
				// check for overlapping atoms (closer than 0.5 Angstrom)
				if (pos.getSquareDistance(atom_it2->getPosition()) < 0.5)
				{
					Log.warn() << "ResidueChecker: atoms too close -- distance between " 
										 << atom_it->getName() << " and " << atom_it2->getName() 
										 << " in " << res_name << " is " << pos.getDistance(atom_it2->getPosition()) 
										 << " A." << std::endl;
					result == false;
				}
				// check for identical names (OK, a bad place, but why not...)
				if (atom_it->getName() == atom_it2->getName())
				{
					Log.warn() << "ResidueChecker: duplicate atom name " << atom_it->getName()
										 << " in " << res_name << "." << std::endl;
					result = false;
				}
			}
		}
										 			
		return result;
	}

	bool ResidueChecker::checkTemplate(const Residue& residue, const Residue& reference, const String& res_name)
		throw()
	{
		bool result = true;
		// check bond lengths (should be within +/- 15% of reference values)
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
				if (deviation > 0.15)
				{
					Log.warn() << "ResidueChecker: in residue " << res_name << ": atom distance " 
										 << "between " << first->getName() << " and " << second->getName() << " suspect: " 
										 << distance << " A instead of " << bond_it->getLength() << " A" << endl;
					result = false;
				}

				// check for the element type of each atom
				if (first->getElement() != bond_it->getFirstAtom()->getElement())
				{
					Log.warn() << "ResidueChecker: in residue " << res_name << ": atom "
										 << first->getName() << " is " 
										 << first->getElement().getSymbol() << " should be "
										 << bond_it->getFirstAtom()->getElement().getSymbol() << endl;
										 // This could be the anchor of a ResidueRectfifier...
										 // (const_cast<Atom*> (first))->setElement(bond_it->getFirstAtom()->getElement());
					result = false;
				}
				if (second->getElement() != bond_it->getSecondAtom()->getElement())
				{
					Log.warn() << "ResidueChecker: in residue " << res_name << ": atom "
										 << second->getName() << " is " 
										 << second->getElement().getSymbol() << " should be "
										 << bond_it->getSecondAtom()->getElement().getSymbol() << endl;
					result = false;
				}
			}
		}

		return result;
	}
					
} // namespace BALL
