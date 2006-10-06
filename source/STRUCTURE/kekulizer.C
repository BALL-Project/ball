// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: kekulizer.C,v 1.1.2.1 2006/10/06 14:01:05 amoll Exp $
//

#include <BALL/STRUCTURE/kekulizer.h>

#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/DATATYPE/hashSet.h>
#include <BALL/STRUCTURE/smartsMatcher.h>

using namespace std;

namespace BALL
{
				
bool Kekuliser::AtomInfo::operator < (const Kekuliser::AtomInfo& info) const
{
	return (*atom) < *info.atom;
}

Kekuliser::Kekuliser()
{
}

bool Kekuliser::setup(Molecule& mol)
{
	// collect aromatic bonds and atoms to speed up SMARTS matching:
	AtomIterator ait = mol.beginAtom();
	for (; +ait; ++ait)
	{
		aromatic_atoms_.insert(&*ait);
	}


	SmartsMatcher sm;
	// dont recalculate the smallest set of smallest rings:
	// no ring information needed for smarts matcher:
	vector<vector<Atom*> > rings_vector;
 	sm.setSSSR(rings_vector);

	Position nr_ca = 0;

	vector<HashSet<const Atom*> > result;
	HashSet<const Atom*>::Iterator sit;

	//////////////////////////////////////////////////////////////
	// fix carboxlic acid:
	sm.match(result, mol, "[#8;D1]#6[#8;D1]", aromatic_atoms_);

	for (Position pos = 0; pos < result.size(); pos++)
	{
		HashSet<const Atom*>& set = result[pos];
		vector<Atom*> oxygen;
		Atom* carbon = 0;

		for (sit = set.begin(); +sit; ++sit)
		{
			if ((*sit)->getElement().getSymbol() == "C")
			{
				carbon = (Atom*)*sit;
			}
			else
			{
				oxygen.push_back((Atom*)*sit);
			}
		}

		oxygen[0]->getBond(*carbon)->setOrder(Bond::ORDER__SINGLE);
		oxygen[1]->getBond(*carbon)->setOrder(Bond::ORDER__DOUBLE);
		aromatic_atoms_.erase(oxygen[0]);
		aromatic_atoms_.erase(oxygen[1]);
		aromatic_atoms_.erase(carbon);
		nr_ca++;
	}
	
	//////////////////////////////////////////////////////////////
	// fix amidene and guanidine
	// sm.match(result, mol, "[#7;D1]#6([#7;D1])*");
	Size nr_am_gu = 0;
	result.clear();
	sm.match(result, mol, "[#7;D1]#6([#7;D1])", aromatic_atoms_);
	if (result.size() != 0)
	{
		for (Position pos = 0; pos < result.size(); pos++)
		{
			HashSet<const Atom*>& set = result[pos];
			vector<Atom*> nitrogen;
			Atom* carbon = 0;

			for (sit = set.begin(); +sit; ++sit)
			{
				if ((*sit)->getElement().getSymbol() == "C")
				{
					carbon = (Atom*)*sit;
				}
				else
				{
					nitrogen.push_back((Atom*)*sit);
				}
			}

			nitrogen[0]->getBond(*carbon)->setOrder(Bond::ORDER__SINGLE);
			nitrogen[1]->getBond(*carbon)->setOrder(Bond::ORDER__DOUBLE);
			aromatic_atoms_.erase(carbon);
			aromatic_atoms_.erase(nitrogen[0]);
			aromatic_atoms_.erase(nitrogen[1]);
			nr_am_gu++;
		}
	}
	
	//////////////////////////////////////////////////////////////
	// fix phosphonic acid
	result.clear();
	Size nr_phos = 0;
//   	sm.match(result, mol, "[p]([oD1])([oD1])([oD1])[#6,#8]");
	sm.match(result, mol, "[P]([#8;D1])([#8;D1])([#8;D1])", aromatic_atoms_);

	for (Position pos = 0; pos < result.size(); pos++)
	{
		HashSet<const Atom*>& set = result[pos];
		vector<Atom*> oxygen;
		Atom* phosphor = 0;

		for (sit = set.begin(); +sit; ++sit)
		{
			if ((*sit)->getElement().getSymbol() == "P")
			{
				phosphor = (Atom*)*sit;
			}
			else
			{
				oxygen.push_back((Atom*)*sit);
			}
		}
		
		oxygen[0]->getBond(*phosphor)->setOrder(Bond::ORDER__DOUBLE);
		oxygen[1]->getBond(*phosphor)->setOrder(Bond::ORDER__SINGLE);
		oxygen[2]->getBond(*phosphor)->setOrder(Bond::ORDER__SINGLE);

		aromatic_atoms_.erase(phosphor);
		aromatic_atoms_.erase(oxygen[0]);
		aromatic_atoms_.erase(oxygen[1]);
		aromatic_atoms_.erase(oxygen[2]);
		nr_phos++;
	}
	
	//////////////////////////////////////////////////////////////
	// fix aromatic rings
	bool ok = fixAromaticRings_();

	// recollect the remaining aromatic bonds:	
	unassigned_bonds_.clear();
	AtomBondIterator bit;
	BALL_FOREACH_BOND(mol, ait, bit)
	{
		if (bit->getOrder() == Bond::ORDER__AROMATIC)
		{
			unassigned_bonds_.push_back(&*bit);
		}
	}

#ifdef BALL_MMFF94_TEST
	Log.error() << "Kekulized bounds: " <<
	            << "CA   "  << nr_ca << " " 
							<< "NH2  "  << nr_am_gu << " " 
							<< "PHOS "  << nr_phos  << std::endl;
#endif
	return ok;
}


void Kekuliser::dump()
{
	for (Position p = 0; p < atom_infos_.size(); p++)
	{
		AtomInfo& ai = atom_infos_[p];
		Log.error() << ai.atom->getFullName() << " a. Bonds: " << ai.abonds.size()
			 << "Cur d. " << ai.curr_double 
			 << "min d. " << ai.min_double
			 << "max d. " << ai.max_double
			 << "mincd. " << ai.min_double_charged 
			 << "maxcd. " << ai.max_double_charged << std::endl;
	}
	Log.error() << std::endl;
}

bool Kekuliser::fixAromaticRings_()
{
	bool ok = true;

	calculateAromaticSystems_();
	getMaximumValence_();

	if (aromatic_systems_.size() == 0) return true;

	AtomInfo temp_ai;
	temp_ai.current_charge = 0;
	temp_ai.curr_double = 0;

	// iterate over all aromatic systems:
	vector<HashSet<Atom*> >::iterator rit = aromatic_systems_.begin();
	for (; rit != aromatic_systems_.end(); rit++)
	{
		// abort for strange rings:
		if ((*rit).size() < 3)
		{
			Log.error() << "Kekulizer: Could not assign ring with " << (*rit).size()<< " atoms. " << std::endl;

			if ((*rit).size())
			{
				Log.error() << (**(*rit).begin()).getFullName() << std::endl;
			}
			ok = false;
			continue;
		}

		atom_infos_.clear();

		bool abort_this_ring = false;

		// for one aromatic system: collect all needed informations for the individual atoms:
		HashSet<Atom*>::Iterator hit = (*rit).begin();
		for (; +hit; ++hit)
		{
			Atom& atom = *(Atom*)*hit;

			// calculate the current valence:
			Index curr_valence = 0;
			AtomBondIterator bit = atom.beginBond();
			for (; +bit; ++bit)
			{
				if (bit->getOrder() < 2 ||
						bit->getOrder() > 4)
				{
					curr_valence++;
				}
				else
				{
					curr_valence += (Index) bit->getOrder();
				}
			}

			// calculate the number of bonds that need order of two:
			Index max_double = max_valence_[&atom] - curr_valence;
			if (max_double < 0)
			{
				Log.error() << "Kekulizer: Could not calculate max number of double bonds for " << atom.getFullName() << std::endl;
				Log.error() << "Max: "  << max_valence_[&atom] << "  Curr:  " << curr_valence << std::endl;
				abort_this_ring = true;
				ok = false;
				break;
			}

			// calculate the maximum number of double bonds if atom is to be charged:
			Index max_double_charged = max_double;
			String esym = atom.getElement().getSymbol();
			if (esym == "N" ||
					esym == "S" ||
					esym == "O")
			{
				/*
				bool hydrogen_bound = false;
				AtomBondIterator abit = atom.beginBond();
				for (; +abit; ++abit)
				{
					if (abit->getPartner(atom)->getElement().getAtomicNumber() == 1)
					{
						hydrogen_bound = true;
						break;
					}
				}

				if (!hydrogen_bound)
					*/
				{
					if (esym == "N" && curr_valence == 3 ||
							esym == "S" && curr_valence == 2 ||
							esym == "O" && curr_valence == 2)
					{
						max_double_charged ++;
					}
				}
			}

			Index min_double_charged = max_double;

			if (esym == "N") 
			{
				if (curr_valence == 2) 
				{
					min_double_charged = 0;
				}
//   				min_double_charged = curr_valence;
			}

			atom_infos_.push_back(temp_ai);
			AtomInfo& info = atom_infos_[atom_infos_.size() - 1];
			info.atom = &atom;
			info.max_double_charged = max_double_charged;
			info.min_double_charged = min_double_charged;
			info.min_double 				= max_double;
			info.max_double 				= max_double;

			// collect aromatic bonds for this atom:
			bit = atom.beginBond();
			for (; +bit; ++bit)
			{
				// add aromatic bonds only once:
				if (bit->getOrder() == Bond::ORDER__AROMATIC &&
						*bit->getPartner(atom) > atom) 
				{
					// set bond order initialy to single
					bit->setOrder(Bond::ORDER__SINGLE);
					info.abonds.push_back(&*bit);
				}
			}

		} // all aromatic atoms of this ring

		if (abort_this_ring) 
		{
			continue;
		}

		std::sort(atom_infos_.begin(), atom_infos_.end());

		// put the ids of the atoms partner into a vector per AtomInfo
		HashMap<Atom*, Position> atom_to_id;
		for (Position p = 0; p < atom_infos_.size(); p++)
		{
			atom_to_id[atom_infos_[p].atom] = p;
		}

		for (Position p = 0; p < atom_infos_.size(); p++)
		{
			AtomInfo& ai = atom_infos_[p];
			for (Position b = 0; b < ai.abonds.size(); b++)
			{
				ai.partner_id.push_back(atom_to_id[ai.abonds[b]->getPartner(*ai.atom)]);
			}
		}

		dump();
		try_charge_ = false;
		if (!fixAromaticSystem_(0)) 
		{
			try_charge_ = true;
			if (!fixAromaticSystem_(0))
			{
				ok = false;
				// we were not successfull, so reset the bonds to aromatic:
				for (Position i = 0; i < atom_infos_.size(); i++)
				{
					AtomInfo& ai = atom_infos_[i];
					for (Position b = 0; b < ai.abonds.size(); b++)
					{
						ai.abonds[b]->setOrder(Bond::ORDER__AROMATIC);
					}
				}
			}
		}
	} // all aromatic systems

	return ok;
}

bool Kekuliser::fixAromaticSystem_(Position it)
{
	// no more bonds and no more atoms?
	if (it >= atom_infos_.size() - 1)
	{
		return idealValenceAchieved_();
	}

	AtomInfo& ai = atom_infos_[it];
	
	// no aromatic bonds left?
	if (ai.abonds.size() == 0)
	{
		return fixAromaticSystem_(it + 1);
		// no aromatic bonds left to setup, so in case of no success:
		// no need to reset the bonds to single order
	}

	Index max_double = ai.max_double;
	if (try_charge_) max_double = ai.max_double_charged;

	// at full valence?
	if (ai.curr_double == max_double)
	{
		if (fixAromaticSystem_(it + 1))
		{
			return true;
		}
	}
	else // not at full valence!
	{
		// protonierung?
		if (buildConjugatedSystem_(it)) return true;

		/*
		if (try_charge_ && ai.curr_double > ai.min_double_charged && ai.curr_double <= ai.max_double_charged))
		{
			if fixAromaticSystem_(it + 1) return true;
		}
		*/
	}

	for (Position b = 0; b < ai.abonds.size(); b++)
	{
		if (ai.abonds[b]->getOrder() == Bond::ORDER__DOUBLE)
		{
			ai.abonds[b]->setOrder(Bond::ORDER__SINGLE);
			ai.curr_double --;
			atom_infos_[ai.partner_id[b]].curr_double --;
		}
	}

	return false;
}


bool Kekuliser::buildConjugatedSystem_(Position it)
{
	AtomInfo& ai = atom_infos_[it];

	for (Position b = 0; b < ai.abonds.size(); b++)
	{
		// get the bond and partner atom:
		Bond* bond = ai.abonds[b];
		Position p = ai.partner_id[b];
		AtomInfo& pi = atom_infos_[p];

		if (pi.curr_double >= pi.max_double)
		{
			continue;
		}

		bond->setOrder(Bond::ORDER__DOUBLE);
		ai.curr_double++;
		pi.curr_double++;

		if (fixAromaticSystem_(it + 1)) return true;

		bond->setOrder(Bond::ORDER__SINGLE);
		ai.curr_double--;
		pi.curr_double--;
	}

	return false;
}


void Kekuliser::calculateAromaticSystems_()
{
	aromatic_systems_.clear();
	aromatic_atoms_.clear();
	// collect all aromatic ring atoms:
	vector<HashSet<Atom*> >::iterator rit = aromatic_rings_.begin();
	for (; rit != aromatic_rings_.end(); ++rit)
	{
		// all atoms in current ring:
		HashSet<Atom*>::Iterator hit = (*rit).begin();
		for (; +hit; ++hit)
		{
			// all bonds of this atom:
			AtomBondIterator bit = (**hit).beginBond();
			for (; +bit; ++bit)
			{
				if (bit->getOrder() == Bond::ORDER__AROMATIC)
				{
					aromatic_atoms_.insert(*hit);
					break;
				}
			}
		}
	}
	
	// sometimes aromatic rings are not correctly recognized:
	rit = rings_.begin();
	for (; rit != rings_.end(); ++rit)
	{
		if (rit->getSize() > 6) continue;

		// all atoms in current ring:
		HashSet<Atom*>::Iterator hit = (*rit).begin();
		for (; +hit; ++hit)
		{
			// all bonds of this atom:
			AtomBondIterator bit = (**hit).beginBond();
			for (; +bit; ++bit)
			{
				if (bit->getOrder() == Bond::ORDER__AROMATIC)
				{
					aromatic_atoms_.insert(*hit);
					break;
				}
			}
		}
	}

	// iterate over all aromatic ring atoms:
	while (aromatic_atoms_.size() > 0)
	{
		// first ring aromatic atom that is still left:
		Atom* atom = (Atom*)*aromatic_atoms_.begin();

		current_aromatic_system_.clear();

		collectSystems_(*atom);

		aromatic_systems_.push_back(current_aromatic_system_);
	}
}

void Kekuliser::collectSystems_(Atom& atom)
{
	current_aromatic_system_.insert(&atom);
	aromatic_atoms_.erase(&atom);

	// all bonds of this atom:
	AtomBondIterator abit = atom.beginBond();
	for (; +abit; ++abit)
	{
		// if aromatic bond:
		if (abit->getOrder() != Bond::ORDER__AROMATIC) continue;

		Atom* partner = abit->getPartner(atom);
		
		// if not seen partner before:
		if (!aromatic_atoms_.has(partner)) continue;

		collectSystems_(*partner);
	}
}

void Kekuliser::getMaximumValence_()
{
	max_valence_.clear();

	vector<HashSet<Atom*> >::iterator rit = aromatic_systems_.begin();
	for (; rit != aromatic_systems_.end(); rit++)
	{
		HashSet<Atom*>::Iterator hit = (*rit).begin();
		for (; +hit; ++hit)
		{
			Atom& atom = **hit;
			Index max_valence = 0;
			if (max_valence_.has(&atom)) continue;

//   			Index formal_charge = (Index)atom.getFormalCharge();

			Position atomic_number = atom.getElement().getAtomicNumber();

			switch (atomic_number)
			{
				case 6:
					max_valence = 4;// - (int) fabs(formal_charge);
					break;

				case 8:
				case 16:
				case 34:
				case 52:
					max_valence = 2;// + formal_charge;
					break;

				 case 7:
				 case 15:
				 case 33:
					 max_valence = 3;// + formal_charge;
					 break;
			}

			// Nitrogen and sulfur:
			if (atomic_number == 7 || 
					atomic_number == 16)
			{
				AtomBondIterator abit = atom.beginBond();
				for (; +abit; ++abit)
				{
					if (abit->getOrder() != Bond::ORDER__DOUBLE) continue;

					if (abit->getPartner(atom)->getElement().getAtomicNumber() == 8)
					{
						max_valence += 2;
					}
				}
			}

			max_valence_[&atom] = max_valence;
		}
	}
}


bool Kekuliser::idealValenceAchieved_()
{
Log.error() << "#~~#   1 "             << " "  << __FILE__ << "  " << __LINE__<< std::endl;
dump();
	for (Position p = 0; p < atom_infos_.size(); p++)
	{
		AtomInfo& ai = atom_infos_[p];
		if (!try_charge_)
		{
			if (ai.curr_double < ai.min_double ||
					ai.curr_double > ai.max_double)
			{
				return false;
			}
		}

		if (ai.curr_double < ai.min_double_charged ||
				ai.curr_double > ai.max_double_charged)
		{
			return false;
		}
	}

	return true;
}


void Kekuliser::clear()
{
	aromatic_systems_.clear();
	aromatic_rings_.clear();
	unassigned_bonds_.clear();
	aromatic_atoms_.clear();
	max_valence_.clear();
	current_aromatic_system_.clear();
	atom_infos_.clear();
	try_charge_ = false;
}

} // namespace BALL
