/* TRANSLATOR BALL::STRUCTURE::DisulfidBondProcessor

		Necessary for lupdate.
*/

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/STRUCTURE/disulfidBondProcessor.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/forEach.h>

namespace BALL
{
	DisulfidBondProcessor::DisulfidBondProcessor()
	{
	}

	DisulfidBondProcessor::~DisulfidBondProcessor()
	{
	}

	bool DisulfidBondProcessor::start()
	{
		clear();
		return true;
	}

	Processor::Result DisulfidBondProcessor::operator () (AtomContainer& ac)
	{
		AtomIterator atom_it;
		AtomBondIterator bond_it;

		Residue* residue1 = NULL;
		Residue* residue2 = NULL;

		BALL_FOREACH_BOND(ac, atom_it, bond_it)
		{
			residue1 = bond_it->getFirstAtom()->getResidue();
			residue2 = bond_it->getSecondAtom()->getResidue();

			if ((bond_it->getFirstAtom()->getElement() == PTE[Element::S])
					&& (bond_it->getSecondAtom()->getElement() == PTE[Element::S])
					&& (residue1->hasProperty(Residue::PROPERTY__HAS_SSBOND) == true)
					&& (residue2->hasProperty(Residue::PROPERTY__HAS_SSBOND) == true)
					&& (residue1 != residue2)
					&& (residue1 != 0)
					&& (residue2 != 0))
			{
				sulfur_bridges_.push_back(pair<Residue*, Residue*>(residue1, residue2));
			}
		}

		return Processor::BREAK;
	}

	bool DisulfidBondProcessor::finish()
	{
		return true;
	}

	void DisulfidBondProcessor::clear()
	{
		sulfur_bridges_.clear();
	}

	bool DisulfidBondProcessor::connect(Atom* atom1, Atom* atom2)
	{
		// if already bonded, nothing to do!
		bool success = !(atom1->getBond(*atom2));

		if (success && (atom1->getElement() == PTE[Element::S])
			          && (atom2->getElement() == PTE[Element::S]))
		{
			// Do the atoms live in the same system?
			Composite* anchestor = atom1->getLowestCommonAncestor(*atom2);
			if (anchestor)
			{
				// valid distance ?  else warning!
				//if (atom1.getDistance(atom2) < 3) 
				////TODO: decent S-S bond distance!

				// find and delete hydrogens
				Atom* hydrogen1 = NULL;
			  Atom* hydrogen2 = NULL;
				Atom::BondIterator b_it  = atom1->beginBond();
				for ( ; +b_it; ++b_it)
				{
					if (b_it->getPartner(*atom1)->getElement() == PTE[Element::H])
					{
						hydrogen1 = b_it->getPartner(*atom1);
					}
				}
				b_it  = atom2->beginBond();
				for ( ; +b_it; ++b_it)
				{
					if (b_it->getPartner(*atom2)->getElement() == PTE[Element::H])
					{
						hydrogen2 = b_it->getPartner(*atom2);
					}
				}

				if (hydrogen1)
				{
					hydrogen1->destroy();
				}
				if (hydrogen2)
				{
					hydrogen2->destroy();
				}

				// add a bond 		
				Bond* bond = atom1->createBond(*atom2);
				if (bond)
				{
					bond->setOrder(Bond::ORDER__SINGLE);
					atom1->getResidue()->setProperty(Residue::PROPERTY__HAS_SSBOND);
					atom2->getResidue()->setProperty(Residue::PROPERTY__HAS_SSBOND);
					bond->setType(Bond::TYPE__DISULPHIDE_BRIDGE);
					sulfur_bridges_.push_back(pair<Residue*, Residue*>(atom1->getResidue(), atom2->getResidue()));
				}
				else
				{
					success = false;
				}
			}
		}
		return success;
	}

	bool DisulfidBondProcessor::connect(Residue* residue1, Residue* residue2)
	{
		bool success = false;
		Atom* atom1 = NULL;
		Atom* atom2 = NULL;

		if (   (Peptides::OneLetterCode(residue1->getName()) == 'C')
				&& (Peptides::OneLetterCode(residue2->getName()) == 'C'))
		{
			// find S 
			AtomIterator a_it = residue1->beginAtom();
			for (; +a_it; ++a_it)
			{
				if (a_it->getElement() == PTE[Element::S])
				{
					atom1 = &*a_it;
				}
			}
			a_it = residue2->beginAtom();
			for (; +a_it; ++a_it)
			{
				if (a_it->getElement() == PTE[Element::S])
				{
					atom2 = &*a_it;
				}
			}
		}
		else
		{
			Log.warn() << "DisulfidBondProcessor: Invalid residue type. Select two cystein residues." << endl;
		}

		if (atom1 && atom2)
		{
			success = connect(atom1, atom2);
		}
		return success;
	}

	bool DisulfidBondProcessor::connect(Composite* composite1, Composite* composite2)
	{
		bool success = false;
		if (RTTI::isKindOf<Residue>(*composite1) && RTTI::isKindOf<Residue>(*composite2) )
		{
			Residue* res1 = reinterpret_cast<Residue*>(composite1);
			Residue* res2 = reinterpret_cast<Residue*>(composite2);
			success = connect(res1, res2);
		}
		else if (RTTI::isKindOf<Atom>(*composite1) && RTTI::isKindOf<Atom>(*composite2))
		{
				Atom* atom1 = reinterpret_cast<Atom*>(composite1);
				Atom* atom2 = reinterpret_cast<Atom*>(composite2);
				success = connect(atom1, atom2);
		}
		else
		{
			Log.warn() << "DisulfidBondProcessor: Invalid object type(s). Allowed objects are atoms or residues." << endl;
		}
		return success;
	}
}
