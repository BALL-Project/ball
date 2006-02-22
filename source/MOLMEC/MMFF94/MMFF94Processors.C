// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Processors.C,v 1.1.2.1 2006/02/22 17:51:57 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94Processors.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>

//    #define BALL_DEBUG_MMFF
#define BALL_DEBUG_TEST

using namespace std;

namespace BALL 
{

MMFF94ChargeProcessor::MMFF94ChargeProcessor()
	:	UnaryProcessor<Atom>()
{
}

MMFF94ChargeProcessor::MMFF94ChargeProcessor(const MMFF94ChargeProcessor& p)
	:	UnaryProcessor<Atom>(p)
{
}

void MMFF94ChargeProcessor::clear()
	throw()
{
}

const MMFF94ChargeProcessor& MMFF94ChargeProcessor::operator = (const MMFF94ChargeProcessor&)
	throw()
{
	return *this;
}


Processor::Result MMFF94ChargeProcessor::operator () (Atom& atom)
{
	atoms_.push_back(&atom);
	return Processor::CONTINUE;
}

void MMFF94ChargeProcessor::assignFormalCharge(Atom& atom)
{	
	const String element = atom.getElement().getSymbol();
	const String name = atom.getFullName();
	Position group = (Position) atom.getElement().getGroup();

	AtomBondIterator bit = atom.beginBond(); 
	Index bonds = 0;
	for (; +bit; ++bit)
	{
		if (bit->getOrder() < Bond::ORDER__AROMATIC)
		{
			bonds += bit->getOrder();
		}
		else
		{
			bonds += 1;
		}
	}

	// ions
	if (bonds == 0)
	{
		if (element == "H" ||
				group > 3 && group < 13)
		{
			unassigned_atoms_.insert(&atom);
			return;
		}

		Position formal = 0;

		if (group < 3)
		{
			formal = - group;
		}
		else if (group < 15)
		{
			formal = - (group - 10);
		}
		else
		{
			formal = 18 - group;
		}

		atom.setFormalCharge(formal);
		return;
	}
			
	if (element == "N")
	{
		atom.setFormalCharge(bonds - 3);
	}
	else if (element == "S") 
	{
		atom.setFormalCharge(bonds - 2);
	}
	else if (element == "O")
	{
		atom.setFormalCharge(bonds - 2);
	}
}

bool MMFF94ChargeProcessor::finish()
{
	for (Position p = 0; p < atoms_.size(); p++)
	{
		atoms_[p]->setCharge(0.0);

		if (atoms_[p]->getFormalCharge() == 0) assignFormalCharge(*atoms_[p]);
	}

	vector<Atom*> to_share;

	// charge sharing over bonds
	for (Position a = 0; a < atoms_.size(); a++)
	{
		Atom& atom = *atoms_[a];
		const Index element = atom.getElement().getAtomicNumber();

		if (atom.getFormalCharge() == 0 || atom.countBonds() == 0) continue;

		Index ring = -1;
		for (Position p = 0; p < aromatic_rings_.size(); p++)
		{
			if (aromatic_rings_[p].has(&atom))
			{
				ring = p;
				break;
			}
		}

		if (ring > -1)
		{
			HashSet<Atom*>::ConstIterator it = aromatic_rings_[ring].begin();
			for (; +it; ++it)
			{
				if ((*it)->getElement().getAtomicNumber() == element)
				{
					to_share.push_back(*it);
				}
			}
		}
		else
		{
			AtomBondIterator bit1 = atom.beginBond();
			for (; +bit1; ++bit1)
			{
				Atom& a2 = *bit1->getPartner(atom);

				AtomBondIterator bit2 = a2.beginBond();
				for (; +bit2; ++bit2)
				{
					Atom& a3 = *bit2->getPartner(a2);
					if (&a3 == &atom) continue;

					if (a3.getElement().getAtomicNumber() == element)
					{
						to_share.push_back(&a3);
					}
				}
			}
		}


		if (to_share.size() == 0)
		{
			atom.setCharge(atom.getFormalCharge());
			continue;
		}

		float new_formal_charge = atom.getFormalCharge() / (float) to_share.size();
		to_share.push_back(&atom);

		for (Position p = 0; p < to_share.size(); p++)
		{
			to_share[p]->setCharge(to_share[p]->getCharge() + new_formal_charge);
		}

		to_share.clear();
	}

	return true;
}

} // namespace BALL
