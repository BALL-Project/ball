/* TRANSLATOR BALL::STRUCTURE::DisulfidBondProcessor

		Necessary for lupdate.
*/

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/STRUCTURE/disulfidBondProcessor.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/forEach.h>

using namespace std;

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

			// detect and assign
			Atom* a1 = bond_it->getFirstAtom();  // dynamic_cast<PDBAtom*>(const_cast<Atom*>(b->getFirstAtom()));
			Atom* a2 = bond_it->getSecondAtom(); // dynamic_cast<PDBAtom*>(const_cast<Atom*>(b->getSecondAtom()));
			if ((a1 != 0) && (a2 != 0)
					&& (a1->getElement() == PTE[Element::S]) && (a2->getElement() == PTE[Element::S])
					&& (residue1 != residue2)
					&& (residue1 != 0) && (residue2 != 0)
					&& (residue1->hasProperty(Residue::PROPERTY__AMINO_ACID))
					&& (residue2->hasProperty(Residue::PROPERTY__AMINO_ACID)))
			{
				residue1->setProperty(Residue::PROPERTY__HAS_SSBOND);
				residue2->setProperty(Residue::PROPERTY__HAS_SSBOND);
			}

			// store for us
			if ((bond_it->getFirstAtom()->getElement() == PTE[Element::S])
					&& (bond_it->getSecondAtom()->getElement() == PTE[Element::S])
					&& (residue1->hasProperty(Residue::PROPERTY__HAS_SSBOND) == true)
					&& (residue2->hasProperty(Residue::PROPERTY__HAS_SSBOND) == true)
					&& (residue1 != residue2)
					&& (residue1 != 0)
					&& (residue2 != 0))
			{
				sulfur_bridges_.insert(pair<Residue*, Residue*>(residue1, residue2));
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

	bool DisulfidBondProcessor::connect(Atom* atom1, Atom* atom2, bool toggle)
	{
		Bond* bonded = (atom1->getBond(*atom2));
		bool success = false;

		if (bonded && toggle)
		{
			success = disconnect(atom1, atom2);
		} // else if already bonded, nothing to do!
		else if (   (bonded == NULL)
				     &&  !(   atom1->getResidue()->hasProperty(Residue::PROPERTY__HAS_SSBOND)
							       || atom1->getResidue()->hasProperty(Residue::PROPERTY__HAS_SSBOND)
						)       )
		{
			// if already bonded, nothing to do!
			if (   (atom1->getElement() == PTE[Element::S])
					&& (atom2->getElement() == PTE[Element::S]))
			{
				// Do the atoms live in the same system?
				Composite* anchestor = atom1->getLowestCommonAncestor(*atom2);
				if (anchestor)
				{
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
						if(hydrogen1->isAutoDeletable())
						{
							delete hydrogen1;
						}
						else
						{
							hydrogen1->destroy();
						}
					}

					if (hydrogen2)
					{
						if(hydrogen2->isAutoDeletable())
						{
							delete hydrogen2;
						}
						else
						{
							hydrogen2->destroy();
						}
					}

					// add a bond 		
					Bond* bond = atom1->createBond(*atom2);

					if (bond)
					{
						bond->setOrder(Bond::ORDER__SINGLE);
						atom1->getResidue()->setProperty(Residue::PROPERTY__HAS_SSBOND);
						atom2->getResidue()->setProperty(Residue::PROPERTY__HAS_SSBOND);
						bond->setType(Bond::TYPE__DISULPHIDE_BRIDGE);
						sulfur_bridges_.insert(pair<Residue*, Residue*>(atom1->getResidue(), atom2->getResidue()));
						success = true;
					}
					else
					{
						success = false;
					}
				}
			}
		}
		return success;
	}

	bool DisulfidBondProcessor::connect(Residue* residue1, Residue* residue2, bool toggle)
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
			success = connect(atom1, atom2, toggle);
		}
		return success;
	}

	bool DisulfidBondProcessor::connect(Composite* composite1, Composite* composite2, bool toggle)
	{
		bool success = false;
        if (RTTI::isKindOf<Residue>(composite1) && RTTI::isKindOf<Residue>(composite2) )
		{
			Residue* res1 = reinterpret_cast<Residue*>(composite1);
			Residue* res2 = reinterpret_cast<Residue*>(composite2);
			success = connect(res1, res2, toggle);
		}
        else if (RTTI::isKindOf<Atom>(composite1) && RTTI::isKindOf<Atom>(composite2))
		{
				Atom* atom1 = reinterpret_cast<Atom*>(composite1);
				Atom* atom2 = reinterpret_cast<Atom*>(composite2);
				success = connect(atom1, atom2, toggle);
		}
		else
		{
			Log.warn() << "DisulfidBondProcessor: Invalid object type(s). Allowed objects are atoms or residues." << endl;
		}
		return success;
	}

	bool DisulfidBondProcessor::disconnect(Atom* atom1, Atom* atom2)
	{
		// if already disconnected, nothing to do!
		Bond* bond = atom1->getBond(*atom2);
		bool success = false;
		if (bond && (atom1->getElement() == PTE[Element::S])
			       && (atom2->getElement() == PTE[Element::S]))
		{
			if(bond->isAutoDeletable())
			{
				delete bond;
			}
			else
			{
				bond->destroy();
			}
			success = true;

			// Unfortunately, use of FragmentDB to rebuild residues, i.e. add hydrogens
			// does not work, so we do it manually	
			PDBAtom* hydrogen1 = new PDBAtom(PTE[Element::H], PTE[Element::H].getSymbol());
			PDBAtom* hydrogen2 = new PDBAtom(PTE[Element::H], PTE[Element::H].getSymbol());

			Vector3 bond = atom1->getPosition() - atom2->getPosition();
			bond = bond.normalize();

			hydrogen1->setPosition(atom1->getPosition() - bond * 1.34);
			hydrogen2->setPosition(atom2->getPosition() + bond * 1.34);

			Bond* b1 = hydrogen1->createBond(*atom1);
			b1->setOrder(1);
			Bond* b2 = hydrogen2->createBond(*atom2);
			b2->setOrder(1);

			atom1->getResidue()->insert(*hydrogen1);
			atom2->getResidue()->insert(*hydrogen2);


/*			FragmentDB fdb = FragmentDB("");
			ReconstructFragmentProcessor rfp(fdb);
			atom1->getResidue()->getProtein()->apply(fdb.normalize_names);
			atom1->getResidue()->getProtein()->apply(rfp);
			atom1->getResidue()->getProtein()->apply(fdb.build_bonds);

			atom1->getResidue()->getParent()->apply(fdb.normalize_names);
			atom1->getResidue()->getParent()->apply(fdb.add_hydrogens);
			atom1->getResidue()->getParent()->apply(fdb.build_bonds);

			atom2->getResidue()->getParent()->apply(fdb.normalize_names);
			atom2->getResidue()->getParent()->apply(fdb.add_hydrogens);
			atom2->getResidue()->getParent()->apply(fdb.build_bonds);
*/
			// delete property
			Residue* residue1 = atom1->getResidue();
			Residue* residue2 = atom2->getResidue();
			residue1->clearProperty(Residue::PROPERTY__HAS_SSBOND);
			residue2->clearProperty(Residue::PROPERTY__HAS_SSBOND);

			// remove from internal list
			DisulfidBonds::iterator bond_it = sulfur_bridges_.begin();
			for ( ; bond_it != sulfur_bridges_.end();  ++bond_it)
			{
				if (   (bond_it->first == residue1 && bond_it->second == residue2)
					  || (bond_it->first == residue2 && bond_it->second == residue1))
				{
					sulfur_bridges_.erase(bond_it);
					break;
				}
			}
		}
		else
		{
			Log.warn() << "DisulfidBondProcessor: No disulfid bond found to disconnect!" << endl;
		}
		return success;
	}

	bool DisulfidBondProcessor::disconnect(Residue* residue1, Residue* residue2)
	{
		bool success = !(   residue1->hasProperty(Residue::PROPERTY__HAS_SSBOND)
									   && residue2->hasProperty(Residue::PROPERTY__HAS_SSBOND));
		if (success)
		{
			Log.error() << "DisulfidBondProcessor: Disconnect not possible!" << endl;
		}
		else
		{
			Atom* atom1 = NULL;
			Atom* atom2 = NULL;
			if (   (Peptides::OneLetterCode(residue1->getName()) == 'C')
					&& (Peptides::OneLetterCode(residue2->getName()) == 'C'))
			{
				// find S 
				AtomIterator a_it = residue1->beginAtom();
				for (; +a_it && !atom1; ++a_it)
				{
					if (a_it->getElement() == PTE[Element::S])
					{
						atom1 = &*a_it;
					}
				}
				a_it = residue2->beginAtom();
				for (; +a_it && !atom2; ++a_it)
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
				success = disconnect(atom1, atom2);
			}
		}
		return success;
	}

	bool DisulfidBondProcessor::disconnect(Composite* composite1, Composite* composite2)
	{
		bool success = false;
        if (RTTI::isKindOf<Residue>(composite1) && RTTI::isKindOf<Residue>(composite2) )
		{
			Residue* res1 = reinterpret_cast<Residue*>(composite1);
			Residue* res2 = reinterpret_cast<Residue*>(composite2);
			success = disconnect(res1, res2);
		}
        else if (RTTI::isKindOf<Atom>(composite1) && RTTI::isKindOf<Atom>(composite2))
		{
				Atom* atom1 = reinterpret_cast<Atom*>(composite1);
				Atom* atom2 = reinterpret_cast<Atom*>(composite2);
				success = disconnect(atom1, atom2);
		}
		else
		{
			Log.warn() << "DisulfidBondProcessor: Invalid object type(s). Allowed objects are atoms or residues." << endl;
		}
		return success;
	}
}
