// $Id: residueChecker.C,v 1.2 1999/09/17 13:47:09 oliver Exp $

#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/DATATYPE/hashSet.h>

namespace BALL
{

	ResidueChecker::ResidueChecker()
		:	fragment_db_(0)
	{
	}
	
	ResidueChecker::ResidueChecker(FragmentDB& fragment_db)
		:	fragment_db_(&fragment_db)
	{
	}
	
	ResidueChecker::ResidueChecker(const ResidueChecker& /* residue_checker */, bool /* deep */)
	{
		// BAUSTELLE
	}

	ResidueChecker::~ResidueChecker()
	{
	}

	bool ResidueChecker::start()
	{
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

		// checking charge: charge should be integral and -2 <= charge <= 2
		float total_charge = 0.0;
		AtomIterator atom_it = residue.beginAtom();
		for (; +atom_it; ++atom_it)
		{
			total_charge += atom_it->getCharge();
		}
		Log.info() << "ResidueChecker: charge of " << res_name << " is " << total_charge << endl;

		// check for very large absolute charges
		if (total_charge < -2.0)
		{
			Log.warn() << "ResidueChecker: in residue " << res_name << ": total charge of " << total_charge << " is too negative." << endl;
		}

		if (total_charge > 2.0)
		{
			Log.warn() << "ResidueChecker: in residue " << res_name << ": total charge of " << total_charge << " is too positive." << endl;
		}

		// check for integrality of charges
		float tmp = fabs(fabs(total_charge) - truncf(fabs(total_charge) + 0.5));
		if (tmp > 0.05)
		{
			Log.warn() << "ResidueChecker: in residue " << res_name << ": residue total charge of " << total_charge << " is not integral." << endl;
		}

		// if a fragment data base is defined, check for completeness
		// of the residue
		if (fragment_db_ != 0)
		{
			const Residue* reference = dynamic_cast<const Residue*>(fragment_db_->getReferenceFragment(residue));
			if (reference == 0)
			{
				Log.warn() << "ResidueChecker: didn't find a reference fragment for " << res_name << endl;
			} else {
				// first, check for completeness
				HashSet<String> reference_names;
				for (atom_it = reference->beginAtom(); +atom_it; ++atom_it)
				{
					reference_names.insert(atom_it->getName());
				}
				
				for (atom_it = residue.beginAtom(); +atom_it; ++atom_it)
				{
					if (reference_names.has(atom_it->getName()))
					{
						reference_names.erase(atom_it->getName());
					} else {
						Log.warn() << "ResidueChecker: did not find atom " << atom_it->getName() << " of " << res_name  << " in the reference residue " << reference->getName() << endl;
					}
				}
				if (reference_names.size() > 0)
				{
					Log.warn() << "ResidueChecker: did not find the following atoms in " << res_name << ": ";
					HashSet<String>::iterator set_it = reference_names.begin();
					for (; set_it != reference_names.end(); ++set_it)
					{
						Log.warn() << *set_it << " ";
					}
					Log.warn() << endl;
				}

				// check bond lengths (should be within +/- 15% of reference values)
				Atom::BondIterator bond_it;
				AtomIterator bond_atom_it;
				Residue res(*reference);
				BALL_FOREACH_BOND(res, bond_atom_it, bond_it)
				{
					Atom* first = 0;
					Atom* second = 0;
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
					
					// if we found the bond atoms in resdiue, check the atom distance
					if ((first != 0) && (second != 0))
					{
						float distance = first->getPosition().getDistance(second->getPosition());
						float deviation = fabs(distance - bond_it->getLength()) / bond_it->getLength();
						if (deviation > 0.15)
						{
							Log.warn() << "ResidueChecker: in residue " << res_name << ": atom distance suspect: " 
												 << distance << " A instead of " << bond_it->getLength() << " A" << endl;
						}
					}
				}
														
			}
		}

		return Processor::CONTINUE;
	}
					
} // namespace BALL
